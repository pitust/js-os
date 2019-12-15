#include "console.h"
#include "console_vga.h"
#include "bios.h"
#include "intr.h"
#include "libk.h"
#include "timer.h"
#include "keyboard.h"
int _2nd_main___();
#define MEGA 1048576
void Mouse_Init();
void main()
{
    ConsoleVGA_Init();
    // Console_WriteString("[[kmain]] Detecting memory\n");
    Regs r;
    r.ax = 0xe801;
    r.bx = 0;
    r.cx = 0;
    r.dx = 0;
    BIOS_Call(0x15, &r);
    size_t memlo = r.cx;
    size_t memhi = r.dx;
    if (r.cx == 0 || r.dx == 0)
    {
        memlo = r.ax;
        memhi = r.bx;
    }
    memlo *= 1024;  // the result is in 1K blocks
    memhi *= 65536; // the result is in 64K blocks here
    if (memlo > memhi)
    {
        // More low memory!!!
        memconfig(MEGA, memlo);
    }
    else
    {
        // More high memory!!!
        memconfig(MEGA * 16, memhi);
    }
    printf("memconfig finished: found %x of low mem, %x of high mem\n", memlo, memhi);
    Intr_SetFaultHandlers(Console_UnhandledFault);
    Console_WriteString("[[kmain]] calling _2nd_main___\n");
    _2nd_main___();
    Console_WriteString("[[kmain]] exiting...");
}
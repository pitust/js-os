#include <libjs.h>
#include <intr.h>
#include <setjmp.h>
#include <bios.h>
#include <keyboard.h>
#define WRREG(regNm, regs, nmParam)   \
    if (strcmp(#regNm, nmParam) == 0) \
    {                                 \
        regs->regNm = regVal;         \
        return;                       \
    }
#define RDREG(regNm, regs, nmParam)   \
    if (strcmp(#regNm, nmParam) == 0) \
    return regs->regNm
int64_t Nat_GETREGS()
{
    return (int64_t)(uint32_t)malloc(sizeof(Regs));
}
void Nat_WRREG(int64_t regsID, char *regNM, int64_t regVal)
{
    Regs *r = (Regs *)(uint32_t)regsID;
    WRREG(eax, r, regNM);
    WRREG(ebx, r, regNM);
    WRREG(ecx, r, regNM);
    WRREG(edx, r, regNM);

    WRREG(ax, r, regNM);
    WRREG(bx, r, regNM);
    WRREG(cx, r, regNM);
    WRREG(dx, r, regNM);

    WRREG(ah, r, regNM);
    WRREG(bh, r, regNM);
    WRREG(ch, r, regNM);
    WRREG(dh, r, regNM);

    WRREG(al, r, regNM);
    WRREG(bl, r, regNM);
    WRREG(cl, r, regNM);
    WRREG(dl, r, regNM);

    WRREG(si, r, regNM);
    WRREG(di, r, regNM);

    WRREG(ds, r, regNM);
    WRREG(es, r, regNM);
    WRREG(eflags, r, regNM);
    WRREG(cf, r, regNM);
    WRREG(pf, r, regNM);
    WRREG(zf, r, regNM);
    WRREG(sf, r, regNM);
    WRREG(tp, r, regNM);
    WRREG(intf, r, regNM);
    WRREG(df, r, regNM);
    WRREG(iopl, r, regNM);
    WRREG(rf, r, regNM);
    WRREG(vm, r, regNM);
    WRREG(vif, r, regNM);
    WRREG(id, r, regNM);
    WRREG(edi, r, regNM);
    WRREG(esi, r, regNM);
    WRREG(esp, r, regNM);
    WRREG(ebp, r, regNM);
    printf("Unknown reg: %s\n", regNM);
    while (1)
        ;
}
int64_t Nat_RDREG(int64_t regsID, char *regNM)
{
    Regs *r = (Regs *)(uint32_t)regsID;
    RDREG(eax, r, regNM);
    RDREG(ebx, r, regNM);
    RDREG(ecx, r, regNM);
    RDREG(edx, r, regNM);
    RDREG(ax, r, regNM);
    RDREG(bx, r, regNM);
    RDREG(cx, r, regNM);
    RDREG(dx, r, regNM);

    RDREG(ah, r, regNM);
    RDREG(bh, r, regNM);
    RDREG(ch, r, regNM);
    RDREG(dh, r, regNM);

    RDREG(si, r, regNM);
    RDREG(di, r, regNM);

    RDREG(al, r, regNM);
    RDREG(bl, r, regNM);
    RDREG(cl, r, regNM);
    RDREG(dl, r, regNM);
    RDREG(ds, r, regNM);
    RDREG(es, r, regNM);
    RDREG(eflags, r, regNM);
    RDREG(cf, r, regNM);
    RDREG(pf, r, regNM);
    RDREG(zf, r, regNM);
    RDREG(sf, r, regNM);
    RDREG(tp, r, regNM);
    RDREG(intf, r, regNM);
    RDREG(df, r, regNM);
    RDREG(iopl, r, regNM);
    RDREG(rf, r, regNM);
    RDREG(vm, r, regNM);
    RDREG(vif, r, regNM);
    RDREG(id, r, regNM);
    RDREG(edi, r, regNM);
    RDREG(esi, r, regNM);
    RDREG(esp, r, regNM);
    RDREG(ebp, r, regNM);

    printf("Unknown reg: %s\n", regNM);
    while (1)
        ;
}
void Nat_BIOS_INT(int64_t regsID, int64_t vec)
{
    Regs *r = (Regs *)(uint32_t)regsID;
    BIOS_Call(vec, r);
}
int64_t Nat_RDMEM(int64_t addr)
{
    return (int64_t)(*((uint8_t *)(uint32_t)addr));
}
void Nat_WRMEM(int64_t addr, int64_t data)
{
    *((uint8_t *)(uint32_t)addr) = (uint8_t)data;
}
void Nat_WRMEM16(int64_t addr, int64_t data)
{
    *((uint16_t *)(uint32_t)addr) = (uint16_t)data;
}
void Nat_WRMEM32(int64_t addr, int64_t data)
{
    *((uint32_t *)(uint32_t)addr) = (uint32_t)data;
}
void Nat_PUTC(int64_t c) {
    printf("%c", c);
}
void Nat_DUMPMEM()
{
    dump_memory();
}
IntrContext ctxs[256];
void hnd(int vec);
int pass = 0;
void intinited();
int readInt(int intID)
{
    intinited();
    Intr_Disable();
    IntrTrampoline[intID].handler = hnd;
    Intr_SaveContext(&ctxs[intID]);
    if (pass == 1)
    {
        pass = 0;
        return 1;
    }
    Intr_Enable();
    return 0;
}
void hnd(int vec)
{
    pass = 1;
    Intr_RestoreContext(&ctxs[vec]);
}
int intInit = 0;
void intinited()
{
    if (intInit)
        return;
    intInit = 1;
    Intr_Init();
    Intr_SetFaultHandlers(Console_UnhandledFault);
    Intr_Enable();
}
int64_t Nat_GET_BIOSBUF_ADDR()
{
    return (int64_t)(uint32_t)&BIOS_SHARED->userdata;
}
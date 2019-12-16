#include "libk.h"
#define INTR(vec) if(readInt(vec))return;
int readInt(int vec);
int64_t Nat_GETREGS();
void Nat_DUMPMEM();
void Nat_WRREG(int64_t regsID, char* regNM, int64_t regVal);
int64_t Nat_RDREG(int64_t regsID, char* regNM);
void Nat_BIOS_INT(int64_t regsID, int64_t vec);
int64_t Nat_RDMEM(int64_t addr);
void Nat_WRMEM(int64_t addr, int64_t data);
void Nat_WRMEM16(int64_t addr, int64_t data);
void Nat_WRMEM32(int64_t addr, int64_t data);
int64_t Nat_GET_BIOSBUF_ADDR();
void Nat_PUTC(int64_t c);
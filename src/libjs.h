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
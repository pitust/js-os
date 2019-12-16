let regs = Nat_GETREGS();
const DRIVE = 0x80;
let bbuf = Nat_GET_BIOSBUF_ADDR();
Nat_WRREG(regs, "ah", 0x41);
Nat_WRREG(regs, "bx", 0x55AA);
Nat_WRREG(regs, "dl", DRIVE);
Nat_WRREG(regs, "cf", 0);
Nat_BIOS_INT(regs, 0x13);
if (Nat_RDREG(regs, "cf") == 1) {
    console.log("LBA ext check fail. ah =", Nat_RDREG(regs, "ah"));
    while (1);
}
let dapAddr = (bbuf + 3) / 4 * 4;
let bufAddr = dapAddr + 32;
function readLBA(lba) {
    Nat_WRMEM(dapAddr + 0, 16);                             // length of the DAP
    Nat_WRMEM(dapAddr + 1, 0);                              // always zero
    Nat_WRMEM16(dapAddr + 2, 1);                            // Sectors to read
    Nat_WRMEM16(dapAddr + 4, bufAddr);                      // offset
    Nat_WRMEM16(dapAddr + 6, 0);                            // segment
    Nat_WRMEM32(dapAddr + 8, 0);                            // 64 - 32 lba (all zeroes; lba is 48bit; only low 4GB addressable for now)
    Nat_WRMEM32(dapAddr + 12, lba & 0xffffff);              // 32 - 0 lba
    Nat_WRREG(regs, "ds", 0);
    Nat_WRREG(regs, "dl", DRIVE);
    Nat_WRREG(regs, "si", dapAddr);
    Nat_WRREG(regs, "ah", 0x42);
    Nat_BIOS_INT(regs, 0x13);
    let buf = [];
    buf[512] = 0;
    for (let i = 0; i < 512; i++) {
        let v = Nat_RDMEM(i + bufAddr);
        buf[i] = v;
    }
    return buf;
}
if (Nat_RDREG(regs, "cf") == 1) {
    console.log("LBA ext read fail. ah =", Nat_RDREG(regs, "ah"));
    while (1);
}
console.log('Print...');
let xbuf = readLBA(0), lbax = 0;
for (let i = 0; i < 512; i++) {
    let v = xbuf[i];
    if (v == 0) break;
    Nat_PUTC(v);
}
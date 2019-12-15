let regs = Nat_GETREGS();
let sectorsPerTrack, headCount;
const DRIVE = 0x1;
let bbuf = Nat_GET_BIOSBUF_ADDR();

Nat_WRREG(regs, "ah", 8);
Nat_WRREG(regs, "dl", DRIVE); // read frop the hda
Nat_BIOS_INT(regs, 0x13); // INT13
if (Nat_RDREG(regs, "cf") == 1) {
    // Carry Set!!!
    console.log('Carry set while reading drive props!');
    while (true);
}
headCount = Nat_RDREG(regs, "dh") + 1;
sectorsPerTrack = Nat_RDREG(regs, "cl") & 0x3f;
console.log("Drive gemetry:");
console.log("  Sectors Per Track:", sectorsPerTrack);
console.log("  Head Count:", headCount);
let lba = 0;
let tmp = lba / sectorsPerTrack;
let sect = (lba % sectorsPerTrack) + 1;
let head = tmp % headCount;
let cyllinder = tmp / headCount;
Nat_WRREG(regs, "ah", 2);
Nat_WRREG(regs, "al", 1);
Nat_WRREG(regs, "ch", cyllinder & 0xff);
Nat_WRREG(regs, "cl", sect | ((cyllinder >> 2) & 0xC0));
Nat_WRREG(regs, "dh", head);
Nat_WRREG(regs, "es", 0);
Nat_WRREG(regs, "bx", bbuf);
Nat_WRREG(regs, "dl", DRIVE);
Nat_BIOS_INT(regs, 0x13); // INT13
if (Nat_RDREG(regs, "cf") == 1) {
    // Carry Set!!!
    console.log('Carry set while reading the drive!');
    while (true);
}
let buf = [];
buf[512] = 0;
for (let i = 0; i < 512; i++) {
    buf[i] = Nat_RDMEM(i + bbuf);
}

for (let i = 0; i < 512 && buf[i] != 0; i++) {
    Nat_PUTC(buf[i]);
}
Nat_PUTC(10);
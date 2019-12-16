const SECCOUNT = 1;
function readDisk(LBA) {
    let readBuf = [];
    readBuf[511] = 0;
    Nat_OUTB(0x1F6, 0xE0 | ((LBA >> 24) & 0x0F));
    Nat_OUTB(0x1F1, 0x00);
    Nat_OUTB(0x1F2, SECCOUNT);
    Nat_OUTB(0x1F3, LBA);
    Nat_OUTB(0x1F4, LBA >> 8);
    Nat_OUTB(0x1F5, LBA >> 16);
    Nat_OUTB(0x1F7, 0x20);
    while (!(Nat_INB(0x1F7) & 8));
    for (let i = 0; i < 256; i++) {
        let val = Nat_INW(0x1F0);
        readBuf[i * 2] = val & 0xff;
        readBuf[i * 2 + 1] = val >> 8;
    }
    return readBuf;
}
let buf = readDisk(0), lr = 0;
for (let i = 0; i < 512; i++) {
    let v = buf[i];
    if (v == 0) break;
    Nat_PUTC(v);
    if (i == 511) {
        buf = readDisk(++lr);
        i = 0;
    }
}
console.log('\nDONE');
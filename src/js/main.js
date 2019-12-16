function outb(port, val) {
    Nat_OUTB(port, val);
}
let LBA = 0;
const SECCOUNT = 1;
outb(0x1F6, 0xE0 | ((LBA >> 24) & 0x0F));
outb(0x1F1, 0x00);
outb(0x1F2, SECCOUNT);
outb(0x1F3, LBA);
outb(0x1F4, LBA >> 8);
outb(0x1F5, LBA >> 16);
outb(0x1F7, 0x20);
while (!(Nat_INB(0x1F7) & 8));
let buf = [];
for (let i = 0;i < 256;i++) {
    let val = Nat_INW(0x1F0);
    buf[i*2] = val & 0xff;
    buf[i*2 + 1] = val >> 8;
}
for (let i = 0;i < 512;i++) {
    let v = buf[i];
    if (v == 0)break;
    Nat_PUTC(v);
}
console.log('\nDONE');
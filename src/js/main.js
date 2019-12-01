let i = 0;
while (++i) {
    console.log(i);
    Nat_WRMEM(0xb8000, 0x48484848);
}
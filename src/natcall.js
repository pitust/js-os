module.exports = [{
    name: 'GETREGS', // GETREGS(): RegsID
    argc: 0,
    rets: 'number'
}, {
    name: 'WRREG', // WRREG(RegsID, reg, val),
    argc: 3,
    rets: 'void'
}, {
    name: 'RDREG', // RDREG(RegsID, reg): regVal
    argc: 2,
    rets: 'number'
}, {
    name: 'BIOS_INT', // BIOS_INT(RegsID, vec)
    argc: 2,
    rets: 'void'
}, {
    name: 'RDMEM', // RDMEM(addr): val
    argc: 1,
    rets: 'number'
}, {
    name: 'WRMEM', // WRMEM(addr, val)
    argc: 2,
    rets: 'void'
}, {
    name: 'DUMPMEM', // DUMPMEM()
    argc: 0,
    rets: 'void'
}];
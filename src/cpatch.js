let fs = require('fs');
let natcall = require('./natcall');
// GC fixes
let pc = `free(gc_main_dicts->data[gc_i]->index->data);free(gc_main_dicts->data[gc_i]->index);free(gc_main_dicts->data[gc_i]->values->data);free(gc_main_dicts->data[gc_i]->values);`.split(';');
let mc = fs.readFileSync('out/main.c').toString();
for (let i = 0; i < 4; i++) {
    mc = mc.replace(new RegExp(pc[i].replace(/([\(\)\[\]])/g, '\\$1') + ';', 'g'), '');
}
mc = mc.replace('#include <string.h>', '');
// Large Ints
mc = mc.replace(/typedef short int16_t;/g, '').replace(/int16_t/g, 'int64_t');
// C FFI
let retRep = { number: 'int64_t', string: 'char*', void: 'void' };
let suf = '\\((.*)\\)\\n\\{[^}]+\\}';
mc = '#include <libjs.h>\n' + mc;
for (let e of natcall) {
    let re = retRep[e.rets] + ' Nat_' + e.name + suf;
    let reObj = new RegExp(re, 'g');
    mc = mc.replace(reObj, '');
}
mc = mc.replace(/void INTR[^}]+\}/g, '').replace(/\n\n+/g,'\n');
fs.writeFileSync('out/main.c', mc);
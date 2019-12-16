let babel = require('@babel/standalone')
let fs = require('fs');
let i = '';
for (let e of fs.readdirSync('src/js')) {
    i += fs.readFileSync('src/js/' + e).toString();
}
let funcs = require('./natcall.js');
let retV = { number: '42', string: '""', void: '' };
let fc = 'function INTR(id) {};function getc(){return \'\';}';
for (let e of funcs) {
    fc += `function Nat_${e.name}(${' '.repeat(e.args.length).split('').map((_e, i) => 'a_' + i).join(',')}) {return ${retV[e.rets]};}`;
}
fs.writeFileSync('out/main.js', fc + '\n' + babel.transform(i, { presets: ['es2015-loose'] }).code)
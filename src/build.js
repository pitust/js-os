let babel = require('@babel/standalone')
let fs = require('fs');
let i = '';
for (let e of fs.readdirSync('src/js')) {
    i += fs.readFileSync('src/js/' + e).toString();
}

fs.writeFileSync('out/babeld.js', babel.transform(i, { presets: ['es2015-loose'] }).code)
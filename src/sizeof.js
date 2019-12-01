let fs = require('fs');
let proc = require('child_process');
let all = proc.execSync('find .').toString().split('\n').map(e => e.trim()).filter(e => e);
let flen = [], glen = 0;
for (let e of all) {
    if (e.includes('.git')) continue;
    if (fs.statSync(e).isDirectory()) continue;
    let len = fs.readFileSync(e).toString().split('\n').length;
    flen.push({ len, file: e });
    glen += len;
}
console.log('   Total lines:', glen);
console.log('  ---- By Lines ----');
let list = flen.sort((a, b) => b.len - a.len);
list.forEach(e => {
        console.log(e.file.padStart(50) + ' '.repeat(10) + e.len);
})
let fs = require('fs');
let proc = require('child_process');
let all = proc.execSync('find .').toString().split('\n').map(e => e.trim()).filter(e => e);
let flen = [], glen = 0;
let forbid = ['.git', 'node_modules', 'metalkit', '.o', '.elf', 'out', '.img']
for (let e of all) {
    let a = false;
    for (let q of forbid) {
        if (e.includes(q)) {
            a = true;
            break;
        }
    }
    if (a) continue;
    if (fs.statSync(e).isDirectory()) continue;
    let len = fs.readFileSync(e).toString().split('\n').length + 1;
    flen.push({ len, file: e });
    glen += len;
}
console.log('   Total lines:', glen);
console.log('  ---- By Lines ----');
let list = flen.sort((a, b) => b.len - a.len);
list.forEach((e,i) => {
        console.log(((i+1) + '.').padEnd(3) + (e).file.slice(2).padStart(50) + ' '.repeat(10) + e.len);
})
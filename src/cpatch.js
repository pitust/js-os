let fs = require('fs');
let pc = `free(gc_main_dicts->data[gc_i]->index->data);free(gc_main_dicts->data[gc_i]->index);free(gc_main_dicts->data[gc_i]->values->data);free(gc_main_dicts->data[gc_i]->values);`.split(';');
let mc = fs.readFileSync('out/main.c').toString();
for (let i = 0; i < 4; i++) {
    mc = mc.replace(new RegExp(pc[i].replace(/([\(\)\[\]])/g, '\\$1') + ';', 'g'), '');
}
mc = mc.replace('#include <string.h>', `int strcmp(char *a, char *b)
{
    int i;
    for (i = 0; a[i] && b[i]; i++)
    {
        if (a[i] < b[i])
            return -1;
        if (a[i] > b[i])
            return 1;
    }
    if (!a[i] && !b[i])
        return 0;
    if (a[i] < b[i])
        return -1;
    if (a[i] > b[i])
        return 1;
}`);
fs.writeFileSync('out/main.c', mc);
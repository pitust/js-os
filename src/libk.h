#include "stdint.h"
#include "stddef.h"
#include "console.h"
#ifndef _LIBK_H_
#define _LIBK_H_ 1
#define printf Console_Format
void free(void *mem);
void trap();
void memconfig(size_t start, size_t len);

extern ptrdiff_t current_usage;

ptrdiff_t current_memory();

ptrdiff_t grow_memory(size_t delta);

void *sbrk(size_t increment);

void dump_memory();
#define memory_assert(cond)                                                \
    if (!(cond))                                                           \
    {                                                                      \
        dump_memory();                                                     \
        printf("Memory error at %s:%d: %s.\n", __FILE__, __LINE__, #cond); \
        trap();                                                            \
    }

typedef enum State
{
    FREE = 0xFEFEFEFE,
    ALLOCATED = 0xC1C1C1C1
} State;
typedef struct MemoryBlock MemoryBlock;
typedef struct MemoryBlock
{
    size_t size;
    MemoryBlock *next;
    State state;
} MemoryBlock;

extern MemoryBlock *start_block;

MemoryBlock *find_available(MemoryBlock **last_block, size_t size);
void dump_memory();
void *malloc(size_t amount);

void *realloc(void *ptr, size_t size);
void _assert_fail(char* e, char* f, int l);
#define assert(expr)							\
    ((expr)								\
     ? (0)						\
     : _assert_fail (#expr, __FILE__, __LINE__))

int strcmp(const char *a, const char *b);

#endif
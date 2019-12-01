#include "stdint.h"
#include "stddef.h"
#include "console.h"
#include "libk.h"
#define printf Console_Format
void free(void *mem);
void trap()
{
    *(uint16_t *)(0xb8000) = 0x6565;
    *(uint16_t *)(0xb8002) = 0x7272;
    *(uint16_t *)(0xb8004) = 0x7272;
    while (1)
        ;
}
void _assert_fail(char *e, char *f, int l)
{
    printf("Asserion failed: %s @ %s:%d", e, f, l);
}
ptrdiff_t current_usage = 0;
size_t memstart = 0x400;
size_t memlen = 0x7c00 - 0x400;
size_t memgiven = 0x100;
void memconfig(size_t start, size_t len){
    memstart = start;
    memlen = len;
}
ptrdiff_t current_memory()
{
    return memgiven;
}

ptrdiff_t grow_memory(size_t delta)
{
    if (memlen > delta + memgiven) {
        memgiven += delta;
        return delta;
    } else {
        printf("[[sbrk]] OUT OF MEMORY!!! (avail %d, inc %d, used %d)", current_usage);
    }
    return 0;
}

void *sbrk(size_t increment)
{
    if (current_usage == 0)
    {
        // First call to sbrk().
        current_usage = current_memory();
    }
    void *return_value = (void *)current_usage;
    ptrdiff_t current_maximum = current_memory();
    ptrdiff_t available = current_maximum - current_usage;
    if (available < increment)
    {
        grow_memory(increment - available + 1);
    }
    current_usage += increment;
    return return_value;
}

void dump_memory();
#define memory_assert(cond)                                                \
    if (!(cond))                                                           \
    {                                                                      \
        dump_memory();                                                     \
        printf("Memory error at %s:%d: %s.\n", __FILE__, __LINE__, #cond); \
        trap();                                                            \
    }


MemoryBlock *start_block = (MemoryBlock *)0;

MemoryBlock *new_block(MemoryBlock *last_block, size_t size)
{
    MemoryBlock *block = (MemoryBlock *)sbrk(sizeof(MemoryBlock) + size);
    if (last_block)
    {
        last_block->next = block;
    }
    block->size = size;
    block->next = 0;
    block->state = ALLOCATED;
    return block;
}

MemoryBlock *find_available(MemoryBlock **last_block, size_t size)
{
    MemoryBlock *current = start_block;
    MemoryBlock *first_free = 0;
    size_t total_free_size = 0;
    while (current && !(current->state == FREE && current->size >= size))
    {
        *last_block = current;

        if (total_free_size == 0 && current->state == FREE)
        {
            total_free_size = current->size;
            first_free = current;
        }
        else if (current->state == FREE)
        {
            total_free_size += sizeof(MemoryBlock) + current->size;
        }
        else
        {
            total_free_size = 0;
        }

        // Can we merge consecutive blocks to fill this allocation?
        if (total_free_size >= size)
        {
            first_free->next = current->next;
            first_free->size = total_free_size;
            current = first_free;
            break;
        }

        current = current->next;
    }

    if (current && current->next && current->size > size + sizeof(MemoryBlock) + 10)
    {
        // This block is significantly larger than what is needed.
        // We can split it and only use the first part of it.
        int nblocks = 1 + 1 + size / sizeof(MemoryBlock);
        MemoryBlock *next = (MemoryBlock *)current + nblocks;

        size_t old_size = current->size;
        next->size = ((current->next - next) - 1) * sizeof(MemoryBlock);
        next->next = current->next;
        next->state = FREE;
        current->next = next;
        current->size = (nblocks - 1) * sizeof(MemoryBlock);

        memory_assert(old_size == current->size + sizeof(MemoryBlock) + next->size);
    }

    return current;
}

void dump_memory()
{
    printf("Memory dump\n");
    size_t allocated_usage = 0;
    size_t free_usage = 0;
    for (MemoryBlock *block = start_block; block; block = block->next)
    {
        memory_assert(block->state == FREE ||
                      block->state == ALLOCATED);
        if (block->state == ALLOCATED)
        {
            allocated_usage += block->size;
        }
        else
        {
            free_usage += block->size;
        }
        printf("  0x%X: size %d. %s.\n",
               (int)block,
               (int)block->size,
               block->state == FREE ? "FREE" : "ALLOCATED");
    }
    printf("  Cur. memory usage: %9d bytes.\n", (int)allocated_usage);
    printf("  Tot. memory usage: %9d bytes.\n", (int)(allocated_usage + free_usage));
    printf("  Peak memory usage: %9d bytes.\n", (int)current_usage);
}

void *malloc(size_t amount)
{
    if (amount == 0)
    {
        return 0;
    }
    if (!start_block)
    {
        start_block = new_block((void *)memstart, amount);
        return start_block + 1;
    }

    MemoryBlock *last = start_block;
    MemoryBlock *block = find_available(&last, amount);
    if (block)
    {
        memory_assert(block->state == FREE);
        block->state = ALLOCATED;
    }
    else
    {
        block = new_block(last, amount);
    }
    return block + 1;
}

void *realloc(void *ptr, size_t size)
{
    if (!ptr)
    {
        return malloc(size);
    }

    MemoryBlock *block = ((MemoryBlock *)ptr) - 1;
    memory_assert(block->state == ALLOCATED);
    if (block->size >= size)
    {
        return ptr;
    }

    if (block->next == 0)
    {
        // This is the last block, so we can just increase it.
        sbrk(size - block->size);
        block->size = size;
        return ptr;
    }
    else
    {
        void *new_ptr = malloc(size);
        memcpy(new_ptr, ptr, block->size);
        free(ptr);
        return new_ptr;
    }
}

void *calloc(size_t num, size_t size)
{
    size_t total = num * size;
    void *mem = malloc(total);
    memset(mem, 0, total);
    return (void *)mem;
}

void free(void *mem)
{
    if (!mem)
    {
        return;
    }
    MemoryBlock *block = ((MemoryBlock *)mem) - 1;
    memory_assert(block->state == ALLOCATED);
    block->state = FREE;
}

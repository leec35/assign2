#include <assert.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>

#include "header.h"
struct block_meta {
    size_t size;
    struct block_meta *next;
    int free;
    int magic;
};

#define META_SIZE sizeof(struct block_meta)

void *global_base = NULL; //always points to first block

void split (struct block_meta *newmem, size_t size){
	struct block_meta *memleft = (void*)((void*)newmem+size+META_SIZE);
	memleft->size = (newmem->size)+size+META_SIZE;
	memleft->free = 1;
	memleft->next = newmem->next;
	newmem-> size = size;
	newmem->free = 0;
	newmem->next = memleft;
}
struct block_meta *find_free_block (struct block_meta **last, size_t size)//finding a free block using the best fit method
{
        struct block_meta *current = global_base;
        struct block_meta *bestblock = NULL;
        while (current)
        {
                if (!bestblock && current->size > size && current->free)//Bestblock becomes the block current is at if it's free and the block's size is larger than input size
                {
                        bestblock = current;
                }
                else if (bestblock && current->size > size && current->size < bestblock->size && current->free)//bestblock becomes current's block when the size of current is less than input size but smaller than bestblock's current size
                {
                        bestblock = current;
                }
                if (bestblock && bestblock->size == size && bestblock->free)//returning bestblock if there is a free block exactly the same size as the input size
                {
                        return bestblock;
                }
                *last = current;
                current = current->next;
        }
        if (bestblock && bestblock->size >= (size+META_SIZE+8) && bestblock->free)//splitting the block if the current size of bestblock is >= to the smallest # of bytes needed for a new block for bestblock
        {
                split (bestblock, size);
        }
        if (bestblock && bestblock->free)
        {
                return bestblock;
        }
        return current;
}
        


struct block_meta *request_space(struct block_meta* last, size_t size)
{
        struct block_meta *block;
        block = sbrk(0);
        void *request = sbrk(size + META_SIZE);
        assert((void*)block == request);
        if (request == (void*)-1)
        {
                return NULL;
        }
        if (last)
        {
                last->next = block;
        }
        block->size = size;
        block->next = NULL;
        block->free = 0;
        block->magic = 0x12345678;
        return block;
}
struct block_meta *get_block_ptr(void *ptr)
{
        return (struct block_meta*)ptr-1;
}
void free(void *ptr)
{
        if (!ptr)
        {
                return;
        }
        struct block_meta* block_ptr = get_block_ptr(ptr);
        assert(block_ptr->free == 0);
        block_ptr->free = 1;
        struct block_meta *current = global_base;
        struct block_meta *delete = current;
        while (current && current->next)//merging fragmented free blocks into one large free block
        {
                if (current->free && current->next->free)
                        {
                                current->size = META_SIZE + current->size + current-> next->size;//combining the sizes of the current and next block
                                current->next = current->next->next;//current's next block is updated to the next of the next block
                                delete = current->next;
                                free (delete->next);
                                delete = current;
                        }
                current = current->next;
                delete = current;
                }
	}


#define padding(x) (((x/8)+1)*8)//padding for blocks to be used for any built-in type
void *malloc (size_t size)
{
        struct block_meta *block;
        size = padding(size);//calculate the size with padded bytes to be used to store any types
        if (size <= 0)
        {
                return NULL;
        }
        if (!global_base)
        {
                block = request_space(NULL, size);
                if (!block)
                {
                        return NULL;
                }
                global_base = block;
        }
        else
        {
                struct block_meta *last = global_base;
                block = find_free_block(&last,size);
                if (!block)
                {
                        block = request_space(last, size);
                        if (!block)
                        {
                                return NULL;
                        }
                }
                else
                {
                        block -> free = 0;
                        block -> magic = 0x77777777;
                }
        }
        return (block+1);
}

void *realloc(void *ptr, size_t size)
{
        if(!ptr)//reallc acts like malloc now bc pointer is NULL
        {
                return malloc(size);
        }
        struct block_meta* block_ptr = get_block_ptr(ptr);
        if(block_ptr->size >= size)
        {
                return ptr;
        }
        void *new_ptr;
        new_ptr = malloc(size);
        if (!new_ptr)
        {
                return NULL;
        }
                memcpy(new_ptr, ptr, block_ptr->size);
                free(ptr);
                return new_ptr;
}
void *calloc(size_t nelem, size_t elsize)
{
        size_t size = nelem * elsize;
        void *ptr = malloc(size);
        memset(ptr, 0, size);
        return ptr;
}

int MemoryLeaks()
{
        struct block_meta *nfree = global_base;
        struct block_meta *bcount = global_base;
        int numfree;
        int blocks;
        while (nfree)
        {
                if (nfree->free)
                {
                        numfree = numfree + nfree->size;
                }
                nfree = nfree->next;
        }
        while (bcount)
        {
                blocks++;
                bcount = bcount->next;
        }
	int sum = blocks+numfree;
return sum;
}


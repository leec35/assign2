#ifndef CMSC257_A2SUPPORT_INCLUDED
#define CMSC257_A2SUPPORT_INCLUDED

void *malloc(size_t size);
void free (void *ptr);
void *realloc (void *ptr, size_t size);
void *calloc (size_t nelem, size_t elsize);
int MemoryLeaks();

#endif


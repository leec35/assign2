#include <stdio.h>
#include <stdlib.h>

#include "header.h"
#include "functions.c"
int i=10;
int j=5;
void *mal;
void *rea;
void *cal;
int main(int argc, char *argv[])
{
	while (i > 0)
	{
        	//int count = 1;
		printf("Hello.\n");
        	int m = rand() % 10;
		int r = rand() % 10;
     		int c = rand() % 10;
		int cc = rand() % 10;
       		mal = malloc(m);
       		rea = realloc(mal, r);
       		cal = calloc(c, cc);
   		printf("Malloc'd: %d", m);
     		printf("Realloc'd: %d", r);
       		printf("Calloc'd: %d and %d", c, cc);
		i--;
	}
	while (j>0)
	{	
       		int f = rand() % 10;
       		free(mal);
       		printf("Freed %d", f);
	}
	printf("Memory Leakage: %d", MemoryLeaks());
	return 0;
}

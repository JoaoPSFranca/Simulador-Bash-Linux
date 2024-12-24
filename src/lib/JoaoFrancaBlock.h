#ifndef BLOCK_H
#define BLOCK_H

#include <stdio.h>
#include <stdlib.h>

#define BLOCK_SIZE 16
#define MAX_I_NODE 32
#define MAX_BLOCKS (MAX_I_NODE*15)

typedef struct sBlock {
    char address[70];
    int status; // 0 free | 1 busy 
    char data[BLOCK_SIZE];
} Block;

typedef struct sFreeBlock {
    Block *block;
    struct sFreeBlock *next;
} FreeBlock;

#endif

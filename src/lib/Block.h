#ifndef BLOCK_H
#define BLOCK_H

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#define BLOCK_SIZE 16
#define MAX_I_NODE 32
#define MAX_BLOCKS (MAX_I_NODE*15)

typedef struct sBlock {
    uint32_t block_number;
    char data[BLOCK_SIZE]; 
} Block;

#endif

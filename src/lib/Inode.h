#ifndef INODE_H
#define INODE_H

#include "Block.h"
#define MAX_FILENAME 256

typedef struct sINode {
    uint32_t inode_number;
    uint32_t size;
    uint32_t block_count;
    uint32_t block_id[15];
} INode;

#endif // INODE_H

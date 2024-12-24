#ifndef FILESYSTEM_H
#define FILESYSTEM_H

#include "Directory.h"

typedef struct sFileSystem {
    uint32_t total_inodes;
    uint32_t total_blocks;
    uint32_t free_inodes;
    uint32_t free_blocks;
    uint8_t *inode_bitmap;
    uint8_t *block_bitmap;
    INode inodes[MAX_I_NODE];
    Block blocks[MAX_BLOCKS];
} FileSystem;

#endif
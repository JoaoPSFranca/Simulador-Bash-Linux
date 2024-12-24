#ifndef INODE_H
#define INODE_H

#include "JoaoFrancaBlock.h"
#define MAX_FILENAME 256

typedef struct sINode {
    int id;
    char type; // 'r' regular file | 'd' directory file
    int status; // 0 free | 1 busy
    char name[MAX_FILENAME];
    long int size;
    int block_count;
    Block blocks[15];
} INode;

typedef struct sFreeINode {
    INode *inode; 
    struct sFreeINode *next; 
} FreeINode;

typedef struct sINodeList {
    INode *inode; 
    struct sINodeList *next; 
} INodeList;

#endif // INODE_H

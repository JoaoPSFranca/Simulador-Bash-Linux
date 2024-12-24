#ifndef DIRECTORY_H
#define DIRECTORY_H

#include "JoaoFrancaInode.h"

struct sDirectory;

typedef struct sDirectoryList {
    struct sDirectory *directory;
    struct sDirectoryList *next;
} DirectoryList;

typedef struct sDirectory {
    INode *inode;
    INodeList *iNodeList;
    char name[MAX_FILENAME]; 
    int childs_cont;
    struct sDirectory *parent; // ..
    struct sDirectoryList *childs; // sub directories
} Directory;

#endif 

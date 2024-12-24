#ifndef DIRECTORY_H
#define DIRECTORY_H

#include "Inode.h"

typedef struct sDirectoryEntry {
    uint32_t inode;
    uint16_t entry_size;
    char file_type;             // r: file, d: directory || can be uint8_t to accept more types
    char name[MAX_FILENAME];
} DirectoryEntry;

typedef struct sDirectory {
    INode *inode;
    DirectoryEntry entries[];
} Directory;

#endif 

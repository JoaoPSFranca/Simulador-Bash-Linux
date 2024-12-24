#include <string.h>

#include "../lib/FileSystem.h"

int allocate_inode(FileSystem *fs) {
    for (uint32_t i = 0; i < fs->total_inodes; i++) {
        if (!(fs->inode_bitmap[i / 8] & (1 << (i % 8)))) {              // Verifica se o inode está livre
            fs->inode_bitmap[i / 8] |= (1 << (i % 8));                  // Marca o inode como ocupado
            return i;                                                   // Retorna o número do inode alocado
        }
    }

    return -1;                                                          // Nenhum inode livre encontrado
}

void free_inode(FileSystem *fs, uint32_t inode_number) {
    fs->inode_bitmap[inode_number / 8] &= ~(1 << (inode_number % 8));   // Marca o inode como livre
}


int allocate_block(FileSystem *fs) {
    for (uint32_t i = 0; i < fs->total_blocks; i++) {
        if (!(fs->block_bitmap[i / 8] & (1 << (i % 8)))) {              // Verifica se o bloco está livre
            fs->block_bitmap[i / 8] |= (1 << (i % 8));                  // Marca o bloco como ocupado
            return i;                                                   // Retorna o número do bloco alocado
        }
    }

    return -1;                                                          // Nenhum bloco livre encontrado
}

void free_block(FileSystem *fs, uint32_t block_number) {
    fs->block_bitmap[block_number / 8] &= ~(1 << (block_number % 8));   // Marca o bloco como livre
}

void free_file_system(FileSystem *fs) {
    free(fs->inode_bitmap);
    free(fs->block_bitmap);
}
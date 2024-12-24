#include "../lib/Block.h"

void generateBlocks(FreeBlock **freeBlocks){
    *freeBlocks = malloc(sizeof(FreeBlock));
    FreeBlock *aux = *freeBlocks;

    for (unsigned int i = 0; i < MAX_BLOCKS; i++) {
        FILE *archive;
        char filename[70];

        snprintf(filename, sizeof(filename), "src/Blocks/%02d.dat", i + 1);

        archive = fopen(filename, "wb");

        if (archive != NULL) {
            Block *block = malloc(sizeof(Block));
            strcpy(block->address, filename);
            block->status = 0; // free block

            fwrite(block, sizeof(Block), 1, archive);
            fclose(archive);
            aux->block = block;

            if (i < MAX_BLOCKS - 1) {
                aux->next = malloc(sizeof(FreeBlock));
                aux = aux->next;
            } else
                aux->next = NULL;
            
        } else
            printf("Error opening file. \n");
    }
}

Block *verifyBlockFree(FreeBlock **freeBlocks){
    FreeBlock *aux = *freeBlocks;
    int flag = 1;

    while (flag && aux != NULL){
        if (aux->block->status == 0)
            flag = 0;
        else
            aux = aux->next;
    }
    
    if (aux == NULL)
        return NULL;

    return aux->block;
}

void alterBlockDat(Block *block){
    FILE *archive;
    char filename[70];
    
    strcpy(filename, block->address);
    archive = fopen(filename, "wb");

    if (archive != NULL) {
        fwrite(block, sizeof(Block), 1, archive);
        fclose(archive);
    } else
        printf("Error opening file. %s\n", filename);
}

void enterFreeBlock(FreeBlock **freeBlock, Block *block){
    block->status = 0;

    FreeBlock *aux = *freeBlock;

    while (aux != NULL)
        aux = aux->next;
    
    FreeBlock *newFreeBlock = malloc(sizeof(FreeBlock));

    newFreeBlock->block = block;
    newFreeBlock->next = NULL;
    aux = newFreeBlock;

    alterBlockDat(block);
}

void removeBlockFree(FreeBlock **freeBlocks, Block *block){
    if (strcmp((*freeBlocks)->block->address, block->address) == 0) {
        (*freeBlocks) = (*freeBlocks)->next;
    } else {
        FreeBlock *aux = *freeBlocks;
        FreeBlock *aux2 = NULL;

        while (strcmp(aux->block->address, block->address) == 0 && aux != NULL) {
            aux2 = aux;
            aux = aux->next; 
        }

        if (aux != NULL){
            if (aux2 == NULL)
                freeBlocks = &aux;    
            else
                aux2->next = aux->next;

            aux->block->status = 1;
            alterBlockDat(block);
        }
        else
            printf("Block not found. \n");
    }
}

void readBlockDat(FreeBlock **fb){
    *fb = NULL;
    FreeBlock *aux = NULL;

    for (unsigned int i = 0; i < MAX_BLOCKS; i++) {
        FILE *archive;
        char filename[70];

        snprintf(filename, sizeof(filename), "src/Blocks/%02d.dat", i + 1);

        archive = fopen(filename, "rb");

        if (archive != NULL) {
            Block *block = malloc(sizeof(Block));
            fread(block, sizeof(Block), 1, archive);
            
            if(block->status == 0){
                FreeBlock *newNode = malloc(sizeof(FreeBlock));
                newNode->block = block;
                newNode->next = NULL;

                if (*fb == NULL)
                    *fb = newNode;    
                else
                    aux->next = newNode;
                
                aux = newNode;
            } else
                free(block);

            fclose(archive);
        } else {
            archive = fopen(filename, "wb");

            if (archive != NULL) {
                Block *block = malloc(sizeof(Block));
                FreeBlock *newNode = malloc(sizeof(FreeBlock));
                
                strcpy(block->address, filename);
                block->status = 0; // free block

                fwrite(block, sizeof(Block), 1, archive);

                newNode->block = block;
                newNode->next = NULL;

                if (*fb == NULL)
                    *fb = newNode;    
                else
                    aux->next = newNode;
                
                aux = newNode;
                
                fclose(archive);
            } else {
                printf("Error opening file. %s\n", filename);
            }
        }
    }
}

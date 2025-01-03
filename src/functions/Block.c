#include "../lib/FileSystem.h"

void generateBlocks(FileSystem **fs){
    for (unsigned int i = 0; i < MAX_BLOCKS; i++) {
        FILE *file;
        char filename[70];

        snprintf(filename, sizeof(filename), "src/Blocks/%02d.dat", i);

        file = fopen(filename, "w");

        if (file != NULL) {
            Block *block = malloc(sizeof(Block));
            block->block_number = i;

            fclose(file);

            (*fs)->blocks[i] = *block;
            
        } else
            printf("Error opening file. \n");
    }
}

void writeBlockDat(Block *block) {
    FILE *file;
    char filename[70];

    snprintf(filename, sizeof(filename), "src/Blocks/%02d.dat", block->block_number);

    file = fopen(filename, "w");

    if (file != NULL) {
        fprintf(file, block->data);
        fclose(file);
    } else
        printf("Error opening file. %s\n", filename);
}

void writeBlockDatDir(Block *block, DirectoryEntry *entry) {
    FILE *file;
    char filename[70];

    snprintf(filename, sizeof(filename), "src/Blocks/%02d.dat", block->block_number);

    file = fopen(filename, "ab+");

    if (file != NULL) {
        fwrite(entry, sizeof(DirectoryEntry), 1, file);
        fclose(file);
    } else
        printf("Error opening file. %s\n", filename);
}

int countBlockItens(Block *block) {
    FILE *file;
    DirectoryEntry *aux;
    char filename[70];
    int count = 0;

    snprintf(filename, sizeof(filename), "src/Blocks/%02d.dat", block->block_number);

    file = fopen(filename, "rb");

    while(!fread(aux, sizeof(DirectoryEntry), 1, file))
        count++;

    return count;
}

Block *getBlock(FileSystem **fs, int block_number) {
    if (block_number == -1) 
        block_number = allocate_block(*fs);
        
    return &((*fs)->blocks[block_number]);
}


#include "../lib/Block.h"

void generateBlocks(FileSystem **fs){
    for (unsigned int i = 0; i < MAX_BLOCKS; i++) {
        FILE *file;
        char filename[70];

        snprintf(filename, sizeof(filename), "src/Blocks/%02d.dat", i + 1);

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

void writeBlockDat(Block *block){
    FILE *file;
    char filename[70];

    snprintf(filename, sizeof(filename), "src/Blocks/%02d.dat", block->block_number + 1);

    file = fopen(filename, "w");

    if (file != NULL) {
        fprintf(file, block->data);
        fclose(file);
    } else
        printf("Error opening file. %s\n", filename);
}


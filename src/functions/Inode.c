#include <conio.h>
#include "../lib/Inode.h"
#include "../lib/Directory.h"

void generateInodes(FileSystem **fs) {
    FILE *file;
    
    file = fopen("src/Resources/INodes.dat", "wb");
    
    if(file != NULL){
        for (unsigned int i = 0; i < MAX_I_NODE; i++){
            INode *inode = malloc(sizeof(INode));

            inode->inode_number = i;
            inode->block_count = 0;
            inode->size = 0;

            fwrite(inode, sizeof(INode), 1, file);

            (*fs)->inodes[i] = *inode;
        }

        fclose(file);
    } else
        printf("Error opening INode file. \n");
}

void writeINodeDat(INode *inode, int type) {
    FILE *file;
    
    if (type == 1) 
        file = fopen("src/Resources/INodes.dat", "ab+");
    else
        file = fopen("src/Resources/INodes.dat", "rb+");

    if (file != NULL) {
        INode aux;
        fwrite(inode, sizeof(INode), 1, file);
        fclose(file);
    } else
        printf("Error opening INode file. \n");
}


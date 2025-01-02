#include <sys/stat.h>

#include "../lib/Directory.h"

int verifyDirectory(const char name[]) {
    struct stat st;

    if (stat(name, &st) == 0) {
        return S_ISDIR(st.st_mode);
    }

    return 0;
}

int verifyDirFS(DirectoryEntry entries[], const char name[], int total) {
    int flag = 0;

    for (unsigned int i = 0; i < total; i++) {
        if(!strcmp(entries[i].name, name)){
            i = total;
            flag = 1;
        }
    }

    return flag;
}

int createDirectoryPhysic(const char name[]) {
    if (!verifyDirectory(name))
        if (!mkdir(name, 755))
            return 1;

    return 0;
}

DirectoryEntry *createDirectoryEntry(INode *inode, const char name[], char type) {
    DirectoryEntry *dirEntry = malloc(sizeof(DirectoryEntry));
    dirEntry->file_type = type;
    dirEntry->inode = inode->inode_number;
    strcpy(dirEntry->name, name);
    dirEntry->entry_size = inode->size;

    return dirEntry;
}

void addDirectory(FileSystem *fs, INode *inode, const char name[]) {
    
    dir->entries[0] = *(createDirectoryEntry(inode, ".", 'd'));
    
    if(parent != NULL && parent->inode->inode_number != inode->inode_number){
        dir->entries[1] = *(createDirectoryEntry(parent->inode, "..", 'd'));

        parent->entries[parent->entry_count] = *(createDirectoryEntry(inode, name, 'd'));
        parent->entry_count = parent->entry_count + 1;

        return dir;
    } else {
        dir->entries[1] = *(createDirectoryEntry(inode, "..", 'd'));
    }

    return dir;
}

Directory *generateRoot(FileSystem **fs) {
    int pos = allocate_inode(*fs);
    INode *inode = &((*fs)->inodes[pos]);

    pos = allocate_block(*fs);
    Block *block = &((*fs)->blocks[pos]);

    inode->block_count = 1;
    inode->block_id[0] = block->block_number;
    inode->size = BLOCK_SIZE;

    return addDirectory(inode, NULL, "C");;
}

void function_mkdir(FileSystem **fs, Directory *parent, char argument[]) {
    int pos = allocate_inode(*fs);
    INode *inode = &((*fs)->inodes[pos]);
    
    if (inode == NULL)
        printf("No Free INode avaible. \n");
    else {
        pos = allocate_block(*fs);
        Block *block = &((*fs)->blocks[pos]);

        if (block == NULL) {
            printf("No Free Blocks avaible. \n");
        } else {
            int flag;

            if (parent == NULL || parent->entry_count == 2)
                flag = 0;
            else 
                flag = verifyDirFS(parent->entry_count, argument, parent->entry_count);

            if (flag) {
                printf("Error: Could not create the directory '%s'. \n\n", argument);
                free_block(*fs, block->block_number);
                free_inode(*fs, inode->inode_number);
            } else {
                inode->block_count = 1;
                inode->block_id[0] = block->block_number;
                inode->size = BLOCK_SIZE;

                Directory *dir = addDirectory(inode, parent, argument);
                parent->inode->size += inode->size;
            }
        } 
    }
}

void function_ls(INode all[2][MAX_I_NODE], Directory *dir, FileSystem *fs, int *countDir, int *countReg){
    *countDir = 0;
    *countReg = 0;

    for (unsigned int i = 0; i < dir->entry_count; i++) {
        DirectoryEntry entry = dir->entries[i];
        
        if (entry.file_type == 'd'){
            all[0][*countDir] = fs->inodes[entry.inode];
            (*countDir)++;
        } else if (entry.entry_size == 'r') {
            all[1][*countReg] = fs->inodes[entry.inode];
            (*countReg)++;
        }
    } 
}

void function_rmdir(FileSystem *fs, Directory *dir, char dirName[]){
    int flag = 1;
    
    for (unsigned int i = 0; i < dir->entry_count; i++) {
        if (dir->entries[i].file_type == 'd'  && strcmp(dir->entries[i].name, dirName) == 0) {
            i = dir->entry_count;
            flag = 0;
        }
    }

    if(flag == 1)
        printf("Erro: directory '%s' not found.\n\n", dirName);
    else{
        if (dirToRemove->iNodeList->next != NULL)
            printf("Erro: directory '%s' is not empty. %s\n\n", dirName, dirToRemove->iNodeList->inode->name);
        else{
            if(previous == NULL)
                currentDirectory->childs = toRemove->next;
            else 
                previous->next = toRemove->next;
            
            INode *aux = dirToRemove->inode;
            int i;

            for (i = 0; i < aux->block_count; i++)
                enterFreeBlock(freeBlock, &(aux->blocks[i]));
            
            enterINodeFree(freeINode, aux);

            Directory *dirParent = dirToRemove->parent;

            INodeList *aux2 = dirParent->iNodeList;
            INodeList *aux2Prev = NULL;

            while (aux2->inode->id != aux->id){
                aux2Prev = aux2;
                aux2 = aux2->next;
            }
                
            aux2Prev->next = aux2->next;
            
            free(aux2);            
            free(dirToRemove);
            free(toRemove);
            // alterDirDat(dirParent);
            // remove_dir(dirName, path, *freeBlock);
        }
    }
}

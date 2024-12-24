#include <sys/stat.h>

#include "../lib/Directory.h"

int verifyDirectory(const char name[]) {
    struct stat st;

    if (stat(name, &st) == 0) {
        return S_ISDIR(st.st_mode);
    }

    return 0;
}

int verifyDirFS(INodeList *list, const char name[]){
    if (list != NULL) {
        INodeList *aux = list;
        int flag = 1;

        while (flag && aux != NULL){
            if (!strcmp(aux->inode->name, name))
                flag = 0;
            else
                aux = aux->next;
        }

        return aux != NULL;
    } else
        return 0;
}

void alterDirDat(Directory *dir){
    FILE *archive;
    archive = fopen("src/Resources/Directory.dat", "rb+");

    printf("Teste 1\n");
    if (archive != NULL) {
        printf("Teste 2\n");
        int flag = 1;
        int resp = 1;

        printf("Teste 3\n");
        while (flag && resp) {
            Directory *aux = malloc(sizeof(Directory));
            resp = fread(aux, sizeof(Directory), 1, archive);

            if (aux->inode->id == dir->inode->id) {
                fseek(archive, -sizeof(Directory), SEEK_CUR);
                fwrite(dir, sizeof(Directory), 1, archive);
                flag = 0;   
            }
        }

        printf("Teste 4\n");
        if (flag == 1 || resp != 1) {
            printf("Error: Directory not found. ");
        }
        
        printf("Teste 5\n");
        fclose(archive);
    } else
        printf("Error opening Directory file. \n");
}

int createDirectory(const char name[]){
    if (!verifyDirectory(name))
        if (!mkdir(name, 755))
            return 1;

    return 0;
}

void remove_dir(const char dirName[], const char path[]) {
    char completePath[MAX_FILENAME];

    if (path[0] != '\0')
        snprintf(completePath, sizeof(completePath), "rmdir c\\%s\\%s", path, dirName);
    else
        snprintf(completePath, sizeof(completePath), "rmdir c\\%s", dirName);

    if (system(completePath)) {
        printf("Erro ao executar o comando");
    } else {
        printf("Diretorio removido com sucesso: %s\n", completePath);
    }
}

Directory *addDirectory(INode *inode, Directory *parent){
    Directory *dir = malloc(sizeof(Directory));
    INodeList *inodeList = malloc(sizeof(INodeList));

    inodeList->inode = inode;
    inodeList->next = NULL;
    
    dir->inode = inode;
    dir->childs_cont = 0;
    dir->iNodeList = inodeList;
    strcpy(dir->name, inode->name);
    dir->parent = parent;
    dir->childs = NULL;

    if(parent != NULL && parent->inode->id != inode->id){
        DirectoryList *dirList = malloc(sizeof(DirectoryList));

        dirList->directory = dir;
        dirList->next = NULL;
        parent->inode->blocks[(parent->inode->block_count)++] = inode->blocks[0];

        if(parent->childs != NULL){
            DirectoryList *aux = parent->childs;

            while (aux->next != NULL)
                aux = aux->next;

            aux->next = dirList;
        } else
            parent->childs = dirList;
        
        INodeList *aux2 = parent->iNodeList;

        while (aux2->next != NULL)
            aux2 = aux2->next;

        INodeList *newNode = malloc(sizeof(INodeList));
        newNode->inode = inode;
        newNode->next = NULL;

        aux2->next = newNode;
        (parent->childs_cont)++;

        // alterDirDat(dir->parent);
        return dir;
    }

    return dir;
}

void appendDir(Directory *dir){
    FILE *archive;

    archive = fopen("src/Resources/Directory.dat", "ab+");

    if (archive != NULL) {
        fwrite(dir, sizeof(Directory), 1, archive);

        fclose(archive);
    } else 
        printf("Error opening Directory file. \n");
    
}

int readDirectoryDat(Directory **root){
    FILE *archive;
    archive = fopen("src/Resources/Directory.dat", "rb");

    if (archive != NULL) {
        int resp = 1;

        while(resp){
            Directory *dir = malloc(sizeof(Directory));
            resp = fread(dir, sizeof(Directory), 1, archive);

            if (resp) {
                printf("Nome Dir: %s\n", dir->name);
                if (!strcmp(dir->name, "c"))
                    *root = dir;
                else
                    addDirectory(dir->inode, dir->parent);
            } else
                free(dir);
        }

        return 1;   
    } else {
        printf("Error opening Directory file. \n");
        return 0;
    }
}

Directory *generateRoot(FreeINode **freeInodes, FreeBlock **freeBlocks){
    INode *inode = verifyINodeFree(freeInodes, 'd');
    Block *block = verifyBlockFree(freeBlocks);
    INodeList *inodeList = malloc(sizeof(INodeList));

    inodeList->inode = inode;
    inodeList->next = NULL;

    removeINodeFree(freeInodes, inode);
    removeBlockFree(freeBlocks, block);

    inode->block_count = 1;
    inode->blocks[0] = *block;
    strcpy(inode->name, "c");
    inode->size = 0;

    Directory *root = malloc(sizeof(Directory));
    root->inode = inode;
    root->iNodeList = inodeList;

    strcpy(root->name, "c");
    root->parent = root;
    root->childs = NULL;

    appendDir(root);

    return root;
}

void function_mkdir(char argument[], FreeINode **freeInodes, FreeBlock **freeBlocks, Directory *parent) {    
    INode *inode = verifyINodeFree(freeInodes, 'd');
    
    if (inode == NULL)
        printf("No Free INode avaible. \n");
    else {
        Block *block = verifyBlockFree(freeBlocks);

        if (block == NULL) {
            printf("No Free Blocks avaible. \n");
        } else {
            int flag;

            if (parent == NULL || parent->iNodeList->next == NULL)
                flag = 0;
            else 
                flag = verifyDirFS(parent->iNodeList->next, argument);

            if (flag) {
                printf("Error: Could not create the directory '%s'. \n\n", argument);
            } else {
                inode->block_count = 1;
                inode->blocks[0] = *block;
                strcpy(inode->name, argument);
                inode->size = 0;

                Directory *dir = addDirectory(inode, parent);
                appendDir(dir);
                removeINodeFree(freeInodes, inode);
                removeBlockFree(freeBlocks, block);
            }
        } 
    }
}

void function_ls(char all[][2][MAX_FILENAME], Directory *currentDirectory, int *countDir, int *countReg){
    INodeList *aux = currentDirectory->iNodeList->next;

    strcpy(all[0][0], ".");
    strcpy(all[1][0], "..");
    *countDir = 2;
    *countReg = 0;

    while(aux != NULL){
        if (aux->inode->type == 'r'){
            strcpy(all[*countReg][1], aux->inode->name); 
            (*countReg)++;
        } else {
            strcpy(all[*countDir][0], aux->inode->name); 
            (*countDir)++;
        }

        aux = aux->next;
    }
}

void function_rmdir(char dirName[], Directory *currentDirectory, FreeBlock **freeBlock, FreeINode **freeINode){
    DirectoryList *previous = NULL;
    DirectoryList *toRemove = currentDirectory->childs;

    while (toRemove != NULL && strcmp(toRemove->directory->name, dirName)) {
        previous = toRemove;
        toRemove = toRemove->next;
    }

    if(toRemove == NULL)
        printf("Erro: directory '%s' not found.\n\n", dirName);
    else{
        Directory *dirToRemove = toRemove->directory;
        
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

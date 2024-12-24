#include <ctype.h>

#include "functions/bitmap.c"
#include "functions/Block.c"
#include "functions/Inode.c"
#include "functions/Directory.c"
#include "functions/File.c"

void init_file_system(FileSystem *fs) {
    fs->total_inodes = MAX_I_NODE;
    fs->total_blocks = MAX_BLOCKS;

    // init bitmap inodes
    fs->inode_bitmap = (uint8_t *)calloc((MAX_I_NODE + 7) / 8, sizeof(uint8_t));
    // memset(fs->inode_bitmap, 0, (MAX_I_NODE + 7) / 8);

    // init bitmap blocks
    fs->block_bitmap = (uint8_t *)calloc((MAX_BLOCKS + 7) / 8, sizeof(uint8_t));
    // memset(fs->block_bitmap, 0, (MAX_BLOCKS + 7) / 8);

    printf("Bash 0.5 [versao 4.0.0]\n\n");
}

void initFileSystem(FreeBlock **freeBlocks, FreeINode **freeInodes, Directory **root, INode **runSH) {
    *root = NULL;
    // int flag = 0;

    if(!verifyDirectory("src/Blocks") && !verifyDirectory("src/Resources")) {
        // flag = 1;
        // flag = readINodeDat(freeInodes);
    //} else {
        createDirectory("src/Blocks");
        createDirectory("src/Resources");    
    }

    // if (!flag) {
        generateBlocks(freeBlocks);
        generateInodes(freeInodes);
    // } 
    // else {
    //     // readBlockDat(freeBlocks);
    //     // readDirectoryDat(root);
    // }
    
    // if (*root == NULL) {
        *root = generateRoot(freeInodes, freeBlocks);
        *runSH = generate_script_sh(freeBlocks, freeInodes);
    // }

}

void format(char comand[]){
    for (unsigned int i = 0; i < strlen(comand); i++)
        comand[i] = tolower(comand[i]);
}

void format_path(char path[]){
    unsigned int i = strlen(path) - 1;
    
    while (path[i] != '/' && i > 0){
        path[i] = '\0';
        i--;
    }
    
    path[i] = '\0';
}

void print_ls(char all[][2][MAX_FILENAME], int countDir, int countReg){
    int i;
    
    for (i = 0; i < countDir; i++)
        printf("%s\t<dir>\n", all[i][0]);
    
    for (i = 0; i < countReg; i++)
        printf("%s\n", all[i][1]);
}

int validateArgument(char argument[], char comand[]){
    if(!strcmp(argument, "")) {
        if (!strcmp(comand, "cls") || !strcmp(comand, "clear") ||
            !strcmp(comand, "ls") || !strcmp(comand, "exit") || 
            !strcmp(comand, "run")) {
            return 1;
        } else
            return 0;
    } else 
        return 1;
}

int validateDirName(char argument[]){
    if (strstr(argument, "/") != NULL || strstr(argument, "\\") != NULL)
        return 1;
    else
        return 0;
}

int validateFileName(char fileName[]) {
    char *dot = strrchr(fileName, '.');
    
    if (dot && dot != fileName && *(dot + 1) != '\0')
        return 1;
    
    return 0;
}

int verifyArgumentCat(char argument[]){
    unsigned int i;
    int flag = 0;

    for (i = 0; i < strlen(argument); i++)
        if (argument[i] == '>')
            flag = 1;

    return flag;
}

int validateSHFile(char argument[]){
    if (strstr(argument, ".sh") != NULL)
        return 1;
    else
        return 0;
}

void functions(char argument[], char comand[], char path[], 
                FreeINode **freeInodes, FreeBlock **freeBlocks, 
                Directory **currentDirectory, Directory **root, INode **runSH) {
    char fileName[MAX_FILENAME];

    if (!validateArgument(argument, comand))
        printf("Command missing argument. \n\n");
    else {
        if (!strcmp(comand, "mkdir")) {
            if (validateDirName(argument))
                printf("Invalid charcter '/' or '\\'. \n\n");
            else
                function_mkdir(argument, freeInodes, freeBlocks, *currentDirectory);
        } else if (!strcmp(comand, "cd")) {
            if (!strcmp(argument, "..")){
                if (path[0] != '\0'){
                    *currentDirectory = (*currentDirectory)->parent;
                    format_path(path);
                } else
                    printf("Error: You are already in the root directory.\n\n");
            } else if (!strcmp(argument, "/")){
                path[0] = '\0';
                *currentDirectory = *root;
            } else if (verifyDirFS((*currentDirectory)->iNodeList->next, argument)){
                if (path[0] != '\0'){
                    path[strlen(path)] = '/';

                    while (path[strlen(path)-1] != '/')
                        path[strlen(path)-1] = '\0';
                }

                DirectoryList *aux = (*currentDirectory)->childs;
                while (strcmp(aux->directory->name, argument) && aux != NULL)
                    aux = aux->next;

                if (aux == NULL || aux->directory == *root)
                    printf("Directory exists, but not found in current file system. \n\n");
                else {
                    *currentDirectory = aux->directory;
                    strcat(path, argument);
                }
            } else if (strcmp(argument, "."))
                printf("The system could not find the specified path. \n\n");
        } else if (!strcmp(comand, "ls")) {
            char all[MAX_I_NODE][2][FILENAME_MAX];
            int countReg, countDir;
            function_ls(all, *currentDirectory, &countDir, &countReg);
            print_ls(all, countDir, countReg);
        } else if (!strcmp(comand, "cat")) {
            if (verifyArgumentCat(argument)) { // create file
                sscanf(argument, "> %s", fileName);
                
                if (validateDirName(fileName))
                    printf("Invalid charcter '/' or '\\'. \n\n");
                else if(!validateFileName(fileName))
                    printf("Missing file extension. \n\n");
                else
                    function_cat_create(fileName, *currentDirectory, freeInodes, freeBlocks);

                // alterDirDat(*currentDirectory);
            } else { // open file
                sscanf(argument, "%s", fileName);

                INode *inode = searchFile(&((*currentDirectory)->iNodeList), fileName);

                if (inode == NULL)
                    printf("File not found. \n\n");
                else
                    function_cat_show(inode);
            }
        } else if (!strcmp(comand, "rm")) {
            sscanf(argument, "%s", fileName);

            INode *inode = searchFile(&((*currentDirectory)->iNodeList), fileName);

            if (inode == NULL)
                printf("File not found. \n\n");
            else
                function_rm(inode, *currentDirectory, freeBlocks, freeInodes);
        } else if (!strcmp(comand, "rmdir")) {
            function_rmdir(argument, *currentDirectory, freeBlocks, freeInodes);
        } else if (!strcmp(comand, "mv")) {
            char newName[MAX_FILENAME];
            sscanf(argument, "%s %s", fileName, newName);

            if (validateDirName(newName))
                printf("Invalid charcter '/' or '\\'. \n\n");
            else if(!validateFileName(newName))
                printf("Missing file extension. \n\n");
            else {
                INode *inode = searchFile(&((*currentDirectory)->iNodeList), fileName);

                if (inode == NULL)
                    printf("File not found. \n\n");
                else
                    function_mv(inode, newName, currentDirectory);
            }
        } else if (!strcmp(comand, "run")) {
            char runComand[15 * BLOCK_SIZE + 1] = "";
            char aux[15 * BLOCK_SIZE + 1];

            if (!strcmp(argument, "")) {
                function_run(*runSH, runComand);
            } else if (validateSHFile(argument)) {
                INode *inode = searchFile(&((*currentDirectory)->iNodeList), argument);

                if (inode == NULL)
                    printf("File not found. \n\n");
                else 
                    function_run(inode, runComand);
            } else {
                printf("Invalid file extension. \n\n");
                strcpy(runComand, "");
            }
            
            if (strcmp(runComand, "")){
                int flag = 1;
                char
                    comandAux[10] = "", 
                    argumentAux[MAX_FILENAME + 7];

                while (flag) {                    
                    sscanf(runComand, "%[^\n] %[^\0]", aux, runComand);
                    sscanf(aux, "%s %[^\n]", comandAux, argumentAux);

                    comandAux[strcspn(comandAux, "\n")] = '\0'; 

                    format(comandAux);

                    printf("c:/%s>%s \n", path, aux);
                    functions(argumentAux, comandAux, path, freeInodes, freeBlocks, currentDirectory, root, runSH);

                    strcpy(argumentAux, "");

                    if (!strcmp(runComand, aux) || !strcmp(runComand, "") || !strcmp(runComand, "\n"))
                        flag = 0;
                }
            }
        } else if (!strcmp(comand, "cls") || !strcmp(comand, "clear")){
            system("cls");
        } else if (strcmp(comand, "exit")) {
            printf("%s comand not recognized. \n\n", comand);
        } 
    }
}

void bash(FreeBlock **freeBlocks, FreeINode **freeInodes, 
        Directory **root, INode **runSH){
    char 
        comand[10] = "", 
        argument[MAX_FILENAME + 7], 
        entry[MAX_FILENAME * 2], 
        path[100];

    Directory *currentDirectory = *root;
    
    path[0] = '\0';

    while (strcmp(comand, "exit")) {
        printf("c:/%s>", path);

        fgets(entry, sizeof(entry), stdin);
        sscanf(entry, "%s %[^\n]", comand, argument);

        comand[strcspn(comand, "\n")] = '\0'; 

        format(comand);
        
        functions(argument, comand, path, freeInodes, freeBlocks, &currentDirectory, root, runSH);
        
        strcpy(argument, "");
    }
}

int main(){
    FreeBlock *freeBlocks = malloc(sizeof(FreeBlock));
    FreeINode *freeInodes = malloc(sizeof(FreeINode));
    Directory *root = NULL;
    INode *runSH = NULL;
    
    initFileSystem(&freeBlocks, &freeInodes, &root, &runSH);

    bash(&freeBlocks, &freeInodes, &root, &runSH);

    return 0;
}

#include <conio.h>
#include "../lib/Inode.h"
#include "../lib/Directory.h"

void generateInodes(FreeINode **freeInodes) {
    FILE *archive;
    FreeINode *aux = *freeInodes;
    
    archive = fopen("src/Resources/INodes.dat", "wb");
    
    if(archive != NULL){
        for (unsigned int i = 0; i < MAX_I_NODE; i++){
            INode *inode = malloc(sizeof(INode));

            inode->id = i;
            inode->type = ((i % 2) ==  0) ? 'd' : 'r';
            inode->status = 0;
            inode->size = 0;
            inode->block_count = 0;

            fwrite(inode, sizeof(INode), 1, archive);

            aux->inode = inode; 

            if (i < MAX_I_NODE - 1) {
                aux->next = malloc(sizeof(FreeINode));
                aux = aux->next;
            } else{
                aux->next = NULL;
            }
        }

        fclose(archive);
    } else
        printf("Error opening INode file. \n");
}

INode *verifyINodeFree(FreeINode **freeInodes, char type) {
    FreeINode *aux = *freeInodes;
    int flag = 1;

    while (flag && aux != NULL){
        if (aux->inode->type == type && aux->inode->status == 0)
            flag = 0;
        else
            aux = aux->next;
    }

    if (aux == NULL)
        return NULL;
    
    return aux->inode;
}

void alterINodeDat(INode *inode) {
    FILE *archive;
    archive = fopen("src/Resources/INodes.dat", "rb+");

    if (archive != NULL) {
        INode aux;
        int flag = 1;
        int resp = 1;

        while (flag && resp) {
            resp = fread(&aux, sizeof(INode), 1, archive);

            if (aux.id == inode->id) {
                fseek(archive, -sizeof(INode), SEEK_CUR);
                fwrite(inode, sizeof(INode), 1, archive);
                flag = 0;   
            }
        }

        if (flag == 1)
            printf("Error: INode not found. ");
        
        fclose(archive);
    } else
        printf("Error opening INode file. \n");
}

void enterINodeFree(FreeINode **freeINode, INode *inode) {
    inode->status = 0;

    FreeINode *aux = *freeINode;

    while (aux != NULL)
        aux = aux->next;
    
    FreeINode *newFreeINode = malloc(sizeof(FreeINode));

    newFreeINode->inode = inode;
    newFreeINode->next = NULL;
    aux = newFreeINode;

    alterINodeDat(inode);
}

void removeINodeFree(FreeINode **freeInodes, INode *inode) { 
    if ((*freeInodes)->inode->id == inode->id) {
        (*freeInodes) = (*freeInodes)->next;
    } else {
        FreeINode *aux = *freeInodes;
        FreeINode *aux2 = NULL;
        int flag = 1;

        while (flag && aux != NULL){
            if(aux->inode->id == inode->id)
                flag = 0;
            else {
                aux2 = aux;
                aux = aux->next;
            }
        }

        if (aux != NULL){
            if (aux2 == NULL)
                freeInodes = &aux;    
            else
                aux2->next = aux->next;
            
            aux->inode->status = 1;

            inode->size = inode->block_count * BLOCK_SIZE;
            alterINodeDat(aux->inode);
        }
        else
            printf("INode not found. ");
    }
}

int readINodeDat(FreeINode **fi) {
    FILE *archive;
    archive = fopen("src/Resources/INodes.dat", "rb");

    if (archive != NULL) {
        *fi = NULL;
        FreeINode *aux = NULL;
        int resp = 1;
        
        while(resp){
            INode *inode = malloc(sizeof(INode));
            resp = fread(inode, sizeof(INode), 1, archive);

            if (inode->status == 0 && resp) {
                FreeINode *newNode = malloc(sizeof(FreeINode));
                newNode->inode = inode;
                newNode->next = NULL;

                if (*fi == NULL) 
                    *fi = newNode;
                else
                    aux->next = newNode;

                aux = newNode;
            } else 
                free(inode);
        }
        
        if (aux != NULL)
            aux->next = NULL;

        fclose(archive);
        return 1;   
    } else {
        printf("Error opening INode file to read. \n");
        return 0;
    }
}

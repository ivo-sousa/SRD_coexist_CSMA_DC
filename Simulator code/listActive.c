#include<stdio.h>
#include <windows.h>

#include "listActive.h"


activeNode* newActiveList(){
    return NULL;
}

int isActiveEmpty(activeNode* q){
    return (q==NULL);
}

void printActiveNodeList(activeNode* q){
    activeNode *aux = q;
    if(isActiveEmpty(q)){
        printf("\n");
        return;
    }

    while(aux!=NULL){
        printf("%d -> %d | ", aux->T, aux->R);
        aux = aux->next;
    }

    printf("\n");
    return;
}

int onList(int val, activeNode* q){
    activeNode* aux = q;
    while(aux!=NULL && aux->T != val)
        aux = aux->next;

    if(aux == NULL)
        return 0;
    else
        return 1;
}

int onListTo(int to, activeNode* q){
    activeNode* aux = q;
    while(aux!=NULL && aux->R != to)
        aux = aux->next;

    if(aux == NULL)
        return 0;
    else
        return 1;
}



void enqueueActive(int val, int to, activeNode** q){
    activeNode* n = malloc(sizeof(activeNode));
    if(n == NULL){
        printf("ERROR - NOT ENOUGH MEMORY\n");
        exit(-1);
    }
    n->T = val;
    n->R = to;

    if(isActiveEmpty((*q))){
        *q = n;
        n->next = NULL;
        return;
    }else if(!onList(val,*q)){
        n->next = *q;
        *q = n;
    }
    return;

}

int dequeueActive(int val, activeNode ** q){
    activeNode* aux = *q;
    activeNode* prev;

    if(isActiveEmpty(*q))
        return 0;
    if((*q)->next == NULL && (*q)->T != val) // list has only 1 node and it is not the one we are looking for
        return 0;

    if((*q)->T == val){ // first node on the list
        *q = aux->next;
        free(aux);
        return 1;
    }else{
        aux = (*q)->next;
        prev = *q;
        while(aux!=NULL){
            if(aux->T == val){
                prev->next = aux->next;
                free(aux);
                return 1;
            }
            aux = aux->next;
            prev = prev->next;
        }
    }

    return 0;
}

void freeActiveList(activeNode** q){

    while(!isActiveEmpty(*q)){
        dequeueActive((*q)->T, q);
    }
    return;
}

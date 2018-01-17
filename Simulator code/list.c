#include<stdio.h>
#include <windows.h>

#include "list.h"




node* newList(){
    return NULL;
}

int isEmpty(node* q){
    return (q==NULL);
}


void printList(node* q,int T){
    node*aux = q;
    if(isEmpty(q)){
        return;
    }
    printf("\n");
    while (aux!=NULL){
        if(aux->Tid == T || T == -1){
            printf("t = %0.3lf | ", aux->Te);
            switch(aux->eventType){
            case(0):
                printf("CSMA startListen");
                break;
            case(1):
                printf("CSMA endListen");
                break;
            case(2):
                printf("CSMA newPacket");
                break;
            case(3):
                printf("CSMA newACK");
                break;
            case(4):
                printf("CSMA endPacket");
                break;
            case(5):
                printf("CSMA endACK");
                break;
            case(6):
                printf("CSMA ACKTimeOut");
                break;
            case(7):
                printf("DC newPacket");
                break;
            case(8):
                printf("DC endPacket");
                break;

            }
            printf(" | %d -> %d | Pid %d\n", aux->Tid, aux->Rid, aux->Pid);

        }
        aux = aux->next;

    }

}


void enqueueSecond(double t, int type, int tid, int rid, int pid, node** q){
    node* new_node = (node*)malloc(sizeof(node));
    if(new_node == NULL){
        printf("ERROR - NOT ENOUGH MEMORY\n");
        exit(-1);
    }
    new_node->Te = t;
    new_node->eventType = type;
    new_node->Rid = rid;
    new_node->Tid = tid;
    new_node->Pid = pid;

    if(isEmpty(*q)){ //q is empty
        *q = new_node;
        new_node->next = NULL;
        return;
    }
    else{
        new_node->next = (*q)->next;
        (*q)->next = new_node;
    }
    return;
}

// Enq sorted
// Enqs node in correspondent place in timeline
void enqueue(double t, int type, int tid, int rid, int pid, node** q){
    node* new_node = (node*)malloc(sizeof(node));
    if(new_node == NULL){
        printf("ERROR - NOT ENOUGH MEMORY\n");
        exit(-1);
    }
    new_node->Te = t;
    new_node->eventType = type;
    new_node->Rid = rid;
    new_node->Tid = tid;
    new_node->Pid = pid;

    if(isEmpty(*q)){ //q is empty
        *q = new_node;
        new_node->next = NULL;
        return;
    }
    if(t<(*q)->Te){ //node is inserted at beginning of q
        new_node->next = *q;
        *q = new_node;
        return;
    }
    //find inserting place within q
    node* current = *q; //auxiliary pointer (starts at head)
    while(current->next!=NULL &&
          current->next->Te <= t){
            current = current->next;
    }
    //already in right position

    new_node->next = current->next;
    current->next = new_node;
    return;

}

// Deq
// Removes first item from q
// Returns 0 if q empty (maybe not so good solution)
int dequeue(node** q){
    if (isEmpty(*q))
        return 0;
    node* aux = *q;
    if((*q)->next == NULL)
        *q = NULL;
    else
        *q = (*q)->next;
    free(aux);
    return 1;

}

void freeEventList(node** eventList){
    while(!isEmpty(*eventList)){
        dequeue(eventList);
    }
    return;
}



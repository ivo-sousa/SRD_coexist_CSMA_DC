#ifndef LISTACTIVE_H_INCLUDED
#define LISTACTIVE_H_INCLUDED

typedef struct activeNode{
    int T; // node id
    int R;
    struct node* next;

}activeNode;

activeNode* newActiveList();
int isActiveEmpty(activeNode* q);
void printActiveNodeList(activeNode* q);
void enqueueActive(int val, int to, activeNode** q);
int dequeueActive(int val, activeNode ** q);
void freeActiveList(activeNode** q);
int onList(int val, activeNode* q);
int onListTo(int to, activeNode* q);


#endif // LISTACTIVE_H_INCLUDED

#ifndef LIST_H_INCLUDED
#define LIST_H_INCLUDED

typedef struct node{
    double Te; // event time
    int eventType;
    int Rid; // receiver id
    int Tid; // transmitter id
    int Pid; // packet id
    struct node* next;

}node;



int isEmpty(node* q);

void printList(node* q, int T);

void enqueueSecond(double t, int type, int tid, int rid, int pid, node** q);

void enqueue(double t, int et, int rid, int tid, int pid, node** q);

int dequeue(node** q);

void freeEventList(node** eventList);


#endif // LIST_H_INCLUDED

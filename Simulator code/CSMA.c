#include <stdio.h>
#include <windows.h>
#include <math.h>

#include "list.h"
#include "listActive.h"
#include "initialization.h"
#include "matrixes.h"
#include "results.h"
#include "CSMA.h"


// if returns 1, prints debug information
int printCondition(node* eventList, networkParameters* np){
    return 0;

}


void initializeStartListen(networkParameters* np, node** eventList, flags** f, counters** c, auxData** ad){
    int i, T, R;
    double t;

    // initialize cycle start time for all nodes
    for(i=0; i<np->nDC; i++){
        (*f)->nextCycleStart[i] = random()*np->TcycleDC;
    }
    for(i=0; i<np->nCSMA; i++){
        (*f)->nextCycleStart[i+np->nDC] = random()*np->TcycleCSMA;
    }


    // put first events on list
    for(i=0; i<np->n; i++){
        if((*ad)->link[i] != -1){

            T = i+1;
            R = (*ad)->link[i];

            if((*ad)->channelAccess[i] == 0){ //DC
                t = (*f)->nextCycleStart[i] + random()*np->ToffsetDC;
                enqueue(t, 7, T, R , 1, eventList);

            }else{ // CSMA
                t = (*f)->nextCycleStart[i] + random()*np->ToffsetCSMA;
                enqueue(t, 0, T, R , 1, eventList);
            }
            updateCycleInfo(T, R, 0, np, f, c, ad, eventList);
        }
    }

    if(printCondition(*eventList, np)){
            printf("- Initial eventList\n");
            printList(*eventList,-1);
    }

    return;
}


void startListen(networkParameters* np, node** eventList, flags** f, counters** c, auxData** ad){
    double Te = (*eventList)->Te;
    int T = (*eventList)->Tid;
    int R = (*eventList)->Rid;
    int P = (*eventList)->Pid;


    // new cycle?
    if(Te>=(*f)->nextCycleStart[T-1]){
        updateCycleInfo(T, R, Te, np, f, c, ad, eventList);
    }


    //event belongs to current cycle?
    if(P != (*f)->currentPid[T-1]){
        return;
    }


    (*c)->offeredPackets[T-1]++;
    enqueueActive(T, R, &((*f)->listeningList));

    listenChannel(T, Te, np, ad, f, eventList);


    // Schedule endListen
    if(np->TL == 0)
        enqueueSecond(Te+np->TL, 1, T, R, P, eventList);
    else
        enqueue(Te+np->TL, 1, T, R, P, eventList);

    return;
}



void endListen(networkParameters* np, node** eventList, flags** f, counters **c, auxData** ad){
    double Te = (*eventList)->Te;
    int T = (*eventList)->Tid;
    int R = (*eventList)->Rid;
    int P = (*eventList)->Pid;
    double t;


    //event belongs to current cycle?
    if(P != (*f)->currentPid[T-1]){
        if(printCondition((*eventList), np))
            printf("- outdated event - drop\n");
        return;
    }

    dequeueActive(T, &((*f)->listeningList));

    // new cycle?
    if(!(Te>=(*f)->nextCycleStart[T-1])){
        // NOT new cycle:

        // detected channel activity?
        if((*f)->collision[T-1]==1 ||
           ((*f)->channelBusySince[T-1] != 0 && Te-(*f)->channelBusySince[T-1] >= np->TR)){
            // reschedule startListen
            t = Te + random()*2*np->Trep;
            enqueue(t, 0, T, R, P, eventList);

            (*c)->rescheduledPackets[T-1]++;
            (*c)->rescheduledPacketsListen[T-1]++;

            if(printCondition((*eventList), np))
                printf("- channel was detected busy by %d\n",T);

        }else{
            //schedule newPacket
            if(np->TD == 0)
                enqueueSecond(Te+np->TD, 2, T, R, P, eventList);
            else
                enqueue(Te+np->TD, 2, T, R, P, eventList);

            if(printCondition((*eventList), np))
                printf("- channel is idle - transmit\n");
        }



    }else{
        // new cycle:
        updateCycleInfo(T, R, Te, np, f, c, ad, eventList);
    }

    // reset flags
    (*f)->collision[T-1] = 0;
    (*f)->channelBusySince[T-1] = 0;
    (*f)->interferenceSum[T-1] = 0;


    return;
}



void newPacket(int packetType, networkParameters* np, flags** f, auxData** ad, counters** c, node** eventList){
    double Te = (*eventList)->Te;
    int T = (*eventList)->Tid;
    int R = (*eventList)->Rid;
    int P = (*eventList)->Pid;

    int Tsensor; // sensor ID
    int RAP; // AP ID



    if(packetType == 0){
        Tsensor = T;
        RAP = R;
    }else{
        Tsensor = R;
        RAP = T;
    }

    // event belongs to current cycle?
    if(P != (*f)->currentPid[Tsensor-1]){
        return;
    }



    // check if new cycle
    if(Te >= (*f)->nextCycleStart[Tsensor-1]){
        updateCycleInfo(Tsensor, RAP, Te, np, f, c, ad, eventList);
        return;
    }


    addInterference(T, R, Te, np, ad, f, eventList);


    // update status
    enqueueActive(T, R, &((*f)->transmittingToList));
    if(!onList(R, (*f)->receivingList)){
        enqueueActive(R, T, &((*f)->receivingList));
    }


    if(packetType == 0){ // data packet
        (*c)->transmittedPackets[T-1]++;
        // schedule endPacket
        enqueue(Te+np->TpCSMA, 4, T, R, P, eventList);
    }else{
        (*c)->transmittedACKs[Tsensor-1]++;

        // schedule endACK
        if(np->TACK == 0)
            enqueueSecond(Te+np->TACK, 5, T, R, P, eventList);
        else
            enqueue(Te+np->TACK, 5, T, R, P, eventList);

    }

    return;
}



void newPacketDC(int packetType, networkParameters* np, flags** f, auxData** ad, counters** c, node** eventList){
    double Te = (*eventList)->Te;
    int T = (*eventList)->Tid;
    int R = (*eventList)->Rid;
    int P = (*eventList)->Pid;



    // check if new cycle
    if(Te>=(*f)->nextCycleStart[T-1]){
        updateCycleInfo(T, R, Te, np, f, c, ad, eventList);
    }


    addInterference(T, R, Te, np, ad, f, eventList);

    (*c)->transmittedPackets[T-1]++;
    (*c)->offeredPackets[T-1]++;

    //(*f)->transmittingTo[T-1] = R;
    enqueueActive(T, R, &((*f)->transmittingToList));

    //if((*f)->receiving[R-1]==0)
    if(!onList(R, (*f)->receivingList)){
        //(*f)->receiving[R-1] = 1;
        enqueueActive(R, T, &((*f)->receivingList));
    }

    // SCHEDULE ENDPACKET

    double t;
    t = Te + np->TpDC;
    enqueue(t, 8, T, R, P, eventList);


    return;

}



void endPacket(int packetType, networkParameters* np, flags** f, auxData** ad, counters** c, node** eventList){
    double Te = (*eventList)->Te;
    int T = (*eventList)->Tid;
    int R = (*eventList)->Rid;
    int P = (*eventList)->Pid;


    int Tsensor; // sensor ID
    int RAP; // AP ID

    if(packetType == 0){
        Tsensor = T;
        RAP = R;
    }else{
        Tsensor = R;
        RAP = T;
    }


    if(P != (*f)->currentPid[Tsensor-1]){
        return;
    }


    dequeueActive(T, &((*f)->transmittingToList));

    if(!onListTo(R, (*f)->transmittingToList)){
        dequeueActive(R, &((*f)->receivingList));
    }


    if((Te>=(*f)->nextCycleStart[Tsensor-1])){
        updateCycleInfo(Tsensor, RAP, Te, np, f, c, ad, eventList);
        return;
    }

    if(packetType == 0){ // data packet

        // schedule ACktimeout
        // NOTA: coloquei o ACK timeout aqui, para se se fizer o enqueueSecond do ACK a seguir, ficar antes deste
        if(np->TtimeoutACK == 0)
            enqueueSecond(Te+np->TtimeoutACK, 6, T, R, P, eventList);
        else
            enqueue(Te+np->TtimeoutACK, 6, T, R, P, eventList);

        (*f)->waitingACK[T-1] = 1;


        if((*f)->collision[T-1] == 0){
            (*c)->successfulPackets[T-1]++;


            // schedule ACK
            if(np->Tresp==0)
                enqueueSecond(Te+np->Tresp, 3, R, T, P, eventList);
            else
                enqueue(Te+np->Tresp, 3, R, T, P, eventList);

            if(printCondition(*eventList, np))
                printf("- packet received successfully; ACK scheduled\n");

        }else{
            (*c)->lostPacketsCollision[T-1]++;
            if(printCondition(*eventList, np))
                printf("- packet on node %d has collided\n",T);
        }

    }else if(packetType == 1){ // ACK packet

        if((*f)->collision[T-1] == 0){
            (*f)->waitingACK[Tsensor-1] = 0;
            (*c)->receivedACKs[Tsensor-1]++;
            (*f)->transmittedLastPacket[Tsensor-1] = 1;

            if(printCondition(*eventList, np))
                printf("- ACK correctly received\n");

        }else{
            (*c)->lostACKs[Tsensor-1]++;
            if(printCondition(*eventList, np))
                printf("- ACK was not received\n");

        }

    }

    // reset flags
    (*f)->collision[T-1] = 0;
    (*f)->interferenceSum[R-1] = 0;

    subtractInterference(T, Te, np, ad, f, eventList);

    return;
}

void endPacketDC(int packetType, networkParameters* np, flags** f, auxData** ad, counters** c, node** eventList){
    int T = (*eventList)->Tid;
    int R = (*eventList)->Rid;

    double Te = (*eventList)->Te;

    activeNode* aux;
    activeNode* aux2;


    dequeueActive(T, &((*f)->transmittingToList));

    if(!onListTo(R, (*f)->transmittingToList)){ // only sets receiving=0 if no other transmitter is currently sending to the same receiver
        dequeueActive(R, &((*f)->receivingList));
    }


    if((Te>=(*f)->nextCycleStart[T-1])){
        updateCycleInfo(T, R, Te, np, f, c, ad, eventList);
        return;
    }

    // CHECK IF THERE WAS A COLLISION

    if((*f)->collision[T-1] == 0){
        (*c)->successfulPackets[T-1]++;
        (*f)->transmittedLastPacket[T-1] = 1;
    }else{
        (*c)->lostPacketsCollision[T-1]++;
        if(printCondition(*eventList, np))
            printf("- packet on node %d has collided\n",T);
    }




    (*f)->collision[T-1] = 0;
    (*f)->interferenceSum[R-1] = 0;

    subtractInterference(T, Te, np, ad, f, eventList);


    return;


}

void ACKTimeOut(networkParameters* np, flags** f, counters** c, auxData** ad, node** eventList){
    double Te = (*eventList)->Te;
    int T = (*eventList)->Tid;
    int R = (*eventList)->Rid;
    int P = (*eventList)->Pid;

    double t;




    if(P != (*f)->currentPid[T-1]){
        return;
    }

    if((Te>=(*f)->nextCycleStart[T-1])){
        updateCycleInfo(T, R, Te, np, f, c, ad, eventList);
        return;
    }

    if((*f)->waitingACK[T-1] == 0){
        if(printCondition(*eventList, np))
            printf("ACK was already received\n");
        return;
    }

    if(printCondition(*eventList, np))
            printf("ACK was not received - reschedule startListen\n");

    (*f)->waitingACK[T-1] = 0;

    (*c)->rescheduledPackets[T-1]++;
    // reschedule startListen
    t = Te + random()*2*np->Ttimeoutrep;
    enqueue(t, 0, T, R, P, eventList);

    if(printCondition(*eventList, np))
        printf("- ACK was not received - reschedule startListen\n");



    return;
}






void updateCycleInfo(int T, int R, double Te, networkParameters* np, flags** f, counters** c, auxData** ad, node** eventList){
    double nextT;
    int nextP;


    if(printCondition(*eventList, np))
        printf("- new cycle!\n");


    // was transmitting when cycle changed
    if(onList(T, (*f)->transmittingToList)){
        if(printCondition(*eventList, np))
            printf("node was transmitting when cycle changed\n");

        //reset transmission flags
        dequeueActive(T, &((*f)->transmittingToList));
        if(onListTo(R, &((*f)->transmittingToList)))
            dequeueActive(R, &((*f)->receivingList));


        subtractInterference(T, Te, np, ad, f, eventList);

    }
    // was listening when cycle changed
    if(onList(T, (*f)->listeningList)){
        if(printCondition(*eventList, np))
            printf("node was listening when cycle changed\n");

        //reset listening flags
        dequeueActive(T, &((*f)->receivingList));

    }



    (*f)->currentPid[T-1] = ((*f)->currentPid[T-1]+1)%2;

    if((*ad)->channelAccess[T-1] == 0){ // DC
        (*f)->nextCycleStart[T-1] = (*f)->nextCycleStart[T-1] + np->TcycleDC;
    }else{
        (*f)->nextCycleStart[T-1] = (*f)->nextCycleStart[T-1] + np->TcycleCSMA;
    }

    //schedule next cycle event
    nextP = ((*f)->currentPid[T-1]+1)%2;

    if((*ad)->channelAccess[T-1] == 0){ // DC
        nextT = (*f)->nextCycleStart[T-1] + random()*np->ToffsetDC;
        enqueue(nextT, 7, T, R, nextP, eventList);
    }else{
        nextT = (*f)->nextCycleStart[T-1] + random()*np->ToffsetCSMA;
        enqueue(nextT, 0, T, R, nextP, eventList);
    }

    // if packet from last cycle was not transmitted
    if((*f)->transmittedLastPacket[T-1]==0){
            (*c)->lostPacketsBuffer[T-1]++;
            if(printCondition(*eventList, np))
                printf("- packet from last cycle was not transmitted\n");
    }



    // Reset flags - just to be safe
    (*f)->waitingACK[T-1] = 0;
    (*f)->transmittedLastPacket[T-1] = 0;
    (*f)->collision[T-1] = 0;
    (*f)->channelBusySince[T-1] = 0;



    return;
}



void addInterference(int T, int R, double Te, networkParameters* np, auxData** ad, flags** f, node** eventList){
    activeNode* aux;
    int i, j;
    double y;

    // -------- ADD INTERFERENCE ON CURRENTLY ACTIVE NODES


    // add interf on all currently active Rx
    for(aux = (*f)->receivingList; aux!= NULL; aux = aux->next){

        j = aux->T-1;
        (*f)->interferenceSum[j] += (*ad)->interference[T-1][j];

        if(printCondition(*eventList, np))
            printf("- added interference %.2lf dBm on active Rx %d\n- interference is now %.1lf\n", 10*log10((*ad)->interference[T-1][j]), j+1, 10*log10((*f)->interferenceSum[j]));

    }




    //check effects of interf
    for(aux = (*f)->transmittingToList; aux!=NULL; aux = aux->next){
        i = aux->T-1;
        j = aux->R-1;
        if((*f)->interferenceSum[j] > (*ad)->interferenceLim[i][j]){
            (*f)->collision[i] = 1;
            if(printCondition(*eventList, np))
                printf("- packet has collided with transmission %d->%d (interferenceSum %.1lf > interfLim %.1lf)\n", i+1, j+1, 10*log10((*f)->interferenceSum[j]), 10*log10((*ad)->interferenceLim[i][j]));
        }
    }




    // -------- ADD INTERFERENCE ON THIS RECEIVER

    (*f)->interferenceSum[R-1] = 0;

    // add interf
    for(aux = (*f)->transmittingToList; aux!= NULL; aux = aux->next){
        i = aux->T-1;

        if(i != T-1){
            (*f)->interferenceSum[R-1] += (*ad)->interference[i][R-1];
            if(printCondition(*eventList, np))
                printf("- Added interference %.1lf dBm from %d on this receiver\n", 10*log10((*ad)->interference[i][R-1]), i+1);
        }
    }






    // check interf
    if((*f)->interferenceSum[R-1] > (*ad)->interferenceLim[T-1][R-1]){
        (*f)->collision[T-1] = 1;

        if(printCondition(*eventList, np))
            printf("- other transmitters caused collision on this packet\n");
    }


    // -------- ADD INTERFERENCE ON LISTENING TRANSMITTERS


    for(aux = (*f)->listeningList; aux != NULL; aux = aux->next){
        i = aux->T-1;

        (*f)->interferenceSum[i] += (*ad)->interference[T-1][i];

        y = 10.0*log10((*f)->interferenceSum[i]);
        if(y > np->Pdet){
            if((*f)->channelBusySince[i] == 0){
                (*f)->channelBusySince[i] = Te;

                if(printCondition(*eventList, np))
                    printf("- transmission was detected by %d\n", i+1);
            }
        }
    }

    return;
}


void subtractInterference(int T, double Te, networkParameters* np, auxData** ad, flags** f, node** eventList){
    activeNode* aux;
    int i,j;
    double prevInterference;
    double y;

    // ------- SUBTRACT INTERFERENCE ON CURRENTLY ACTIVE RECEIVERS

    // subtract interf on active receivers
    for(aux = (*f)->receivingList; aux!=NULL; aux = aux->next){
        j = aux->T-1;


        (*f)->interferenceSum[j] -= (*ad)->interference[T-1][j];

        if(printCondition(*eventList, np))
            printf("- subtracted interference %.1lf from active Rx %d (it is now %.1lf)\n", 10*log10((*ad)->interference[T-1][j]), j+1, 10*log10((*f)->interferenceSum[j]));

    }

    // -------  SUBTRACT INTERFERENCE ON LISTENING TRANSMITTERS

    for(aux = (*f)->listeningList; aux!=NULL; aux = aux->next){
        i = aux->T-1;

        prevInterference = 10.0*log10((*f)->interferenceSum[i]);
        (*f)->interferenceSum[i] -= (*ad)->interference[T-1][i];

        y = 10.0*log10((*f)->interferenceSum[i]);
        if(prevInterference > np->Pdet && y < np->Pdet){
            if(Te - (*f)->channelBusySince[i] > np->TR){
                (*f)->collision[i] = 1;

                if(printCondition(*eventList, np))
                    printf("- %d has detected channel busy\n", i+1);
            }else{
                (*f)->channelBusySince[i] = 0;
            }
        }
    }

    return;
}


void listenChannel(int T, double Te, networkParameters* np, auxData** ad, flags** f, node** eventList){
    activeNode* aux;
    int i;
    double y;


    (*f)->interferenceSum[T-1] = 0;
    (*f)->channelBusySince[T-1] = 0;


    for(aux=(*f)->transmittingToList; aux != NULL; aux = aux->next){
        i = aux->T - 1;
        (*f)->interferenceSum[T-1] += (*ad)->interference[i][T-1];

        if(printCondition(*eventList, np))
            printf("- added activity %.1lf dBm from active Tx %d (total now is %.1lf dBm)\n- interference is now %.1lf\n", 10*log10((*ad)->interference[i][T-1]), i+1, 10*log10((*f)->interferenceSum[T-1]));

    }

    y = 10.0*log10((*f)->interferenceSum[T-1]);
    if(y > np->Pdet){
        (*f)->channelBusySince[T-1] = Te;
        if(printCondition(*eventList,np))
           printf("- channel marked busy\n");
    }


    return;
}






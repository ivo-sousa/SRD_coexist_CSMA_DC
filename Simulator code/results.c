#include <stdio.h>
#include <windows.h>

#include "list.h"
#include "listActive.h"
#include "matrixes.h"
#include "initialization.h"
#include "results.h"




// Create and print

results* newResults(iterationParameters* it, networkParameters* np){
    int i;
    int n = it->nn; // horizontal size of results matrixes
    int n2; // vertical size of results matrixes
    if(it->n2>1)
        n2 = it->maxNsimulations;
    else
        n2 = np->Nsimulations;



    results* r = malloc(sizeof(results));

    // Results

    r->G_DC = malloc(n2 * sizeof(double*));
    for(i=0; i<n2; i++)
        r->G_DC[i] = calloc(1,n * sizeof(double));
    r->avG_DC = calloc(1,n*sizeof(double));
    r->varG_DC = calloc(1,n*sizeof(double));

    r->G_CSMA = malloc(n2 * sizeof(double*));
    for(i=0; i<n2; i++)
        r->G_CSMA[i] = calloc(1,n * sizeof(double));
    r->avG_CSMA = calloc(1,n*sizeof(double));
    r->varG_CSMA = calloc(1,n*sizeof(double));


    r->S_DC = malloc(n2 * sizeof(double*));
    for(i=0; i<n2; i++)
        r->S_DC[i] = calloc(1,n * sizeof(double));
    r->avS_DC = calloc(1,n*sizeof(double));
    r->varS_DC = calloc(1,n*sizeof(double));


    r->S_CSMA = malloc(n2 * sizeof(double*));
    for(i=0; i<n2; i++)
        r->S_CSMA[i] = calloc(1,n * sizeof(double));
    r->avS_CSMA = calloc(1,n*sizeof(double));
    r->varS_CSMA = calloc(1,n*sizeof(double));


    r->PLRBuffer_DC = malloc(n2 * sizeof(double*));
    for(i=0; i<n2; i++)
        r->PLRBuffer_DC[i] = calloc(1,n * sizeof(double));
    r->avPLRBuffer_DC = calloc(1,n*sizeof(double));
    r->varPLRBuffer_DC = calloc(1,n*sizeof(double));

    r->PLRBuffer_CSMA = malloc(n2 * sizeof(double*));
    for(i=0; i<n2; i++)
        r->PLRBuffer_CSMA[i] = calloc(1,n * sizeof(double));
    r->avPLRBuffer_CSMA = calloc(1,n*sizeof(double));
    r->varPLRBuffer_CSMA = calloc(1,n*sizeof(double));


    r->PLRCollisions_DC = malloc(n2 * sizeof(double*));
    for(i=0; i<n2; i++)
        r->PLRCollisions_DC[i] = calloc(1,n * sizeof(double));
    r->avPLRCollisions_DC = calloc(1,n*sizeof(double));
    r->varPLRCollisions_DC = calloc(1,n*sizeof(double));

    r->PLRCollisions_CSMA = malloc(n2 * sizeof(double*));
    for(i=0; i<n2; i++)
        r->PLRCollisions_CSMA[i] = calloc(1,n * sizeof(double));
    r->avPLRCollisions_CSMA = calloc(1,n*sizeof(double));
    r->varPLRCollisions_CSMA = calloc(1,n*sizeof(double));


    r->transmissionRate = malloc(n2 * sizeof(double*));
    for(i=0; i<n2; i++)
        r->transmissionRate[i] = calloc(1,n * sizeof(double));
    r->avTransmissionRate = calloc(1,n*sizeof(double));
    r->varTransmissionRate = calloc(1,n*sizeof(double));

    r->ACKlossRate = malloc(n2 * sizeof(double*));
    for(i=0; i<n2; i++)
        r->ACKlossRate[i] = calloc(1,n * sizeof(double));
    r->avACKlossRate = calloc(1,n*sizeof(double));
    r->varACKlossRate = calloc(1,n*sizeof(double));





    r->channelLoad = calloc(1,n * sizeof(double));

    // Average counters

    r->avApplicationPackets = calloc(1,n * sizeof(double));
    r->avOfferedPackets = calloc(1,n * sizeof(double));
    r->avTransmittedPackets = calloc(1,n * sizeof(double));
    r->avLostPacketsBuffer = calloc(1,n * sizeof(double));
    r->avLostPacketsCollision = calloc(1,n * sizeof(double));
    r->avReceivedACKs = calloc(1,n * sizeof(double));
    r->avSuccessfulPackets = calloc(1,n * sizeof(double));
    r->avRescheduledPackets = calloc(1,n * sizeof(double));
    r->avRescheduledPacketsListen = calloc(1,n * sizeof(double));
    r->avTransmittedACKs = calloc(1,n * sizeof(double));
    r->avLostACKs = calloc(1,n * sizeof(double));


    return r;
}






// SAVE RESULTS

void saveResults(networkParameters* np, results** r, counters* c, int k, int i){
    double* aux = createArraydouble(np->n);
    double* auxCSMA = createArraydouble(np->nCSMA);


    //Results

    //-------------------- PLR buffer ---------------------//

    // -- DC
    divInt(c->lostPacketsBuffer, c->offeredPackets, aux, np->n);
    (*r)->PLRBuffer_DC[k][i] = averagedouble(aux, np->nDC);

    // -- CSMA
    divInt(c->lostPacketsBuffer, c->applicationPackets, aux, np->n);
    (*r)->PLRBuffer_CSMA[k][i] = averagedoublePartial(aux, np->nDC, np->n);



    //-------------------- PLR collision ---------------------//

    divInt(c->lostPacketsCollision, c->transmittedPackets, aux, np->n);
    (*r)->PLRCollisions_CSMA[k][i] = averagedoublePartial(aux, np->nDC, np->n);

    //-------------------- transmissionRate ---------------------//

    divInt(c->transmittedPackets, c->applicationPackets, aux, np->n);
    (*r)->transmissionRate[k][i] = averagedoublePartial(aux, np->nDC, np->n);


    //-------------------- G (offered load) ---------------------//

    // -- DC
    (*r)->G_DC[k][i] = np->TpDC / np->TcycleDC * np->nDC;

    // -- CSMA
    divInt(c->offeredPackets, c->applicationPackets, aux, np->n);
    (*r)->G_CSMA[k][i] = averagedoublePartial(aux, np->nDC, np->n) * np->TpCSMA / np->TcycleCSMA * np->nCSMA;


    //-------------------- S (throughput) ---------------------//

    // -- DC
    divInt(c->successfulPackets, c->offeredPackets, aux, np->n);
    (*r)->S_DC[k][i] = averagedouble(aux, np->nDC) * np->TpDC / np->TcycleDC * np->nDC;

    // -- CSMA
    divInt(c->receivedACKs, c->applicationPackets, aux, np->n);
    (*r)->S_CSMA[k][i] = averagedoublePartial(aux, np->nDC, np->n) * np->TpCSMA / np->TcycleCSMA * np->nCSMA;


    //-------------------- ACKlossRate ---------------------//

    divInt(c->lostACKs, c->transmittedACKs, aux, np->n);
    (*r)->ACKlossRate[k][i] = averagedoublePartial(aux, np->nDC, np->n) * np->TpCSMA / np->TcycleCSMA * np->nCSMA;




    // Counters average (not used)
    double a = averageInt(c->applicationPackets, np->n);

    (*r)->avApplicationPackets[i] = averageInt(c->applicationPackets, np->n);
    (*r)->avOfferedPackets[i] = averageInt(c->offeredPackets, np->n);
    (*r)->avTransmittedPackets[i] = averageInt(c->transmittedPackets, np->n);
    (*r)->avLostPacketsBuffer[i] = averageInt(c->lostPacketsBuffer, np->n);
    (*r)->avLostPacketsCollision[i] = averageInt(c->lostPacketsCollision, np->n);
    (*r)->avReceivedACKs[i] = averageInt(c->receivedACKs, np->n);
    (*r)->avSuccessfulPackets[i] = averageInt(c->successfulPackets, np->n);
    (*r)->avRescheduledPackets[i] = averageInt(c->rescheduledPackets, np->n);
    (*r)->avRescheduledPacketsListen[i] = averageInt(c->rescheduledPacketsListen, np->n);
    (*r)->avTransmittedACKs[i] = averageInt(c->transmittedACKs,np->n);
    (*r)->avLostACKs[i] = averageInt(c->lostACKs,np->n);



    free(aux);
    return;
}


void calculateVariance(results** r, iterationParameters* it, networkParameters* np){
    int i, j;
    int Nsim, size;

    if(it->n2 > 1){
        size = it->maxNsimulations;
    }else{
        size = np->Nsimulations;
    }



    double* aux = calloc(1, size* sizeof(double));

    for(i=0; i<it->nn; i++){ // for all values of n
        if(it->n2 == 1){
            Nsim = np->Nsimulations;
        }else{
            Nsim = it->Nsimulations[i];
        }


        collumn((*r)->G_DC, aux, Nsim, i);
        (*r)->avG_DC[i] = averagedouble(aux, Nsim);
        (*r)->varG_DC[i] = stdDevDouble(aux, (*r)->avG_DC[i], Nsim);

        collumn((*r)->G_CSMA, aux, Nsim, i);
        (*r)->avG_CSMA[i] = averagedouble(aux, Nsim);
        (*r)->varG_CSMA[i] = stdDevDouble(aux, (*r)->avG_CSMA[i], Nsim);


        collumn((*r)->S_DC, aux, Nsim, i);
        (*r)->avS_DC[i] = averagedouble(aux, Nsim);
        (*r)->varS_DC[i] = stdDevDouble(aux, (*r)->avS_DC[i], Nsim);

        collumn((*r)->S_CSMA, aux, Nsim, i);
        (*r)->avS_CSMA[i] = averagedouble(aux, Nsim);
        (*r)->varS_CSMA[i] = stdDevDouble(aux, (*r)->avS_CSMA[i], Nsim);


        collumn((*r)->PLRBuffer_DC, aux, Nsim, i);
        (*r)->avPLRBuffer_DC[i] = averagedouble(aux, Nsim);
        (*r)->varPLRBuffer_DC[i] = stdDevDouble(aux, (*r)->avPLRBuffer_DC[i], Nsim);

        collumn((*r)->PLRBuffer_CSMA, aux, Nsim, i);
        (*r)->avPLRBuffer_CSMA[i] = averagedouble(aux, Nsim);
        (*r)->varPLRBuffer_CSMA[i] = stdDevDouble(aux, (*r)->avPLRBuffer_CSMA[i], Nsim);


        collumn((*r)->PLRCollisions_CSMA, aux, Nsim, i);
        (*r)->avPLRCollisions_CSMA[i] = averagedouble(aux, Nsim);
        (*r)->varPLRCollisions_CSMA[i] = stdDevDouble(aux, (*r)->avPLRCollisions_CSMA[i], Nsim);



        collumn((*r)->ACKlossRate, aux, Nsim, i);
        (*r)->avACKlossRate[i] = averagedouble(aux, Nsim);
        (*r)->varACKlossRate[i] = stdDevDouble(aux, (*r)->avACKlossRate[i], Nsim);

        collumn((*r)->transmissionRate, aux, Nsim, i);
        (*r)->avTransmissionRate[i] = averagedouble(aux, Nsim);
        (*r)->varTransmissionRate[i] = stdDevDouble(aux, (*r)->avTransmissionRate[i], Nsim);

    }
    free(aux);

}


// Output

void outputNetworkParameters(FILE* file, networkParameters* np){

    fprintf(file,"\n-- SIMULATION PARAMETERS --\n");

    fprintf(file,"DC_DC;%f\n", np->DCDC);
    fprintf(file,"Tp_DC;%f\n", np->TpDC);
    fprintf(file,"Tcycle_DC;%f\n", np->TcycleDC);
    fprintf(file,"ToffsetDC/Tp;%f\n", np->ToffsetDC/np->TpDC);

    fprintf(file,"\n-- CSMA --\n");
    fprintf(file,"DC_CSMA;%f\n", np->DCCSMA);
    fprintf(file,"Tp_CSMA;%f\n", np->TpCSMA);
    fprintf(file,"Tcycle_CSMA;%f\n", np->TcycleCSMA);
    fprintf(file,"ToffsetCSMA/Tp;%f\n", np->ToffsetCSMA/np->TpCSMA);

    fprintf(file,"Pt;%f\n", np->Pt);

    fprintf(file,"\n");

    return;
}

void checkOutputFile(networkParameters* np, char* f){
    FILE* file = fopen(f, "a");
    if(file == NULL){
        printf("Cannot create output file\n");
        exit(0);
    }
    fclose(file);
    return;
}

void outputResults(results* r, iterationParameters* it, networkParameters* np, char* f){
FILE* file = fopen(f, "a");
    if(file == NULL){
        printf("Cannot create output file\n");
        exit(0);
    }

    fprintf(file, "\n\n\n-------- NEW SIMULATION ---------\n");
    outputNetworkParameters(file, np);

    int k;

    fprintf(file, "nDC;%d\n", np->nDC);

    // Number of simulations
    if(it->n2 > 1){
        int aux2[it->n2];
        for(k=0; k<it->n2; k++)
            aux2[k]=it->Nsimulations[k];

        fprintf(file, "Nsimulations;");
        outputArrayInt(file, aux2, it->n2);
    }
    else
        fprintf(file, "Nsimulations;%d\n",np->Nsimulations);



    fprintf(file, "nDC;%d\n", np->nDC);

    //Average and variance
    fprintf(file, "\nG_DC av;");
    outputArray(file, r->avG_DC, it->nn);
    fprintf(file, "G_DC std dev;");
    outputArray(file, r->varG_DC, it->nn);
    fprintf(file,"\n");

    fprintf(file,"S_DC av;");
    outputArray(file, r->avS_DC, it->nn);
    fprintf(file,"S_DC std dev;");
    outputArray(file, r->varS_DC, it->nn);
    fprintf(file,"\n");

    fprintf(file, "PLR buffer_DC av;");
    outputArray(file, r->avPLRBuffer_DC, it->nn);
    fprintf(file, "PLR buffer_DC std dev;");
    outputArray(file, r->varPLRBuffer_DC, it->nn);
    fprintf(file,"\n");



    // Number of simulations (again)
    if(it->n2 > 1){
        int aux2[it->n2];
        for(k=0; k<it->n2; k++)
            aux2[k]=it->Nsimulations[k];

        fprintf(file, "Nsimulations;");
        outputArrayInt(file, aux2, it->n2);
    }
    else
        fprintf(file, "Nsimulations;%d\n",np->Nsimulations);


    fprintf(file, "nCSMA;");
    outputArrayInt(file, it->n, it->nn);


    //Average and variance
    fprintf(file, "\nG_CSMA av;");
    outputArray(file, r->avG_CSMA, it->nn);
    fprintf(file, "G_CSMA std dev;");
    outputArray(file, r->varG_CSMA, it->nn);
    fprintf(file,"\n");

    fprintf(file,"S_CSMA av;");
    outputArray(file, r->avS_CSMA, it->nn);
    fprintf(file,"S_CSMA std dev;");
    outputArray(file, r->varS_CSMA, it->nn);
    fprintf(file,"\n");

    fprintf(file, "PLR buffer_CSMA av;");
    outputArray(file, r->avPLRBuffer_CSMA, it->nn);
    fprintf(file, "PLR buffer_CSMA std dev;");
    outputArray(file, r->varPLRBuffer_CSMA, it->nn);
    fprintf(file,"\n");

    fprintf(file, "PLR collisions_CSMA av;");
    outputArray(file, r->avPLRCollisions_CSMA, it->nn);
    fprintf(file, "PLR collisions_CSMA std dev;");
    outputArray(file, r->varPLRCollisions_CSMA, it->nn);
    fprintf(file,"\n");

    fprintf(file, "ACK loss rate av;");
    outputArray(file, r->avACKlossRate, it->nn);
    fprintf(file, "ACK loss rate std dev;");
    outputArray(file, r->varACKlossRate, it->nn);
    fprintf(file,"\n");

    fprintf(file, "Transmission rate rate av;");
    outputArray(file, r->avTransmissionRate, it->nn);
    fprintf(file, "Transmission rate std dev;");
    outputArray(file, r->varTransmissionRate, it->nn);

    fclose(file);

    return;

}

void outputDetailedResults(results* r, iterationParameters* it, networkParameters* np, char* f){
    FILE* file = fopen(f, "a");
    if(file == NULL){
        printf("Cannot create output file\n");
        exit(0);
    }

    fprintf(file, "\n\n\nNEW SIMULATION\n");
    outputNetworkParameters(file, np);

    int k, size;


    // Number of simulations
    if(it->n2 > 1){
        int aux2[it->n2];
        for(k=0; k<it->n2; k++)
            aux2[k]=it->Nsimulations[k];

        fprintf(file, "Nsimulations;");
        outputArrayInt(file, aux2, it->n2);
    }else{
        fprintf(file, "Nsimulations;%d\n",np->Nsimulations);
    }

    // n
    if(it->nn > 1){
        int aux[it->nn];
        for(k=0; k<it->nn; k++)
            aux[k]=it->n[k];

        fprintf(file, "n;");
        outputArrayInt(file, aux, it->nn);
    }else{
        fprintf(file, "n;%d\n",np->n);
    }
    if(it->n2 > 1){
        size = it->maxNsimulations;
    }else{
        size = np->Nsimulations;
    }

    //Full results


    fprintf(file,"------ CSMA ------\n");



    fprintf(file, "PLR buffer_CSMA\n");
    outputMatrix(file, r->PLRBuffer_CSMA, size, it->nn);


    fclose(file);
}

void outputCounters(networkParameters* np, counters* c, iterationParameters* it, char* f, auxData *ad){
    FILE* file = fopen(f, "a");
    if(file == NULL){
        printf("Cannot create output file\n");
        exit(0);
    }
    fprintf(file, "----- new simulation -----\n\n");
    fprintf(file,"nDC;%d\nnCSMA;%d\n", np->nDC, np->nCSMA);
    fprintf(file, "\nchannelAccess;");
    outputArrayInt(file, ad->channelAccess, np->n);

    fprintf(file,"application packets;");
    outputArrayInt(file,c->applicationPackets,np->n);
    fprintf(file,"offered packets;");
    outputArrayInt(file,c->offeredPackets,np->n);
    fprintf(file,"transmitted packets;");
    outputArrayInt(file,c->transmittedPackets,np->n);
    fprintf(file,"successful packets;");
    outputArrayInt(file,c->successfulPackets,np->n);
    fprintf(file,"rescheduled packets;");
    outputArrayInt(file,c->rescheduledPackets,np->n);
    fprintf(file,"lost packets collision;");
    outputArrayInt(file,c->lostPacketsCollision,np->n);
    fprintf(file,"lost packets buffer;");
    outputArrayInt(file,c->lostPacketsBuffer,np->n);
    fprintf(file,"transmitted ACKs;");
    outputArrayInt(file,c->transmittedACKs,np->n);
    fprintf(file,"received ACKs;");
    outputArrayInt(file,c->receivedACKs,np->n);
    fprintf(file,"lost ACKs;");
    outputArrayInt(file,c->lostACKs,np->n);
    fprintf(file,"rescheduled packets listen;");
    outputArrayInt(file,c->rescheduledPacketsListen,np->n);

    fprintf(file,"\n\n");


    fclose(file);

    return;
}

void outputCountersAverage(networkParameters* np, node* eventList, counters* c){
    char f[100];
    sprintf(f, "Results/countersAverageTime.csv");
    FILE* file = fopen(f, "a");
    if(file == NULL){
        printf("Cannot create output file\n");
        exit(0);
    }

    fprintf(file,"T; application; offered; transmitted; successful; rescheduled;lostCollision; lostBuffer\n");
    fprintf(file, "%.2f;", eventList->Te);


    fprintf(file, "%.2f;",averageInt(c->applicationPackets,np->n));
    fprintf(file, "%.2f;",averageInt(c->offeredPackets,np->n));
    fprintf(file, "%.2f;",averageInt(c->transmittedPackets,np->n));
    fprintf(file, "%.2f;",averageInt(c->successfulPackets,np->n));
    fprintf(file, "%.2f;",averageInt(c->rescheduledPackets,np->n));
    fprintf(file, "%.2f;",averageInt(c->lostPacketsCollision,np->n));
    fprintf(file, "%.2f;",averageInt(c->lostPacketsBuffer,np->n));



    fclose(file);
    return;
}

void outputAuxData(networkParameters* np, auxData* ad, char* f){
    int i,j;
    int SIZE = np->n + np->nAP;

    FILE* file = fopen(f, "a");
    if(file == NULL){
        printf("Cannot create output file\n");
        exit(0);
    }

    fprintf(file, "\n\n------------------------------------------------------\n");


    fprintf(file,"-- AP POSITIONS --\n");
    fprintf(file, "AP;x;y;range\n");

    for(i=0; i<np->nAP; i++){
        fprintf(file, "%d;%lf;%lf;%lf\n", i+1, ad->APpositions[i][0], ad->APpositions[i][1], range(np));
    }
    fprintf(file, "\n\n");

    fprintf(file, "-- SENSOR POSITIONS --\n");
    fprintf(file, "node;x;y;link\n");

    for(i=0; i<np->n; i++){
        fprintf(file, "%d;%lf;%lf;%d\n", i+1, ad->sensorPositions[i][0], ad->sensorPositions[i][1], ad->link[i]);
    }

    fprintf(file, "\nOut of range sensors;");
    for(i=0; i<np->n; i++){
        if(ad->link[i]==-1)
            fprintf(file,"%d;",i+1);
    }

    fprintf(file,"\n-- LINK --\n");
    for(i=0; i<np->n; i++){
        fprintf(file, "%d;%d\n", i,ad->link[i]);
    }


    fprintf(file,"\n-- PATH LOSS [db] --\n");
    for(i=0; i<SIZE; i++){
        for(j=0; j<SIZE;j++){
            fprintf(file, "%lf;", (double) ad->pathLoss[i][j]);
        }
        fprintf(file,"\n");
    }


    fprintf(file, "\n-- INTERFERENCE [db] --\n");
    for(i=0; i<SIZE; i++){
        for(j=0; j<SIZE;j++){
            fprintf(file, "%lf;", (double) 10.0*log10(ad->interference[i][j]));
        }
        fprintf(file, "\n");
    }

    fprintf(file, "\n-- INTERFERENCE LIMIT [db] --\n");

    for(i=0; i<SIZE; i++){
        for(j=0; j<SIZE;j++){
            fprintf(file, "%lf;", (double) 10*log10(ad->interferenceLim[i][j]));
        }
        fprintf(file, "\n");
    }

    fclose(file);
}

#include <stdio.h>
#include <windows.h>
#include <math.h>

#include "listActive.h"
#include "list.h"
#include "initialization.h"
#include "matrixes.h"


flags* newFlags(networkParameters* np){
    int i;
    int SIZE = np->n + np->nAP;

    flags* f = (flags*)malloc(sizeof(flags));

    f->listening = calloc(1, SIZE * sizeof(int));
    f->listeningList = NULL;//newActiveList();
    f->transmittingTo = calloc(1, SIZE * sizeof(int));
    f->transmittingToList = NULL;//newActiveList();
    f->receiving = calloc(1, SIZE * sizeof(int));
    f->receivingList = NULL;//newActiveList();
    f->waitingACK = calloc(1, SIZE * sizeof(int));
    f->transmittedLastPacket = calloc(1, SIZE * sizeof(int));
    // fill transmittedLastPacket with 1, so it does not throw away the first newPacket event
    for(i=0; i<SIZE; i++){
        f->transmittedLastPacket[i] = 1;
    }
    f->collision = calloc(1, SIZE * sizeof(int));
    f->currentPid = calloc(1, SIZE * sizeof(int));

    f->channelBusySince = calloc(1, SIZE * sizeof(double));
    f->nextCycleStart = calloc(1, SIZE * sizeof(double));

    f->interferenceSum = calloc(1, SIZE * sizeof(long double));



    return f;
}

counters* newCounters(networkParameters* np){
    int SIZE = np->n;

    counters* c = malloc(sizeof(counters));

    c->applicationPackets = calloc(1, SIZE * sizeof(int));
    c->offeredPackets = calloc(1, SIZE * sizeof(int));
    c->lostPacketsCollision = calloc(1, SIZE * sizeof(int));
    c->lostPacketsBuffer = calloc(1, SIZE * sizeof(int));
    c->transmittedPackets = calloc(1, SIZE * sizeof(int));
    c->successfulPackets = calloc(1, SIZE * sizeof(int));
    c->receivedACKs = calloc(1, SIZE * sizeof(int));
    c->rescheduledPackets = calloc(1, SIZE * sizeof(int));
    c->rescheduledPacketsListen = calloc(1, SIZE * sizeof(int));
    c->transmittedACKs = calloc(1, SIZE * sizeof(int));
    c->lostACKs = calloc(1, SIZE * sizeof(int));


    return c;
}

networkParameters* newNetwork(){
    networkParameters*np = malloc(sizeof(networkParameters));
    if(np==NULL){
        printf("Not enough memory\n");
        exit(-1);
    }
    return np;
}

iterationParameters* newIteration(){
    iterationParameters* it = malloc(sizeof(iterationParameters));
    it->nPt = 1;
    it->Pt = NULL;
    it->nTsim = 1;
    it->Tsim = NULL;
    it->n = NULL;
    it->nn = 1;
    it->Nsimulations = NULL;
    it->n2 = 1;
    it->maxNsimulations = 1;


    return it;
}




// Auxiliary functions

double random(){
    return (double)rand() / (double)RAND_MAX ;
}

int countTokens(char* str){
    char* pal;
    int k = 0;
    pal = strtok(str," ");
    while(pal!=NULL){
        k++;
        pal = strtok(NULL, " ");
    }
    return k;
}

int* parseStringInt(char* str, int n){
    int* array = createArrayInt(n);
    char* p;
    double val;
    int i = 0;

    p = strtok(str, " ");
    while(p!= NULL){
        if(sscanf(p,"%f",&val)==1){
            array[i] = atoi(p);
            i++;
        }
        p = strtok(NULL, " ");
    }
    return array;
}

double* parseStringdouble(char*str, int n){
    double* array = createArraydouble(n);
    char* p;
    double val;
    int i = 0;

    p = strtok(str, " ");
    while(p!= NULL){
        if(sscanf(p,"%f",&val)==1){
            array[i] = atof(p);
            i++;
        }
        p = strtok(NULL, " ");
    }
    return array;
}

int perfectSquare(int x){
    float aux = sqrt((float)x);

    aux = aux - (int) aux;
    if (aux == 0)
        return 1;
    else
        return 0;

}



// Main initialization functions

void setParameter(networkParameters ** nw, char* parameter, double val){
    if(strcmp(parameter, "Ncycles")==0){
        (*nw)->Tmax = val;
        return;
    }
    else if(strcmp(parameter, "Nsimulations")==0){
        (*nw)->Nsimulations = val;
        return;
    }
    else if(strcmp(parameter, "nCSMA")==0){
        (*nw)->nCSMA = val;
        return;
    }
    else if(strcmp(parameter, "nDC")==0){
        (*nw)->nDC = val;
        return;
    }
    else if(strcmp(parameter, "nAP")==0){
        (*nw)->nAP = val;
        return;
    }
    else if(strcmp(parameter, "lx")==0){
        (*nw)->lx = val;
        return;
    }
    else if(strcmp(parameter, "ly")==0){
        (*nw)->ly = val;
        return;
    }
    else if(strcmp(parameter, "lz")==0){
        (*nw)->lz = val;
        return;
    }
    else if(strcmp(parameter, "z")==0){
        (*nw)->z = val;
        return;
    }
    else if(strcmp(parameter, "f")==0){
        (*nw)->f = val;
        return;
    }
    else if(strcmp(parameter, "a")==0){
        (*nw)->a = val;
        return;
    }
    else if(strcmp(parameter, "DCCSMA")==0){
        (*nw)->DCCSMA = val;
        return;
    }
    else if(strcmp(parameter, "DCDC")==0){
        (*nw)->DCDC = val;
        return;
    }
    else if(strcmp(parameter, "TpCSMA")==0){
        (*nw)->TpCSMA = val;
        return;
    }
    else if(strcmp(parameter, "TpDC")==0){
        (*nw)->TpDC = val;
        return;
    }
    else if(strcmp(parameter, "B")==0){
        (*nw)->B = val;
        return;
    }
    else if(strcmp(parameter, "Pt")==0){
        (*nw)->Pt = val;
        return;
    }
    else if(strcmp(parameter, "N0")==0){
        (*nw)->N0 = val;
        return;
    }
    else if(strcmp(parameter, "NF")==0){
        (*nw)->NF = val;
        return;
    }
    else if(strcmp(parameter, "TL")==0){
        (*nw)->TL = val;
        return;
    }
    else if(strcmp(parameter, "TD")==0){
        (*nw)->TD = val;
        return;
    }
    else if(strcmp(parameter, "TR")==0){
        (*nw)->TR = val;
        return;
    }
    else if(strcmp(parameter, "Trep/Tp")==0){
        (*nw)->Trep = val;
        return;
    }
    else if(strcmp(parameter, "SINRmin")==0){
        (*nw)->SINRmin = val;
        return;
    }
    else if(strcmp(parameter, "Pdet")==0){
        (*nw)->Pdet = val;
        return;
    }
    else if(strcmp(parameter, "ToffsetCSMA/Tp")==0){
        (*nw)->ToffsetCSMA = val;
        return;
    }
    else if(strcmp(parameter, "TtimeoutACK")==0){
        (*nw)->TtimeoutACK = val;
        return;
    }
    else if(strcmp(parameter, "Tresp")==0){
        (*nw)->Tresp = val;
        return;
    }
    else if(strcmp(parameter, "Ttimeoutrep/Tp")==0){
        (*nw)->Ttimeoutrep = val;
        return;
    }
    else if(strcmp(parameter, "TACK")==0){
        (*nw)->TACK = val;
        return;
    }
    else if(strcmp(parameter, "ACK_switch")==0){
        (*nw)->ACK_switch = val;
        return;
    }
    else if(strcmp(parameter, "scenario")==0){
        (*nw)->scenario = val;
        if(val != 0 && val!= 1){
            printf("ERRO - \"scenario\" parameter in initialization file needs to be 0 or 1\n");
            exit(-1);
        }
        return;
    }
    else{
        printf("parameter %s in file but not implemented\n", parameter);
    }

    (*nw)->scenario = 1; // current simulator implementation only supports "building" scenario

    return;

}

networkParameters* initializeParameters(char* f){
    FILE* file = fopen(f, "r");
    if(file == NULL){
        printf("Initialization file could not be opened\n");
        exit(-1);
    }

    networkParameters* nw = newNetwork();

    int k;
    double val;
    char line[1024];
    char parameter[20];

    while(fgets(line, 100, file) != NULL){
        k = sscanf(line, "%s %*s %lf", &parameter, &val);
        if (k==2){
            setParameter(&nw, parameter, val);
        }
    }

    nw->n = nw->nCSMA + nw->nDC;
    nw->ToffsetCSMA = nw->ToffsetCSMA * nw->TpCSMA;
    nw->TcycleCSMA = nw->TpCSMA/(nw->DCCSMA/100.0);
    nw->Trep = nw->Trep * nw->TpCSMA;
    nw->Ttimeoutrep = nw->Ttimeoutrep * nw->TpCSMA;

    nw->TcycleDC = nw->TpDC/(nw->DCDC/100.0);
    nw->ToffsetDC = nw->TcycleDC - nw->TpDC -0.001;


    nw->Tmax = nw->Tmax * nw->TcycleCSMA;
    if(nw->scenario == 0)
        nw->nAP = 1;


    if(nw->ToffsetCSMA >= nw->TcycleCSMA){
        printf("WARNING: Toffset must be smaller than Tcycle\n");
        exit(0);
    }
    if(nw->Trep*2 >= nw->TcycleCSMA){
        printf("WARNING: 2Trep must be smaller than Tcycle\n");
        exit(0);
    }
    if(nw->Ttimeoutrep*2 >= nw->TcycleCSMA){
        printf("WARNING: 2Ttimeoutrep must be smaller than Tcycle\n");
        exit(0);
    }
    if(nw->TtimeoutACK <= nw->Tresp + nw->TACK){
        printf("WARNING: TtimeoutACK must be larger than Tresp + TACK\n");
        exit(0);
    }
    if(!perfectSquare(nw->nAP)){
        printf("WARNING: nAP must be a perfect square\n");
        exit(0);
    }


    fclose(file);

    return nw;
}

int maxInt(int* arr, int k){
    int n = 0;
    int i;
    for(i=0;i<k;i++)
        if(arr[i]>n)
            n = arr[i];
    return n;
}


iterationParameters* initializeIterationParameters(char* f){
    FILE* file = fopen(f, "r");
    if(file == NULL){
        printf("There is no parameter iteration file\n");
        return NULL;
    }

    iterationParameters* it = newIteration();

    int k, n, i;
    double val1, val2;
    char line[1024], aux[1024];
    char parameter[20];

    while(fgets(line, 1024, file) != NULL){
        k = sscanf(line, "%s %*s %f %f",&parameter, &val1, &val2);
        if (k == 3){
            strcpy(aux, line);
            n = countTokens(aux)-2;
            i = 0;

            if(strcmp(parameter, "n") == 0){
                it->n = parseStringInt(line,n);
                it->nn = n;
            }
            else if(strcmp(parameter, "Pt") == 0){
                it->Pt = parseStringInt(line, n);
                it->nPt = n;
            }
            else if(strcmp(parameter, "Tsim") == 0){
                it->Tsim = parseStringInt(line, n);
                it->nTsim = n;
            }
            else if(strcmp(parameter, "Nsimulations") == 0){
                if(n == it->nn){
                    it->Nsimulations = parseStringInt(line,n);
                    it->n2 = n;
                    it->maxNsimulations = maxInt(it->Nsimulations, it->n2);
                }else{
                    printf("WARNING: number of Nsimulations items in \"iterations.txt\" does not correspond to number of items of n\n");
                    exit(0);
                }
            }
        }
    }



    fclose(file);
    return it;
}





// AUX DATA

auxData* newAuxData(networkParameters* np){
    int i;

    auxData* ad = malloc(sizeof(auxData));

    ad->link = calloc(1, np->n * sizeof(int));
    ad->channelAccess = calloc(1, np->n * sizeof(int));

    ad->sensorPositions = malloc(np->n * sizeof(double*));
    if(ad->sensorPositions == NULL){
        printf("ERROR - NOT ENOUGH MEMORY\n");
        exit(0);
    }
    for(i=0;i<np->n;i++){
        ad->sensorPositions[i] = calloc(1,3 * sizeof(double));
        if(ad->sensorPositions[i] == NULL){
            printf("ERROR - NOT ENOUGH MEMORY\n");
            exit(0);
        }
    }

    ad->APpositions = malloc(np->nAP * sizeof(double*));
    if(ad->APpositions == NULL){
        printf("ERROR - NOT ENOUGH MEMORY\n");
        exit(0);
    }
    for(i=0;i<np->nAP;i++){
        ad->APpositions[i] = calloc(1,3 * sizeof(double));
        if(ad->APpositions[i] == NULL){
            printf("ERROR - NOT ENOUGH MEMORY\n");
            exit(0);
        }
    }


    int SIZE = np->n + np->nAP;


    ad->pathLoss = malloc(SIZE * sizeof(long double*));
    if(ad->pathLoss == NULL){
        printf("ERROR - NOT ENOUGH MEMORY\n");
        exit(0);
    }
    for(i=0;i<SIZE;i++){
        ad->pathLoss[i] = calloc(1, SIZE * sizeof(long double));
        if(ad->pathLoss[i] == NULL){
            printf("ERROR - NOT ENOUGH MEMORY\n");
            exit(0);
        }
    }


    ad->interferenceLim = malloc(SIZE * sizeof(long double*));
    if(ad->interferenceLim == NULL){
        printf("ERROR - NOT ENOUGH MEMORY\n");
        exit(0);
    }
    for(i=0; i<SIZE; i++){
        ad->interferenceLim[i] = calloc(1,SIZE * sizeof(long double));
        if(ad->interferenceLim[i] == NULL){
            printf("ERROR - NOT ENOUGH MEMORY\n");
            exit(0);
        }
    }


    ad->interference = malloc(SIZE * sizeof(long double*));
    if(ad->interference == NULL){
        printf("ERROR - NOT ENOUGH MEMORY\n");
        exit(0);
    }
    for(i=0; i<SIZE; i++){
        ad->interference[i] = calloc(1, SIZE * sizeof(long double));
        if(ad->interference[i] == NULL){
            printf("ERROR - NOT ENOUGH MEMORY\n");
            exit(0);
        }
    }

    return ad;
}

void initializeAuxData(networkParameters* np, auxData** ad){
    int i;

    if(np->scenario == 0){ // reference
        // all sensors are connected to same AP; no need to calculate rest of auxData
        for(i=0; i<np->n; (*ad)->link[i++]=np->n+1);

    }else{ // building
        // calculates all auxData

        for(i=0; i<np->nDC; i++)
            (*ad)->channelAccess[i] = 0;
        for(i=0; i<np->nCSMA; i++)
            (*ad)->channelAccess[i+np->nDC] = 1;

        generateAPpositions(np, ad);
        generatePositions(np, ad);
        initializeLink(np, ad);
        initializePathLoss(np, ad);


        initializeInterferenceLim(np, ad);
        initializeInterference(np, ad);

    }

    return;
}

void initializePathLoss(networkParameters* np, auxData** ad){

    double PL;// path loss in dB
    double d_ij;// distance between devices i and j [m]
    double f = np->f;//frequency [MHz]
    double a = np->a;//path loss exponent
    int floors; //number of floors appart
    int i,j;

    int SIZE = np->n + np->nAP;

    // ----- criar uma matriz auxiliar que seja uma concatenacao das posiçoes dos sensores e APs

    double** aux = malloc(SIZE * sizeof(double*));
    for(i=0; i<SIZE; i++)
        aux[i] = calloc(1,3 * sizeof(double));

    for(i=0; i<SIZE; i++){
        for(j=0; j<3; j++){
            if(i<np->n){
                aux[i][j] = (*ad)->sensorPositions[i][j];
            }else{
                aux[i][j] = (*ad)->APpositions[i-np->n][j];
            }
        }
    }





    for(i=0; i < SIZE; i++){
        for(j=0; j < SIZE; j++){
            if(j!=i){


                //2D distance
                d_ij = sqrt(pow(aux[i][0]-aux[j][0],2)+pow(aux[i][1]-aux[j][1],2));
                // note: height is not considered in the current simulator implementation

                floors = 0;

                // path loss (dB)
                PL = 20*log10f(f) + a*10.0*log10f(d_ij) - 28;
                (*ad)->pathLoss[i][j] = PL;

            }
        }
    }



    for (i=0; i<SIZE; i++)
        free(aux[i]);
    free(aux);



    return;
}

void initializeInterferenceLim(networkParameters* np, auxData** ad){

    int i,j;
    long double th = pow(10,np->SINRmin/10); //minimum SINR (threshold) in lin units
    long double N = np->N0 + np->NF; // N = N0+NF
    long double n = pow(10,N/10); // n = n0*nf
    long double pt = pow(10,np->Pt/10); // transmitted power in lin units
    long double x;

    if(th<0 || pt<0){
        printf("ERROR!!! power function overflow\n");
        exit(0);
    }

    int SIZE = np->n+np->nAP;

    for(i=0;i<SIZE;i++){
        for(j=0;j<SIZE;j++){
            if(i!=j){


                x = (*ad)->pathLoss[i][j]/10;
                x = pow(10,x);
                if(x < 0){
                    printf("ERROR!!! power function overflow\n",x);
                    exit(0);
                }


                (*ad)->interferenceLim[i][j] = (pt/x)/th - n;

            }
        }
    }

    return;
}

void initializeInterference(networkParameters* np, auxData** ad){

    int i, j;
    long double pt = pow(10,np->Pt/10);
    long double x;

    if(pt<0){
        printf("ERROR!!! power function overflow\n");
        exit(0);
    }

    int SIZE = np->n+np->nAP;



    for(i=0;i<SIZE;i++){
        for(j=0;j<SIZE;j++){
            if(i!=j){

                x = (*ad)->pathLoss[i][j]/10;
                x = pow(10,x);

                if(x<0){
                    printf("ERROR!!! power function overflow\n");
                    exit(0);
                }

                (*ad)->interference[i][j] = pt/x;
            }
        }
    }
    return;
}


void initializeLink(networkParameters* np, auxData** ad){
    int i,j;
    double x, y, xAP, yAP, dmin, r, d;
    int AP;

    r = range(np); // range of APs (considered the same)


    for(i=0; i<np->n; i++){
        x = (*ad)->sensorPositions[i][0];
        y = (*ad)->sensorPositions[i][1];

        //initial value of distance, to determine minimum:
        dmin = distance2D(x, y, (*ad)->APpositions[0][0], (*ad)->APpositions[0][1]);
        AP = -1;

        for(j=0; j<np->nAP; j++){ //see distance to all APs
            xAP = (*ad)->APpositions[j][0];
            yAP = (*ad)->APpositions[j][1];

            d = distance2D(x, y, xAP, yAP);
            // if within range and closer than last distance
            if(d<=r && d<= dmin){
                // save AP and new min distance
                AP = j+1;
                dmin = d;
            }
        }
        if(AP != -1)
            (*ad)->link[i] = AP+np->n;
        else{
            (*ad)->link[i] = -1;
            printf("ERROR! One of the sensors is out of range\n");
            exit(0);
        }

    }

    return;
}



double range(networkParameters* np){
     double d;

     d = pow(np->f, -2/np->a) * pow(10, (np->Pt+28-np->Pdet)/(10*np->a));

    return d;
}



void printAuxData(auxData* ad, networkParameters* np){
    int i;

    if(np->scenario == 1){
        printf("\n-- AP Positions --\n");
        printMatrix(ad->APpositions, np->nAP, 3);

        printf("\nOut of range sensors: ");
        for(i=0; i<np->n; i++)
            if(ad->link[i] == -1)
                printf("%d ", i+1);
        printf("\n");

        printf("\n-- Device Positions --\n");
        printMatrix(ad->sensorPositions, np->n, 3);


        printf("\n-- Range --\n");
        printf("%f\n",range(np));

        printf("\n-- pathLoss (lin) --\n");
        printMatrixWidth(ad->pathLoss, np->n, np->n);

        printf("\n-- interferenceLim(lin) --\n");
        printMatrixWidth(ad->interferenceLim, np->n, np->n);

        printf("\n-- interferenceLim(dB) --\n");
        printMatrixdB(ad->interferenceLim, np->n, np->n);

        printf("\n-- interference(lin) --\n");
        printMatrixWidth(ad->interference, np->n, np->n);

        printf("\n-- interference(dB) --\n");
        printMatrixdB(ad->interference, np->n, np->n);
    }

    return;
}

double distance2D(double x1, double y1, double x2, double y2){
    double d;

    d = sqrt(pow(x1-x2,2) + pow(y1-y2,2));
    return d;

}



void generateAPpositions(networkParameters* np, auxData** ad){
    int i,j,k = 0;
    int div = sqrt(np->nAP);
    double x,y;

    for(i=0; i<div; i++){
        for(j=0; j<div; j++){
            x = i*np->lx/div + np->lx/div/2;
            (*ad)->APpositions[k][0] = x;


            y = random()*np->ly/div/2 + j*np->ly/div + np->ly/div/4;
            (*ad)->APpositions[k][1] = y;
            // z is considered zero (only one floor)
            k++;
        }
    }

    return;
}


int withinRange(int sensor, networkParameters* np, auxData** ad){
    int AP;
    double r;
    double d, xAP, yAP;

    double x = (*ad)->sensorPositions[sensor][0];
    double y = (*ad)->sensorPositions[sensor][1];



    r = range(np); // range of APs (considered the same)

    for(AP=0; AP<np->nAP; AP++){

        xAP = (*ad)->APpositions[AP][0];
        yAP = (*ad)->APpositions[AP][1];

        d = distance2D(x, y, xAP, yAP);
        // if within range return 1
        if(d<=r){
            return 1;
        }

    }

    // is not within range of any AP
    return 0;

}


void generatePositions(networkParameters * np, auxData** ad){
    int i;
    double r;
    double a1, a2, b1, b2, x, y;


    for(i=0; i<np->n; i++){
        while(1){
            (*ad)->sensorPositions[i][0] = random()*np->lx;
            (*ad)->sensorPositions[i][1] = random()*np->ly;
            (*ad)->sensorPositions[i][2] = 0; // building só tem um andar

            if(withinRange(i, np, ad))
                break;
        }

    }
    return;
}



double floorLoss(int z){ // unused function
    switch(z){
        case(0):
            return 0;
            break;
        case(1):
            return 9;
            break;
        case(2):
            return 19;
            break;
        case(3):
            return 24;
            break;
    }
    return 0;
}



// Free structures

void freeAll(networkParameters* np, flags** f, counters** c, auxData** ad){
    int i;
    int SIZE;

    // flags

    free((*f)->listening);
    freeActiveList(&((*f)->listeningList));
    free((*f)->transmittingTo);
    freeActiveList(&((*f)->transmittingToList));
    free((*f)->receiving);
    freeActiveList(&((*f)->receivingList));

    free((*f)->waitingACK);
    free((*f)->transmittedLastPacket);
    free((*f)->collision);
    free((*f)->channelBusySince);
    free((*f)->currentPid);
    free((*f)->nextCycleStart);
    free((*f)->interferenceSum);
    free((*f));

    // counters

    free((*c)->applicationPackets);
    free((*c)->offeredPackets);
    free((*c)->lostPacketsCollision);
    free((*c)->lostPacketsBuffer);
    free((*c)->transmittedPackets);
    free((*c)->successfulPackets);
    free((*c)->receivedACKs);
    free((*c)->rescheduledPackets);
    free((*c)->rescheduledPacketsListen);
    free((*c)->transmittedACKs);
    free((*c)->lostACKs);
    free((*c));

    // auxData

    free((*ad)->link);
    free((*ad)->channelAccess);

    for(i=0; i<np->n; i++)
        free((*ad)->sensorPositions[i]);
    free((*ad)->sensorPositions);

    for(i=0; i<np->nAP; i++)
        free((*ad)->APpositions[i]);
    free((*ad)->APpositions);


    SIZE = np->n; // provisorio


    for(i=0; i<SIZE; i++)
        free((*ad)->pathLoss[i]);
    free((*ad)->pathLoss);


    for(i=0; i<SIZE; i++)
        free((*ad)->interferenceLim[i]);
    free((*ad)->interferenceLim);


    for(i=0; i<SIZE; i++)
        free((*ad)->interference[i]);
    free((*ad)->interference);


    free((*ad));


    return;
}





// --------------- Printing functions (for debug) ---------------- //


void printActive(int* array, int n){
    int i;
    for(i=0; i<n; i++){
        if(array[i] > 0){
            printf("%d->%d | ",i+1,array[i]);
        }
    }
    printf("\n");
    return;
}



void printFlags(flags* f, networkParameters* np){
     int i;

     printf("-- flags --\n");
     printf("transmitted lastcycle:"); printActive(f->transmittedLastPacket,np->n);
     //printf("listening:            "); printActive(f->listening,np->n);
     printf("listeningList         "); printActiveNodeList(f->listeningList);

     //printf("transmittingTo:       "); printActive(f->transmittingTo,np->n);
     printf("transmittingToList    "); printActiveNodeList(f->transmittingToList);

     //printf("receivingfrom:        "); printActive(f->receivingFrom,np->n);
     printf("receivingfromList     "); printActiveNodeList(f->receivingList);

     printf("current Pid:          "); printActive(f->currentPid,np->n);
     printf("waitingACK:           "); printActive(f->waitingACK, np->n);
     printf("collision:            "); printActive(f->collision, np->n);
     printf("interferenceSum[dB]:  ");
     for(i=0;i<np->n;i++){
        if(f->interferenceSum[i]>0)
            printf("%d->%.2f  ",i+1,10.0*log10(f->interferenceSum[i]));
     }

     printf("\n");
     return;
}



void printCounters(counters* c, networkParameters* np){

     printf("-- counters --");

     printf("\nofferedPackets        "); printArrayInt(c->offeredPackets,np->n);
     printf("\nrescheduledPackets    "); printArrayInt(c->rescheduledPackets, np->n);
     printf("\ntransmittedpackets    "); printArrayInt(c->transmittedPackets,np->n);
     printf("\nsuccessfulPackets     "); printArrayInt(c->successfulPackets, np->n);
     printf("\nlostpacketscollision  "); printArrayInt(c->lostPacketsCollision,np->n);
     printf("\nlostPacketsBuffer     "); printArrayInt(c->lostPacketsBuffer, np->n);

     printf("\n");
     printf("\ntransmittedACKs       "); printArrayInt(c->transmittedACKs,np->n);
     printf("\nreceivedACKs          "); printArrayInt(c->receivedACKs, np->n);
     printf("\nlostACKs              "); printArrayInt(c->lostACKs,np->n);

     printf("\n\n");

     return;
}



void printNetworkParameters(networkParameters* np){
    printf("\n----- Simulation settings -----\n");
    printf("Nsimulations = %d\n",np->Nsimulations);
    printf("Tmax = %.2f ms\n",np->Tmax);
    printf("n = %d devices\n",np->n);
    printf("nAP = %d access points\n", np->nAP);
    if(np->scenario == 0){
        printf("Scenario: REFERENCE (all nodes can hear each other)\n");
    }else if(np->scenario == 1){
        printf("Scenario: BUILDING\n");
    }

    printf("\n--- device parameters --- \n");
    printf("DC_DC = %0.2f %\n", np->DCDC);
    printf("DC_CSMA = %0.2f %\n", np->DCCSMA);

    printf("Tp_DC = %0.2f ms\n", np->TpDC);
    printf("Tp_CSMA = %0.2f ms\n", np->TpCSMA);
    printf("Tcycle_DC = %0.2f ms\n", np->TcycleDC);
    printf("Tcycle_CSMA = %0.2f ms\n", np->TcycleCSMA);
    printf("Toffset_DC (maximum) = %0.2f ms\n", np->ToffsetDC);
    printf("Toffset_CSMA (maximum) = %0.2f ms\n", np->ToffsetCSMA);

    printf("\n------- CSMA -------\n");
    printf("TL = %0.2f ms\n", np->TL);
    printf("TD = %0.2f ms\n", np->TD);
    printf("TR = %0.2f ms\n", np->TR);
    printf("Trep (mean) = %0.2f ms\n", np->Trep);

    if(np->ACK_switch == 1){
        printf("\n------- ACK -------\n");
        if(np->Tresp+np->TACK < np->TL-np->TR+np->TD)
            printf("- ACKs are received within safe zone\n");
        else
            printf("- ACKs are received OUTSIDE safe zone\n");
        printf("TtimeoutACK = %0.2f ms\n", np->TtimeoutACK);
        printf("Tresp = %0.2f ms\n", np->Tresp);
        printf("Ttimeoutrep (mean) = %0.2f ms\n", np->Ttimeoutrep);
        printf("TACK = %0.2f ms\n", np->TACK);
    }else
        printf("\n---- ACK deactivated ----\n");

    if(np->scenario == 1){


        printf("\n-- Physical parameters --\n");
        printf("B = %0.2f kHz\n", np->B);
        printf("Pt = %0.2f dBm\n", np->Pt);
        printf("N0 = %0.2f dBm\n", np->N0);
        printf("NF = %0.2f dBm\n", np->NF);
        printf("SINRmin = %0.2f dB\n", np->SINRmin);
        printf("Pdet = %0.2f dBm\n", np->Pdet);

        printf("\nRange = %.2f m\n", range(np));
        if(range(np)>sqrt(pow(np->lx,2)+pow(np->ly,2)))
            printf("- all devices can hear each other within same floor\n");

        printf("\n-- Scenario --\n");
        printf("lx = %0.2f m\n",np->lx);
        printf("ly = %0.2f m\n",np->ly);
        printf("lz = %0.2f m\n",np->lz);
        printf("z = %d floors\n",np->z);
        printf("f = %0.2f MHz\n", np->f);
        printf("a = %0.2f\n", np->a);
    }

    printf("\n\n");

    return;
}

void printIterations(iterationParameters* it){
    printf("-------- Iteration values --------");
    if(it->nn > 1){
        printf("\nn = ");
        printArrayInt(it->n, it->nn);
    }if(it->Nsimulations > 1){
        printf("\nNsimulations = ");
        printArrayInt(it->Nsimulations, it->n2);

    }
    if(it->nPt > 1){
        printArrayInt(it->Pt, it->nPt);
    }

    printf("\n\n\n");
    return;
}

void printProgress(double isim, double Nsimul, int* x, counters* c){
    if(isim/Nsimul > 0.10 && (*x) == 0){
        printf("10 -- ");
        (*x)++;

    }else if(isim/Nsimul > 0.25 && (*x) == 1){
        printf("25 -- ");
        (*x)++;

    }else if(isim/Nsimul > 0.50 && (*x) == 2){
        printf("50 -- ");
        (*x)++;

    }else if(isim/Nsimul > 0.75 && (*x) == 3){
        printf("75 -- \n\n");
        (*x)++;

    }
    return;
}


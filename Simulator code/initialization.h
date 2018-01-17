#ifndef DC_H_INCLUDED
#define DC_H_INCLUDED


typedef struct networkParameters{
    int Nsimulations;
    double Tmax; // simulation time
    int scenario; //simulation scenario type (0-ref; 1-building)
    int n; //number of nodes
    int nDC;
    int nCSMA;
    int nAP; //number of access points

    //environment:
    double lx,ly,lz; //building dimentions
    int z; //number of floors
    double f; //frequency
    double a; //path loss exponent

    //devices:
    double DCCSMA; //duty cycle [%]
    double DCDC;
    double TpCSMA; //packet length
    double TpDC;
    double TcycleDC; //distance between generation of packets in the application layer
    double TcycleCSMA;
    double ToffsetCSMA; //random offset for initializing packet transmission
    double ToffsetDC;
    double B;
    double Pt;
    double N0;
    double NF;

    //ACK
    int ACK_switch;
    double TtimeoutACK;
    double Tresp;
    double Ttimeoutrep; //mean retrial interval after no ACK is received
    double TACK;

    //CSMA
    double Pdet; //listening detection threshold
    double TL; //listen time
    double TD; //dead time
    double TR;
    double Trep; //mean transmission retrial interval
    double SINRmin;
} networkParameters;

typedef struct flags{

    int* listening; //not used
    activeNode* listeningList;
    int* transmittingTo; //not used
    activeNode* transmittingToList;
    int* receiving; //not used
    activeNode* receivingList;

    int* waitingACK;
    int* transmittedLastPacket;
    int* collision;
    long double* interferenceSum;
    double* channelBusySince;
    int* currentPid;
    double* nextCycleStart;

} flags;



typedef struct counters{

    int* applicationPackets;
    int* offeredPackets;
    int* transmittedPackets;
    int* lostPacketsBuffer;
    int* lostPacketsCollision;
    int* receivedACKs;
    int* successfulPackets;
    int* rescheduledPackets;
    int* rescheduledPacketsListen;
    int* transmittedACKs;
    int* lostACKs;

} counters;




typedef struct auxData{

    int* link;
    int* channelAccess;
    double** sensorPositions;
    double** APpositions;

    long double** pathLoss; // dB
    long double** interferenceLim; // linear units
    long double** interference; // linear units

} auxData;




typedef struct iterationParameters{
    int* Pt;
    int nPt;

    int nTsim;
    int* Tsim;

    int* n; //values to iterate
    int nn; //number of iterations

    int* Nsimulations;
    int n2;
    int maxNsimulations;


} iterationParameters;






// Create structures
networkParameters* newNetwork();
flags* newFlags(networkParameters* np);
counters* newCounters(networkParameters* np);
iterationParameters* newIteration();


// Print structures
void printFlags(flags* f, networkParameters* np);
void printCounters(counters* c, networkParameters* np);
void printNetworkParameters(networkParameters* nw);
void printCountersAverage(counters*c, networkParameters* np);
void printProgress(double isim, double Nsimul, int* x, counters* c);


// Auxiliary functions
void printIterations(iterationParameters* it);
double random();
int maxInt(int* arr, int n);



// Main initialization functions
iterationParameters* initializeIterationParameters(char* f);
networkParameters* initializeParameters(char* f);



// Free structures
void freeAll(networkParameters* np, flags** f, counters** c, auxData** ad);



// ----------------------- auxData ---------------------------- //

// Create structures
auxData* newAuxData(networkParameters* np);

// Print
void printAuxData(auxData* ad, networkParameters* np);

// Scenario-related functions
double range(networkParameters* np);
double distance2D(double x1, double y1, double x2, double y2);
void generatePositions(networkParameters * np, auxData** ad);
double floorLoss(int z);
void initializePathLoss(networkParameters* np, auxData** ad);
void initializeInterferenceLim(networkParameters* np, auxData** ad);
void initializeInterference(networkParameters* np, auxData** ad);




#endif // DC_H_INCLUDED

#ifndef RESULTS_H_INCLUDED
#define RESULTS_H_INCLUDED

typedef struct results{

    // Results
    double** G_DC;
    double* avG_DC;
    double* varG_DC;

    double** G_CSMA;
    double* avG_CSMA;
    double* varG_CSMA;

    double** S_DC;
    double* varS_DC;
    double* avS_DC;

    double** S_CSMA;
    double* varS_CSMA;
    double* avS_CSMA;

    double** PLRBuffer_DC;
    double* avPLRBuffer_DC;
    double* varPLRBuffer_DC;

    double** PLRBuffer_CSMA;
    double* avPLRBuffer_CSMA;
    double* varPLRBuffer_CSMA;

    double** PLRCollisions_DC;
    double* avPLRCollisions_DC;
    double* varPLRCollisions_DC;

    double** PLRCollisions_CSMA;
    double* avPLRCollisions_CSMA;
    double* varPLRCollisions_CSMA;

    double** ACKlossRate;
    double* avACKlossRate;
    double* varACKlossRate;

    double** transmissionRate;
    double* avTransmissionRate;
    double* varTransmissionRate;

    // will use these for other results:

    double* avPLRCollisions2;
    double* varPLRCollisions2;
    double** channelLoad;
    double* avChannelLoad;
    double* varChannelLoad;




    // Counters average
    double* avApplicationPackets;
    double* avOfferedPackets;
    double* avTransmittedPackets;
    double* avLostPacketsBuffer;
    double* avLostPacketsCollision;
    double* avReceivedACKs;
    double* avSuccessfulPackets;
    double* avRescheduledPackets;
    double* avTransmittedACKs;
    double* avLostACKs;
    double* avRescheduledPacketsListen;


} results;

// Create
results* newResults(iterationParameters* it, networkParameters* np);


// Output
void checkOutputFile(networkParameters* np, char* f);
void outputNetworkParameters(FILE* file, networkParameters* np);
void saveResults(networkParameters* np, results** r, counters* c, int k, int i);
void outputResults(results* r, iterationParameters* it, networkParameters* np, char* f);
void outputCountersAverage(networkParameters* np, node* eventList, counters*c);
void outputCounters(networkParameters* np, counters* c, iterationParameters* it, char* f, auxData *ad);
void outputAuxData(networkParameters* np, auxData* ad, char* f);



#endif // RESULTS_H_INCLUDED

//**********************************************************//
// Author: Rita Bandeira
// Last modified: February 2017
//**********************************************************//


#include <stdio.h>
#include <stdlib.h>
#include <locale.h>

#include "list.h"
#include "listActive.h"
#include "matrixes.h"
#include "initialization.h"
#include "CSMA.h"
#include "results.h"


int main()
{
    setlocale(LC_NUMERIC, "French_Canada.1252");
    int i, a = 0, b = 0, nr = 0;
    double Tsim = 0;
    flags* f;
    counters* c;
    results* r;
    auxData* ad;
    node* eventList;




    // ------------ Initialize network parameters from file ----------//

    // Network Parameters
    networkParameters* np = initializeParameters("Input/initialization_CSMA.txt");
    printNetworkParameters(np);

    // Iteration Parameters
    iterationParameters* it = initializeIterationParameters("Input/iterations_CSMA.txt");
    printIterations(it);

    // Check if output file is available
    char resultsFile[1024];
    sprintf(resultsFile, "Results/Results.csv");
    checkOutputFile(np, resultsFile);
    char detResultsFile[1024];
    sprintf(detResultsFile, "Results/Detailed Results.csv");
    checkOutputFile(np, resultsFile);
    char countersFile[1024];
    sprintf(countersFile, "Results/Counters.csv");
    checkOutputFile(np, countersFile);


    // ------------ Initialize support structures -------------------//

    // Results
    r = newResults(it, np);


    // -------------------- set iterations ---------------------//

    int iPt; // current Pt iteration number

    for(iPt=0; iPt<it->nPt; iPt++){
        if(iPt > 1)
            np->Pt = it->Pt[iPt];

        printf("---- SIMULATING FOR Pt = %f -----\n", np->Pt);

        int in; // current n iteration number
        int isim; // current simulation number (to reach Nsimulations)

        for(in = 0; in<it->nn; in++){
            int x = 0;

            if(it->nn > 1){ //there are values to iterate
                np->nCSMA = it->n[in];
                np->n = np->nCSMA + np->nDC;

                if(it->n2 > 1){ // is there a correspondent Nsimulations value?
                    np->Nsimulations = it->Nsimulations[in];
                    np->Tmax = it->Tsim[in]*np->TcycleCSMA;
                }
                printf("\n------ SIMULATING %d TIMES FOR n = %d (Tsim = %f) ------\n\n", np->Nsimulations, np->n, np->Tmax);
            }



            for(isim = 0; isim<np->Nsimulations; isim++){
                // ----------------- initialization -------------------//
                // Event linked list
                eventList = newList();

                // Flags
                f = newFlags(np);

                // Counters
                c = newCounters(np);

                // Auxiliary data
                ad = newAuxData(np);

                initializeAuxData(np, &ad);

                //outputAuxData(np, ad, "Results/auxData.csv");

                // enqueue first StartListen event for each node
                initializeStartListen(np, &eventList, &f, &c, &ad);


                printProgress(isim, np->Nsimulations, &x, c);

                // ------------------------- start simulation ---------------------------//

                nr = 0;
                Tsim = 0;


                double aux1 = 0, aux2 = 0;

                while(Tsim < np->Tmax){

                    nr++;

                    if(isEmpty(eventList)){
                        printf("\nEventList is empty at Tsim = %0.2f\n", Tsim);
                        exit(0);
                    }
                    if(eventList->Te < Tsim){
                        printf("\nWhoooa, we're goin' back in time at Tsim = %lf (Te = %lf), nr = %d!\n", Tsim, eventList->Te, nr);
                        exit(0);
                    }

                    Tsim = eventList->Te;
                    //printProgress(Tsim, np->Tmax, &x, c);


                    switch(eventList->eventType){
                        case(0):
                            startListen(np, &eventList, &f, &c, &ad);
                            break;
                        case(1):
                            endListen(np, &eventList, &f, &c, &ad);
                            break;
                        case(2): // data packet
                            newPacket(0, np, &f, &ad, &c, &eventList);
                            break;
                        case(3): // ACK
                            newPacket(1, np, &f, &ad, &c, &eventList);
                            break;
                        case(4): // data packet
                            endPacket(0, np, &f, &ad, &c, &eventList);
                            break;
                        case(5): // ACK
                            endPacket(1, np, &f, &ad, &c, &eventList);
                            break;
                        case(6):
                            ACKTimeOut(np, &f, &c, &ad, &eventList);
                            break;
                        case(7):
                            newPacketDC(0, np, &f, &ad, &c, &eventList);
                            break;
                        case(8):
                            endPacketDC(0, np, &f, &ad, &c, &eventList);
                            break;
                    }

                    dequeue(&eventList);


                    if(printCondition(eventList, np)){
                        printf("\n");
                        //printCounters(c, np);
                        printFlags(f, np);
                    }
                }


                // -------------------------- Results -------------------------//

                sumInt(c->lostPacketsBuffer, c->receivedACKs, c->applicationPackets, np->n, 1);


                //printCounters(c, np);
                //printCountersAverage(c, np);
                //outputCounters(np, c, it, countersFile, ad);

                saveResults(np, &r, c, isim, in);






                // -------------------------- Reset simulation -------------------------//

                freeEventList(&eventList);
                freeAll(np, &f, &c, &ad);
            }
        }



        calculateVariance(&r, it, np);
        outputResults(r, it, np, resultsFile);
        //outputDetailedResults(r, it, np, detResultsFile);


    }

    printf("\n\nFINISHED!\n");

    return 0;
}

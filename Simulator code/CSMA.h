#ifndef CSMA_H_INCLUDED
#define CSMA_H_INCLUDED

int printCondition(node* eventList, networkParameters* np);



void startListen(networkParameters* np, node** eventList, flags** f, counters** c, auxData** ad);
void endListen(networkParameters* np, node** eventList, flags** f, counters **c, auxData** ad);
void newPacket(int packetType, networkParameters* np, flags** f, auxData** ad, counters** c, node** eventList);
void endPacket(int packetType, networkParameters* np, flags** f, auxData** ad, counters** c, node** eventList);
void ACKTimeOut(networkParameters* np, flags** f, counters** c, auxData** ad, node** eventList);

void initializeStartListen(networkParameters* np, node** eventList, flags** f, counters** c, auxData** ad);
void updateCycleInfo(int T, int R, double Te, networkParameters* np, flags** f, counters** c, auxData** ad, node** eventList);
void addInterference(int T, int R, double Te, networkParameters* np, auxData** ad, flags** f, node** eventList);
void subtractInterference(int T, double Te, networkParameters* np, auxData** ad, flags** f, node** eventList);
void listenChannel(int T, double Te, networkParameters* np, auxData** ad, flags** f, node** eventList);

#endif // CSMA_H_INCLUDED

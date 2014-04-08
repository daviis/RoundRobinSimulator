//Written by Isaac Davis
#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <stdlib.h>
#include <sstream>
#include <queue>
#include "program.h"

using namespace std;

queue<int>* breakLine(string);
vector<Program*>* runSimulation(queue<Program*>*, int, int*, int);
void printStates(queue<Program*>, queue<Program*>);
void checkIOQueue(queue<Program*>*, queue<Program*>*, int);
void incWaitCount(queue<Program*>*);


int main(int argc, char* argv[]){
	//some config stuff

	int timeQuantum = 3;
	int numIoDevices = 1;
	int maxNumIoDevices = 10;
	while(numIoDevices < maxNumIoDevices){
//		cout << "\tTimeQuantum : " << timeQuantum << endl;
		cout << "\tIoDevices: " << numIoDevices << endl;
		
		int idleTime = 0;	
		queue<Program*>* progQueue = new queue<Program*>;
	
		//open the file
		fstream simFile;
		simFile.open("progData.dat");
		string line;

		//read the file and make program objects out of it	
		int idCount = 1;
		while(getline(simFile, line)){
			queue<int>* clockCounts = breakLine(line);
			progQueue->push(new Program(clockCounts, idCount));
			idCount++;
		}
		//hacky way to set the number of times through the while loop correctly
		maxNumIoDevices = idCount;
		vector<Program*>* finalCounts = runSimulation(progQueue, timeQuantum, &idleTime, numIoDevices);	
		
		//final output
		bool indv = false;
		if(indv){
			for(vector<Program*>::iterator it = finalCounts->begin(); it != finalCounts->end(); ++it){	
				Program* p = *it;	
				cout << p->progId << " = wait : " << p->wc() << " turnaround time : " << p->rc() << endl;
			}
		}
		else{
			long totalWait = 0;
			long totalIOWait = 0;
			long totalTurn = 0;
			for(vector<Program*>::iterator it = finalCounts->begin(); it != finalCounts->end(); ++it){	
				Program* p = *it;
				totalWait += p->wc();
				totalIOWait += p->ioWC();
				totalTurn += p->rc();
			}
			cout << totalWait / idCount <<  " : average wait" << endl;
			cout <<totalTurn / idCount << " : average turnAround" << endl;
			cout << totalIOWait /idCount << " : average ioWait" << endl;	
		}
	
		cout << idleTime << " idle cycles in system" << endl;
		//timeQuantum++;
		numIoDevices++;
	}
	return 0;
}

vector<Program*>* runSimulation(queue<Program*>* runQueue, int quant, int* idle, int ioDev){
	bool doPrint = false;
	vector<Program*>* outputVec = new vector<Program*>;
	queue<Program*>* ioQue = new queue<Program*>;
	while(!runQueue->empty() || !ioQue->empty()){
		int currClock = 0;
		if(runQueue->empty()){
			while(runQueue->empty()){
				*idle += 1;	
				queue<Program*> printQueue (*runQueue);
				queue<Program*> ioQueue (*ioQue);
				checkIOQueue(runQueue, ioQue, ioDev);
				if(doPrint){
					printStates(printQueue, ioQueue);
				}
			}
		}
		Program* runningProg = runQueue->front();
		int lifeLength = runningProg->lifeLength();
		if(doPrint){
			cout << runningProg->progId << " enters running state" << endl;
		}
		runQueue->pop();
		while(currClock < quant && runningProg->incRC()){
			incWaitCount(runQueue);
			queue<Program*> printQueue (*runQueue);
			queue<Program*> ioQueue (*ioQue);
			checkIOQueue(runQueue, ioQue, ioDev);
			if(doPrint){
				cout << runningProg->progId << " is running"  << endl;
				printStates(printQueue, ioQueue);
			}
			currClock++;
		}
		if(!runningProg->exited()){
			if(runningProg->needsIO(lifeLength)){
				ioQue->push(runningProg);
			}
			else{
				runQueue->push(runningProg);
			}
		}
		else{
			if(doPrint){
				cout << runningProg->progId << " is done after " << runningProg->rc() << " cycles"<< endl;
			}
			outputVec->push_back(runningProg);
		}
	}
	return outputVec;
}

void checkIOQueue(queue<Program*>* runQ, queue<Program*>* ioQ, int numIoDev){
	queue<Program*>* tempQ = new queue<Program*>;
	//only use the right number of io devices
	//for(int i = 0; i < numIoDev; i++){
	int i = 0;
	while(!ioQ->empty() && i < numIoDev){
		Program* p = ioQ->front();
		ioQ->pop();
		if(p->incIOC()){
			tempQ->push(p);
		}
		else{
			runQ->push(p);
		}
		i++;	
	}
	while(!ioQ->empty()){
		Program* p = ioQ->front();
		ioQ->pop();
		p->incIOWC();
		tempQ->push(p);
	}
/*
	while(!ioQ->empty()){
		Program* p = ioQ->front();
		ioQ->pop();
		if(p->incIOC()){
			tempQ->push(p);
		}
		else{
			runQ->push(p);
		}	
	}
*/
	//repack the ioQ to the same state it was when it came in
	while(!tempQ->empty()){
		Program* p = tempQ->front();
		tempQ->pop();
		ioQ->push(p);
	}
}

void incWaitCount(queue<Program*>* runQ){
	queue<Program*>* tempq = new queue<Program*>;
	while(!runQ->empty()){
		Program* p = runQ->front();
		runQ->pop();
		tempq->push(p);
		p->incWC();
		p->incRC_notRunning();
	}
	while(!tempq->empty()){
		Program* p = tempq->front();
		tempq->pop();
		runQ->push(p);
	}
}

void printStates(queue<Program*> printQ, queue<Program*> ioQ){
	while(!printQ.empty()){
		Program* p = printQ.front();
		printQ.pop();	
		cout << "\t\t" << p->progId << " is waiting" << endl;
	}
	while(!ioQ.empty()){
		Program* p = ioQ.front();
		ioQ.pop();
		cout << '\t' << p->progId << " is doing io : ioCount " << p->ioC() << endl;
	}
}

queue<int>* breakLine(string line){
	char aChar = ' ';
	istringstream linestream(line);
	queue<int>* clockVec = new queue<int>;
	string intBuff = "";
	while(linestream >> noskipws >> aChar){
		if(aChar != ' '){
			intBuff += (aChar);
		}
		else{
			int aNum = atoi(intBuff.c_str());
			clockVec->push(aNum);
			intBuff = "";
		}
	}
	clockVec->push(atoi(intBuff.c_str()));
	return clockVec;
}

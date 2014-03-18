//Written by Isaac Davis
#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <stdlib.h>
#include <sstream>
#include <queue>

using namespace std;

class Program{	
	int runCycles, waitCycles, ioCycles;
	queue<int>* lifeCycle;
    public:
	Program(queue<int>*, int);
	bool incRC();
	void incRC_notRunning();
	void decRC();	
	void incWC();
	bool incIOC();
	int progId;
	bool exited();
	int lifeLength();
	bool needsIO(int);
	int wc();
	int rc();	
	queue<int>* getLC();
};

Program::Program(queue<int>* aQue, int id){
	runCycles = 0;
	waitCycles = 0;
	ioCycles = 0;
	lifeCycle = aQue;
	progId = id;
}

int Program::wc(){
	return this->waitCycles;
}

int Program::rc(){
	return this->runCycles;
}

void Program::incWC(){
	this->waitCycles++;
}

void Program::decRC(){
	this->runCycles--;
}
queue<int>* Program::getLC(){
	return this->lifeCycle;
}

int Program::lifeLength(){
	return this->lifeCycle->size();
}

bool Program::needsIO(int previous){
	return (previous != this->lifeLength());
}

void Program::incRC_notRunning(){
	this->runCycles++;
}

bool Program::incRC(){
	int val = this->lifeCycle->front();
	if(val > 0){
		this->runCycles++;
		this->lifeCycle->front()--;
		return true;
	}
	else{
		this->lifeCycle->pop();
		return false;	
	}	
}

bool Program::incIOC(){
	int val = this->lifeCycle->front();
	this->runCycles++;
	if(val > 1){
		this->lifeCycle->front()--;
		return true;
	}
	else{
		this->lifeCycle->pop();
		return false;	
	}	
}

bool Program::exited(){
	return (this->lifeCycle->empty() || (this->lifeCycle->size() == 1 && this->lifeCycle->front() == 0));
}

queue<int>* breakLine(string);
vector<Program*>* runSimulation(queue<Program*>*, int, int*);
void printStates(queue<Program*>, queue<Program*>);
void checkIOQueue(queue<Program*>*, queue<Program*>*);
void incWaitCount(queue<Program*>*);


int main(int argc, char* argv[]){
	//some config stuff
	int timeQuantum = 10;
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
	vector<Program*>* finalCounts = runSimulation(progQueue, timeQuantum, &idleTime);	
	
	//final output
	for(vector<Program*>::iterator it = finalCounts->begin(); it != finalCounts->end(); ++it){	
		Program* p = *it;	
		cout << p->progId << " wait " << p->wc() << endl;
	}	
	cout << idleTime << " idle cycles in system" << endl;
	return 0;
}

vector<Program*>* runSimulation(queue<Program*>* runQueue, int quant, int* idle){
	vector<Program*>* outputVec = new vector<Program*>;
	queue<Program*>* ioQue = new queue<Program*>;
	while(!runQueue->empty() || !ioQue->empty()){
		int currClock = 0;
		if(runQueue->empty()){
			while(runQueue->empty()){
				*idle += 1;	
				queue<Program*> printQueue (*runQueue);
				queue<Program*> ioQueue (*ioQue);
				printStates(printQueue, ioQueue);
				checkIOQueue(runQueue, ioQue);
			}
		}
		Program* runningProg = runQueue->front();
		int lifeLength = runningProg->lifeLength();
		cout << runningProg->progId << " enters running state" << endl;
		runQueue->pop();
		while(currClock < quant && runningProg->incRC()){
			incWaitCount(runQueue);
			cout << runningProg->progId << " is running"  << endl;
			queue<Program*> printQueue (*runQueue);
			queue<Program*> ioQueue (*ioQue);
			checkIOQueue(runQueue, ioQue);
			printStates(printQueue, ioQueue);
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
			cout << runningProg->progId << " is done after " << runningProg->rc() << " cycles"<< endl;
			outputVec->push_back(runningProg);
		}
	}
	return outputVec;
}

void checkIOQueue(queue<Program*>* runQ, queue<Program*>* ioQ){
	queue<Program*>* tempQ = new queue<Program*>;
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
		cout << '\t' << p->progId << " is doing io" << endl;
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

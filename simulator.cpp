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
	bool incWC();
	bool incIOC();
	int progId;	
};

Program::Program(queue<int>* aQue, int id){
	runCycles = 0;
	waitCycles = 0;
	ioCycles = 0;
	lifeCycle = aQue;
	progId = id;
}

bool Program::incRC(){
	
}

queue<int>* breakLine(string);
vector<Program*>* runSimulation(queue<Program*>*, int);
void printStates(queue<Program*>, queue<Program*>);

int main(int argc, char* argv[]){
	//some config stuff
	int timeQuantum = 10;	
	queue<Program*>* progQueue = new queue<Program*>;
	

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
	vector<Program*>* finalCounts = runSimulation(progQueue, timeQuantum);	
	
	return 0;
}

vector<Program*>* runSimulation(queue<Program*>* runQueue, int quant){
	vector<Program*>* outputVec = new vector<Program*>;
	queue<Program*>* ioQue = new queue<Program*>;
	while(!runQueue->empty()){
		int currClock = 0;
		Program* runningProg = runQueue->front();
		cout << runningProg->progId << " enters running state" << endl;
		runQueue->pop();
		while(currClock < quant && runningProg->incRC()){
			//inc waitCount for all in queue; 
			cout << runningProg->progId << " is running" << endl;
			queue<Program*> printQueue (*runQueue);
			queue<Program*> ioQueue (*ioQue);
			printStates(printQueue, ioQueue);
			currClock++;
		}
	}
	return outputVec;
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

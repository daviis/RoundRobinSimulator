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
	Program(queue<int>*);
	bool incRC();
	bool incWC();
	bool incIOC();	
};

Program::Program(queue<int>* aQue){
	runCycles = 0;
	waitCycles = 0;
	ioCycles = 0;
	lifeCycle = aQue;
}

bool Program::incRC(){
	
}

queue<int>* breakLine(string);
vector<Program*>* runSimulation(queue<Program*>*, int);


int main(int argc, char* argv[]){
	//some config stuff
	int timeQuantum = 10;	
	queue<Program*>* progQueue = new queue<Program*>;
	

	fstream simFile;
	simFile.open("progData.dat");
	string line;

	//read the file and make program objects out of it	
	while(getline(simFile, line)){
		queue<int>* clockCounts = breakLine(line);
		progQueue->push(new Program(clockCounts));
	}
	vector<Program*>* finalCounts = runSimulation(progQueue, timeQuantum);	
	
	return 0;
}

vector<Program*>* runSimulation(queue<Program*>* runQueue, int quant){
	vector<Program*>* outputVec = new vector<Program*>;
	while(!runQueue->empty()){
		int currClock = 0;
		Program* runningProg = runQueue->front();
		runQueue->pop();
		while(currClock < quant && runningProg->incRC()){
			//inc waitCount for all in queue; 
			currClock++;
			cout << currClock << endl;
		}
	}
	return outputVec;
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

//Written by Isaac Davis
#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <stdlib.h>
#include <sstream>

using namespace std;

class Program{	
	int runCycles, waitCycles, ioCycles;
	vector<int> lifeCycle;
    public:
	Program(vector<int>);
	void incRC();
	void incWC();
	void incIOC();	
};

Program::Program(vector<int> aVec){
	runCycles = 0;
	waitCycles = 0;
	ioCycles = 0;
}

vector<int>* breakLine(string);

int main(int argc, char* argv[]){
	fstream simFile;
	simFile.open("progData.dat");
	string line;

	//read the file and make program objects out of it	
	while(getline(simFile, line)){
		vector<int>* clockCounts = breakLine(line);
	}
	return 0;
}


vector<int>* breakLine(string line){
	char aChar = ' ';
	istringstream linestream(line);
	vector<int>* clockVec = new vector<int>;
	string intBuff = "";
	while(linestream >> noskipws >> aChar){
		if(aChar != ' '){
			intBuff += (aChar);
		}
		else{
			int aNum = atoi(intBuff.c_str());
			clockVec->push_back(aNum);
			intBuff = "";
		}
	}
	clockVec->push_back(atoi(intBuff.c_str()));
	return clockVec;
}

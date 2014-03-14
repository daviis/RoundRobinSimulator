//Written by Isaac Davis
#include <iostream>
#include <vector>

using namespace std;

typedef struct{
	int runCycles;
	int waitCycles;
	int ioCycles;
	vector<int> lifeCycle;
} Program;

void initProgram(Program* prog){
	prog -> runCycles = 0;
	prog -> waitCycles = 0;
	prog -> ioCycles = 0;
	prog -> lifeCycle = new Vector<int>;
}

int main(int argc, char* argv[]){
	Program* firstProg = new Program;
	Program* secondProg = new Program;
	initProgram(firstProg);

	return 0;
}

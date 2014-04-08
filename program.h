#ifndef PROGRAM_H
#define PROGRAM_H

#include <queue>

using namespace std;

class Program{  
        int runCycles, waitCycles, ioCycles, ioWaitCycles;
        queue<int>* lifeCycle;
    public:
        Program(queue<int>*, int);
        bool incRC();
        void incRC_notRunning();
        void decRC();   
        void incWC();
	void incIOWC();
        bool incIOC();
        int progId;
        bool exited();
        int lifeLength();
        bool needsIO(int);
        int wc();
        int rc();       
	int ioC();
	int ioWC();
        queue<int>* getLC();
};



#endif

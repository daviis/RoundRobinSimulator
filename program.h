#ifndef PROGRAM_H
#define PROGRAM_H

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



#endif

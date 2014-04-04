#include <queue>
#include "program.h"

using namespace std;

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


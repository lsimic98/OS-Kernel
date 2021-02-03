#include "thread.H"
#include "PCB.H"
#include "timer.H"
#include "lock.H"
#include "SCHEDULE.H"



// Zabranjuje prekide
#define lock asm cli

// Dozvoljava prekide
#define unlock asm sti

ID Thread::Id = 0;
Thread* Thread::mainThread = 0;

Thread::Thread(StackSize stackSize, Time timeSlice){
	id = Id;
	Id++;
	myPCB = new PCB(this,stackSize, timeSlice);
	PCB::allPCB->insert(myPCB);


}

Thread::Thread(char* c){ //For Main Thread
	id = Id;
	Id++;
	myPCB = new PCB(this,defaultStackSize,1);
	myPCB->status=RUNNING;
	PCB::running = myPCB;
	Timer::cntr = PCB::running->timeSlice;
	PCB::allPCB->insert(myPCB);


}

Thread::~Thread(){
	waitToComplete();
	if(myPCB!=0){
		myPCB->myThread=0;
		delete myPCB;
	}
}

void dispatch(){

	Timer::context_switch_on_demand = 1;
	Timer::timer();



}

void Thread::start(){

		myPCB->createStack();
		if(myPCB->status!=IDLE){
			Scheduler::put(myPCB);
	}


}

void Thread::waitToComplete(){

	if(PCB::running!=this->myPCB && this->myPCB->status!=FINISHED && this->myPCB->status!=IDLE){
		Lock::declockFlag();
		PCB::running->status = BLOCKED;
		this->myPCB->waitToCompleteList->insert(PCB::running);
		Lock::inclockFlag();
		dispatch();
	}


}

ID Thread::getID(){
	return id;
}

ID Thread::getRunningID(){
	Thread* pom = PCB::running->getThread();
	return  pom->getID();
}

Thread* Thread::getThreadById(ID id){
	if(id>1){
		PCB* p = PCB::allPCB->getById(id);
		return p->getThread();
	}
	else{
		return 0;
	}
}


PCB* Thread::getmyPCB(){
	return myPCB;
}











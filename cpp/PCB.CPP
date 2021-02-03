#include <dos.h>
#include "PCB.H"
#include "thread.h"
#include "SCHEDULE.H"
#include "List.H"
#include "lock.H"

PCB* PCB::running = 0;
List* PCB::allPCB = new List();



PCB::PCB(Thread* thread, StackSize sstackSize, Time ttimeSlice):myThread(thread),stackSize(sstackSize){
	if(ttimeSlice==0){
		timeSlice = 1;
		infinityTime = 1;
	}
	else{
		timeSlice = ttimeSlice;
		infinityTime = 0;
	}
	status=READY;
	ss=0;
	bp=0;
	sp=0;
	stack=0;
	deblockedbyTimer=0;
	waitToCompleteList = new List();
}

	void PCB::createStack(){

		stack = new unsigned[stackSize];
		stack[stackSize-1] =0x200;//setovan I fleg u pocetnom PSW-u za nit

#ifndef BCC_BLOCK_IGNORE

		stack[stackSize-2] = FP_SEG(PCB::wrapper);
		stack[stackSize-3] = FP_OFF(PCB::wrapper);

		sp = FP_OFF(stack+(stackSize-12)); //svi sacuvani registri pri ulasku u interrupt rutinu
		ss = FP_SEG(stack+(stackSize-12));
		bp = FP_OFF(stack+(stackSize-12));

#endif

	}

	void PCB::wrapper(){
		PCB::running->myThread->run();

		Lock::declockFlag();



		//vracam waitToComplete niti u scheduler
		while(PCB::running->waitToCompleteList->first!=0){
			PCB::running->waitToCompleteList->getFirst()->status=READY;
			Scheduler::put(PCB::running->waitToCompleteList->getFirst());
			PCB::running->waitToCompleteList->removeFirst();
		}


		//end
		PCB::running->status = FINISHED;
		Lock::inclockFlag();
		dispatch();
	}

	PCB::~PCB(){
		if(myThread!=0){
			myThread->myPCB=0;
			delete myThread;
		}
			delete waitToCompleteList;
			delete[] stack;

	}

	Thread* PCB::getThread(){
		return myThread;
	}











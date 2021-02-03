#include "ksem.H"
#include "timer.h"
#include "lock.H"
#include "list.H"
#include "semlist.H"
#include "PCB.H"
#include "SCHEDULE.h"
#include  "semaphor.H"

SemList* KernelSem::allSemaphores = new SemList();

KernelSem::KernelSem(int init,Semaphore* s):v(init),mySem(s){
	allwaitThreads = new List();
	allTTLThreads = new List();
}


int KernelSem::val() const {
	return v;
}

int KernelSem::wait(Time maxTimeToWait){
	Lock::declockFlag();
	v--;
	if(v<0){
		if(maxTimeToWait>0){
			PCB::running->status=BLOCKED;
			allwaitThreads->insert(PCB::running);
			allTTLThreads->insertTTL(PCB::running,maxTimeToWait);

		}
		else{
			PCB::running->status=BLOCKED;
			allwaitThreads->insert(PCB::running);
		}
		Lock::inclockFlag();
		
		dispatch();

		Lock::declockFlag();

		if(PCB::running->deblockedbyTimer==1){//Odblokirana od Tajmera - isteko TTL
			v++;
			PCB::running->deblockedbyTimer=0;
			Lock::inclockFlag();
			return 0;
		}
		else{
			PCB::running->deblockedbyTimer=0;
			Lock::inclockFlag();
			return 1;
		}
	}
	Lock::inclockFlag();
	return 1;
}



int KernelSem::signal(int n){
	Lock::declockFlag();
	PCB* pcb;
	int i=0;

	if(n<0){
		Lock::inclockFlag();
		return n;
	}
	else if(n==0){ //signal(0)
		v++;
		if(v<=0){
			pcb = allwaitThreads->getFirst();
			pcb->status=READY;
			Scheduler::put(pcb);
			if(allwaitThreads->first->ttl!=0)
				allTTLThreads->remove(pcb);
			allwaitThreads->removeFirst();
			pcb->deblockedbyTimer=0;

		}
		Lock::inclockFlag();
		return 0;
	}// end signal(0)
	else{ //singal(n>0)
		while(n>0){
			v++;
			if(v<=0){
				pcb = allwaitThreads->getFirst();
				pcb->status=READY;
				Scheduler::put(pcb);
				if(allwaitThreads->first->ttl!=0)
					allTTLThreads->remove(pcb);
				allwaitThreads->removeFirst();
				pcb->deblockedbyTimer=0;
				i++;
			}
			n--;
		}
		Lock::inclockFlag();
		return i;

	}//end signal(n>0)
	Lock::inclockFlag();
	return -1;
}


void KernelSem::updateTTL(){
	KernelSem::allSemaphores->updateTTL();
}

KernelSem::~KernelSem(){
	if(mySem!=0){
		mySem->myImpl=0;
		delete mySem;
	}
	if(v<0){
		int vv=v*-1;
		signal(vv);
	}
	delete allwaitThreads;
	delete allTTLThreads;
	allwaitThreads=0;
	allTTLThreads=0;

}

void KernelSem::deleteallSemaphores(){
	if(allSemaphores!=0){
		delete allSemaphores;
		allSemaphores = 0;
	}
}

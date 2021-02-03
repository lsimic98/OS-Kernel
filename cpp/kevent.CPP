#include "kevent.H"
#include "event.H"
#include "ivtentry.H"
#include "SCHEDULE.H"
#include "lock.H"
#include "PCB.H"
#include "kevent.H"

KernelEv::KernelEv():v(0),holder(0){}

void KernelEv::signal(){
	Lock::declockFlag();
	if(v==-1){
		if(holder->status!=FINISHED){
		holder->status=READY;
		Scheduler::put(holder);
		}
		v=0;
		holder=0;
	}

	Lock::inclockFlag();
}

void KernelEv::wait(){
	Lock::declockFlag();
	if(v==0){
		v=-1;
		holder=PCB::running;
		PCB::running->status=BLOCKED;
		Lock::inclockFlag();
		dispatch();
	}
	Lock::inclockFlag();

}

KernelEv::~KernelEv(){}

KernelEv* KernelEv::getkevent(IVTNo No){
	return IVTEntry::gettablekevent(No);
}



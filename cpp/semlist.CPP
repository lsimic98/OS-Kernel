#include "semlist.H"
#include "ksem.H"
#include "list.h"
#include "SCHEDULE.H"
#include "PCB.H"
#include "lock.H"

SemList::~SemList(){
	deleteSemList();
}


void SemList::insert(KernelSem* ksem){ //Na kraj
	Elem* newelem = new Elem(ksem);
	if(first==0){
		first=newelem;
		last=newelem;
	}
	else{
		last->next = newelem;
		last = newelem;
	}
}

void SemList::remove(KernelSem* ksem){
	if(first==0)
		return;

	if(first->ksem == ksem){
		if(first->next == 0){
			delete(first);
			first = last = 0;
			return;
		}
		Elem* old = first;
		first = first->next;
		delete old;
	}
	else{
		Elem* cur = first;
		Elem* prev = first;

		while((cur->ksem!=ksem) && (cur->next!=0)){
			prev = cur;
			cur = cur->next;
		}

		if((cur->ksem!=ksem )&& (cur->next==0))
			return;
		else if((cur->ksem==ksem) && (cur->next==0)){
			last=prev;
			prev->next=0;
			delete(cur);
		}
		else{
			prev->next = cur->next;
			delete(cur);
		}
	}
}

void SemList::deleteSemList(){
	Elem* cur = first;
	Elem* old;
	while(cur!=0){
		old = cur;
		cur=cur->next;
		delete old;
	}
	first = 0;
	last = 0;
}

KernelSem*  SemList::getFirst(){
	if(first==0){
		return 0;
	}
	else{
		return first->ksem;
	}
}

void SemList::removeFirst(){
	if(first==0)
		return;
	Elem* old = first;
	if(first->next==0){
		first=0;
		last=0;
		delete old;
	}
	else{
		first=first->next;
		delete old;
	}

}


void SemList::updateTTL(){
	Lock::declockFlag();
	Elem* cur = first;
	while(cur!=0){


		cur->ksem->allTTLThreads->decTTL();

		while(cur->ksem->allTTLThreads->first!=0 && cur->ksem->allTTLThreads->first->ttl<=0){
			cur->ksem->allwaitThreads->remove(cur->ksem->allTTLThreads->first->pcb);
			cur->ksem->allTTLThreads->first->pcb->status=READY;
			cur->ksem->allTTLThreads->first->pcb->deblockedbyTimer=1;
			Scheduler::put(cur->ksem->allTTLThreads->first->pcb);
			cur->ksem->allTTLThreads->removeFirst();
		}
		cur=cur->next;
	}
	Lock::inclockFlag();

}





#include "List.H"
#include "PCB.H"
#include "thread.H"



List::~List(){
	deleteList();
}



void List::insert(PCB* pcb){ //Na kraj
	Elem* newelem = new Elem(pcb,0);
	if(first==0){
		first=newelem;
		last=newelem;
	}
	else{
		last->next = newelem;
		last = newelem;
	}
}

void List::remove(PCB* pcb){
	if(first==0)
		return;

	if(first->pcb == pcb){
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

		while((cur->pcb!=pcb) && (cur->next!=0)){
			prev = cur;
			cur = cur->next;
		}

		if((cur->pcb!=pcb )&& (cur->next==0))
			return;
		else if((cur->pcb==pcb) && (cur->next==0)){
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

PCB* List::getById(ID id){
	Elem* cur = first;
	while(cur!=0){
		if(cur->pcb->getThread()->getID() == id){
			return cur->pcb;
		}
		cur=cur->next;
	}
	return 0;
}


void List::deleteList(){
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

PCB*  List::getFirst(){
	if(first==0){
		return 0;
	}
	else{
		return first->pcb;
	}
}

void List::removeFirst(){
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

void List::insertTTL(PCB* pcb, unsigned ttl){
	Elem* newelem = new Elem(pcb,ttl);
	if(first==0){
		first=newelem;
		last=newelem;
	}
	else if(first->ttl >= ttl){
		newelem->next = first;
		first = newelem;
	}
	else if(ttl >= last->ttl){
		last->next = newelem;
		last = newelem;
	}
	else{
		Elem* cur = first;
		Elem* prev = first;
		while(cur->ttl < ttl){
			prev=cur;
			cur=cur->next;
		}
		newelem->next=cur;
		prev->next=newelem;
	}
}


void List::decTTL(){
	Elem* cur = first;
	while(cur!=0){
		cur->ttl--;
		cur=cur->next;
	}
}

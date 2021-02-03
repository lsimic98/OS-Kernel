#include "idle.H"
#include "thread.H"
#include "PCB.H"
#include "intLock.h"
#include <iostream.h>
#include <dos.h>



Idle* Idle::idle = 0;


void Idle::run(){
	while(1){}
}

Idle::Idle():Thread(defaultStackSize,1){
	if(idle==0){
		idle=this;
		idle->myPCB->status = IDLE;
		start();
	}
}

PCB* Idle::getidlePCB(){
	return idle->myPCB;
}

void Idle::deleteIdle(){
	if(idle!=0){
		delete idle;
		idle=0;
	}
}






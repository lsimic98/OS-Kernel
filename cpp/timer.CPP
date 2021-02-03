#include <iostream.h>
#include <dos.h>
#include "timer.H"
#include "PCB.H"
#include "lock.H"
#include "SCHEDULE.H"
#include "idle.H"
#include "ksem.H"


extern void tick();
// Zabranjuje prekide
#define lock asm cli

// Dozvoljava prekide
#define unlock asm sti


int Timer::context_switch_on_demand = 0;
int Timer::cntr = 0;
pInterrupt Timer::oldInterrupt = 0;
unsigned tsp;
unsigned tss;
unsigned tbp;


void interrupt Timer::timer(...){
	if (Timer::context_switch_on_demand==0){
		if(PCB::running->infinityTime==0){
			Timer::cntr--;
		}
			KernelSem::updateTTL();

		tick();


	}
	if (Timer::cntr== 0 || Timer::context_switch_on_demand==1) {
		if(Lock::lockFlag==1){
			Timer::context_switch_on_demand=0;

		asm {
			// cuva sp
			mov tsp, sp
			mov tss, ss
			mov tbp, bp
		}

		PCB::running->sp = tsp;
		PCB::running->ss = tss;
		PCB::running->bp = tbp;


		if(PCB::running->status != FINISHED && PCB::running->status != BLOCKED && PCB::running->status != IDLE){
			PCB::running->status = READY;
			Scheduler::put(PCB::running);
		}
		PCB::running = Scheduler::get();

		if(PCB::running==0){    //Dodavanje IDLE niti
			PCB::running = Idle::idle->getidlePCB();
			PCB::running->status = IDLE;
		}
		else{
			PCB::running->status = RUNNING;
		}

		tsp = PCB::running->sp;
		tss = PCB::running->ss;
		tbp = PCB::running->bp;

		Timer::cntr = PCB::running->timeSlice;


		asm {
			mov sp, tsp   // restore sp
			mov ss, tss
			mov bp, tbp
		}
	}
	else {
		Timer::context_switch_on_demand = 1;
	}
}


if(Timer::context_switch_on_demand==0){
	Timer::oldInterrupt();
}




}

void Timer::inic(){
#ifndef BCC_BLOCK_IGNORE

	Timer::oldInterrupt = getvect(8);
	setvect(8,Timer::timer);


	#endif



}

void Timer::restore(){
#ifndef BCC_BLOCK_IGNORE

	setvect(8,Timer::oldInterrupt);

#endif
}







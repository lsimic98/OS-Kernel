#include<iostream.h>
#include <malloc.h>
#include <stdlib.h>
#include "thread.H"
#include "PCB.H"
#include "lock.H"
#include "timer.H"
#include "SCHEDULE.H"
#include "idle.H"
#include "ksem.H"
#include "semaphor.H"
#include "event.H"
#include "List.H"
#include "semlist.H"


#define lock asm cli
#define unlock asm sti


int counter = 0;

void* operator new(unsigned size) {

  void* ret = malloc(size);

  if(ret==0)
	exit(1);//Out of mem
  counter++;

  return ret;
}

void operator delete(void* loc) {

  if(loc != 0){
	counter--;
    free(loc);
  }
}






class A :public Thread{
public:
	Semaphore* s;

	A():Thread(defaultStackSize,21){ s = new Semaphore(0);};

	virtual void run();

	virtual ~A(){
		waitToComplete();
		delete s;
	}


};

void A::run(){

#ifndef BCC_BLOCK_IGNORE
	for (int i =0; i < 30; ++i) {
					lock
					cout<<"ID:"<<this->getID()<<" i = "<<i<<endl;
					unlock
					for (int k= 0; k<10000; ++k)
						for (int j = 0; j <30000; ++j);
				}

#endif
}


class B :public Thread{


public:
	 Semaphore* s;
	 B():Thread(defaultStackSize,20){s = new Semaphore(0);};

	virtual void run();

	virtual ~B(){
		waitToComplete();
		delete s;

	}
};

void B::run(){

#ifndef BCC_BLOCK_IGNORE
	for (int i =0; i < 30; ++i) {
		lock
		cout<<"ID:"<<this->getID()<<" i = "<<i<<endl;
		unlock
		//for (int k = 0; k<10000; ++k)
			//for (int j = 0; j <30000; ++j);
		s->wait(10);

	}
#endif
}


void lok(){asm cli;}
void unlok(){asm sti;}

#define debug(str) \
	{\
	lok();\
	cout<<str<<": counter= "<<counter<<endl;\
	unlok();\
	}

extern int userMain (int argc, char* argv[]);

int main(int argc, char* argv[]){
	debug("MAIN");



	lock;
	Thread::mainThread = new Thread("MainThread");
	new Idle();
	Timer::inic();
	unlock;
	/*int i;
	B **b = new B*[20];
	Semaphore* s = new Semaphore(0);

	for (i=0; i<20; i++){
		b[i] = new B();
		b[i]->start();

	}

	//debug("");


#ifndef BCC_BLOCK_IGNORE
	for (i = 0; i < 30; ++i) {
  	lock
  	cout<<"main "<<i<<endl;
  	unlock
	//for (int j = 0; j< 30000; ++j)
		//for (int k = 0; k < 30000; ++k);
	s->wait(5);


	}



#endif

	debug("Brisanje b[i]");
	for (i=0; i<20; i++){

		delete b[i];
	}
	debug("Brisanje []b");
	delete[] b;
	debug("Brisanje s");
	delete s;

	debug("Kraj");*/





	int ret =  userMain (argc, argv);

	debug("Pre brisanja restore()");

	KernelSem::deleteallSemaphores();
	debug("Kraj1");
	PCB::allPCB->deleteList();
	PCB::allPCB = 0;
	debug("Kraj2");
	Timer::restore();
	debug("Kraj3");
	Idle::idle->deleteIdle();

	//debug("Kraj4");
	delete Thread::mainThread;
	//debug("Kraj5");


	while(Scheduler::get());



	lock
	cout<<"Izlaz iz main: counter= "<<counter<<endl;
	unlock



	return ret;

}




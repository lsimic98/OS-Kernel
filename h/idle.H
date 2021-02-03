#ifndef IDLE_H_
#define IDLE_H_

#include "thread.H"
#include "PCB.H"


class Idle:public Thread{
public:
	virtual void run();
	Idle();

	static Idle* idle;
	static PCB* getidlePCB();
	static void deleteIdle();

	virtual ~Idle(){}
};



#endif /* IDLE_H_ */

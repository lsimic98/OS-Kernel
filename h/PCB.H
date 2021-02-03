#ifndef PCB_H_
#define PCB_H_
#include "thread.H"
#include "List.H"

enum Status{READY, RUNNING, BLOCKED, FINISHED,IDLE};


class PCB{
public:
	unsigned bp;
	unsigned sp;
	unsigned ss;
	enum Status status;
	Time timeSlice;
	int infinityTime;
	int deblockedbyTimer;

	static PCB* running;
	static List* allPCB;
	unsigned *stack;

	static void wrapper();


	PCB(Thread* thread, StackSize sstackSize, Time ttimeSlice);
	~PCB();

	void createStack();

	Thread* getThread();
	List* waitToCompleteList;


private:
	Thread* myThread;
	StackSize stackSize;
	friend class Thread;




};




#endif /* PCB_H_ */

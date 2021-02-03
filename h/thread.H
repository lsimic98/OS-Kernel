#ifndef THREAD_H_
#define THREAD_H_


typedef unsigned long StackSize;
const StackSize defaultStackSize = 4096;
typedef unsigned int Time; //time, x55ms
const Time defaultTimeSlice = 20; //default = 2*55ms
typedef int ID;


class PCB; //Kernel's implementation of user's thread

class Thread{
public:
	void start();
	void waitToComplete();
	virtual ~Thread();
	Thread(char* c);   //For Main Thread

	ID getID();
	static ID getRunningID();
	static Thread* getThreadById(ID id);
	static Thread* mainThread;

	PCB* getmyPCB();







protected:

	friend class PCB;
	friend class Idle;
	Thread (StackSize stackSize = defaultStackSize, Time timeSlice = defaultTimeSlice);
	virtual void run(){}

private:
	PCB* myPCB;
	static ID Id;
	ID id;




};

void dispatch();



#endif /* THREAD_H_ */

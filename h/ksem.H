#ifndef KSEM_H_
#define KSEM_H_
#include  "semaphor.H"

typedef unsigned int Time;

class List;
class SemList;


class KernelSem{
public:
	int v;

	int wait(Time maxTimeToWait);
	int signal(int n);

	int val() const;

	List* allwaitThreads;
	List* allTTLThreads;

	KernelSem(int init,Semaphore* s);
	virtual ~KernelSem();


	static SemList* allSemaphores;
	static void updateTTL();
	static void deleteallSemaphores();
private:

	Semaphore* mySem;
	friend class Semaphore;


};





#endif /* KSEM_H_ */

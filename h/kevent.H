#ifndef KEVENT_H_
#define KEVENT_H_


typedef unsigned char IVTNo;
class PCB;

class KernelEv{
public:
	KernelEv();
	~KernelEv();

	void signal();

	static KernelEv* getkevent(IVTNo No);


private:
	PCB* holder;
	int v;
	friend class Event;
	friend class IVTEntry;
	void wait();

};

#endif /* KEVENT_H_ */

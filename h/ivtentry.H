#ifndef IVTENTRY_H_
#define IVTENTRY_H_
#include "kevent.H"

typedef void interrupt (*ppInterrupt)(...);
typedef unsigned char IVTNo;
class KernelEv;

class IVTEntry{

public:
	IVTEntry(IVTNo ivtNoo, ppInterrupt newintr);
	~IVTEntry();


	void calloldInterrupt();

	KernelEv* getkevent();
	static KernelEv* gettablekevent(IVTNo No);


private:
	IVTNo ivtNo;
	static IVTEntry* table[256];
	KernelEv* kevent;
	ppInterrupt oldInterrupt;


	friend class KernelEv;

};

#define PREPAREENTRY(no, flag)\
	void interrupt intr##no(...);\
	IVTEntry ivte##no(no, intr##no);\
	void interrupt intr##no(...) {\
		ivte##no.getkevent()->signal();\
		if (flag) ivte##no.calloldInterrupt();\
	}

#endif

#include "ivtentry.H"
#include <dos.h>
#define lock asm cli
#define unlock asm sti


IVTEntry* IVTEntry::table[256];

IVTEntry::IVTEntry(IVTNo ivtNoo, ppInterrupt newintr){
#ifndef BCC_BLOCK_IGNORE

	oldInterrupt = getvect(ivtNoo);
	setvect(ivtNoo, newintr);

#endif
	ivtNo=ivtNoo;
	kevent = new KernelEv();
	table[ivtNoo] = this;
}

KernelEv* IVTEntry::getkevent(){
	return kevent;
}


KernelEv* IVTEntry::gettablekevent(IVTNo No){
	return table[No]->kevent;
}

void IVTEntry::calloldInterrupt(){
	oldInterrupt();
}

IVTEntry::~IVTEntry(){
#ifndef BCC_BLOCK_IGNORE
	lock
	setvect(ivtNo, oldInterrupt);
	unlock
#endif
	oldInterrupt = 0;
	delete kevent;
	kevent = 0;
	table[ivtNo] = 0;
}



#include "event.H"
#include "kevent.H"
#include "PCB.H"


Event::Event(IVTNo ivtNo){
	myImpl = KernelEv::getkevent(ivtNo);
	holder = PCB::running;
}

void Event::wait(){
	if(PCB::running==holder){
		myImpl->wait();
	}
}

void Event::signal(){
	myImpl->signal();
}

Event::~Event(){}



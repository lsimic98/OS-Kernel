#include "semaphor.H"
#include "ksem.H"
#include "semlist.H"


Semaphore::Semaphore(int init){
	myImpl = new KernelSem(init,this);
	KernelSem::allSemaphores->insert(myImpl);
}

int Semaphore::wait(Time maxTimeToWait){
	return myImpl->wait(maxTimeToWait);
}

int Semaphore::signal(int n){
	return myImpl->signal(n);
}

int Semaphore::val()const{
	return myImpl->val();
}

Semaphore::~Semaphore(){
	if(myImpl!=0){
		KernelSem::allSemaphores->remove(myImpl);
		myImpl->mySem=0;
		delete myImpl;

	}
}




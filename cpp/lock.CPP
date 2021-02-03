#include "lock.H"
#include <dos.h>
int Lock::lockFlag=1;

#define lock asm cli
#define unlock asm sti

Lock::Lock() { }
void Lock::inclockFlag(){
	lockFlag=1;
}
void Lock::declockFlag(){
	lockFlag=0;
}



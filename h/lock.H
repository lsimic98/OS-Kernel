#ifndef LOCK_H_
#define LOCK_H_

class Lock{



public:
	static int lockFlag;
	static void inclockFlag();
	static void declockFlag();
	Lock();

};





#endif /* LOCK_H_ */

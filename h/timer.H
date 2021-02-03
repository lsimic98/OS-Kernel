#ifndef TIMER_H_
#define TIMER_H_

typedef void interrupt (*pInterrupt)(...);

class Timer{

public:
	static int context_switch_on_demand;
	static int cntr;
	static pInterrupt oldInterrupt;
	static void interrupt timer(...);
	static void inic();
	static void restore();
};





#endif /* TIMER_H_ */

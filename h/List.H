#ifndef LIST_H_
#define LIST_H_
#include "thread.H"

class PCB;

class List{
private:
	struct Elem{
		PCB* pcb;
		unsigned ttl;
		Elem* next;
		Elem(PCB* ppcb,unsigned TTL):pcb(ppcb),next(0),ttl(TTL){}
		~Elem(){}
	};
public:
	Elem* first;
	Elem *last;
	void insert(PCB* pcb);
	void insertTTL(PCB* pcb, unsigned TTL);
	void remove(PCB* pcb);
	PCB* getFirst();
	void removeFirst();

	void decTTL();


	PCB* getById(ID id);

	void deleteList();
	List():first(0),last(0){}
	~List();
};





#endif /* LIST_H_ */

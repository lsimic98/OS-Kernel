#ifndef SEMLIST_H_
#define SEMLIST_H_

class KernelSem;

class SemList{
	private:
		struct Elem{
			KernelSem* ksem;
			Elem* next;
			Elem(KernelSem* kksem):ksem(kksem),next(0){}
			~Elem(){}
		};
	public:
		Elem* first;
		Elem *last;
		void insert(KernelSem* ksem);
		void remove(KernelSem* ksem);
		KernelSem* getFirst();
		void removeFirst();

		void deleteSemList();

		SemList():first(0),last(0){}
		~SemList();

		void updateTTL();



		friend class List;
		friend class KernelSem;
	};









#endif /* SEMLIST_H_ */

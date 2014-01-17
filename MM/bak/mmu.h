/*
 * mmu.h
 *
 *  Created on: Nov 11, 2013
 *      Author: aaronwong
 */

#ifndef MMU_H_
#define MMU_H_

#include "common.h"
#include "baseclass.h"


class AARON_MMU
{
	private:
		char mmu_type;      //denote the current mmu type, which is used to creat according resources
		vector<int> *fifo;
		//PTE* ptecpy;
		//int* frametablecpy;
	public:

	AARON_MMU(char type)
	{
		mmu_type=type;
		if(mmu_type=='f')
		{
			fifo=new vector<int>;
			/*debug*/
			//printf("FIFO type constructed!\n");
		}




	}

	~AARON_MMU()
	{
		if(mmu_type=='f')
		{
			delete fifo;
			/*debug*/
			//printf("FIFO type destructed!\n");
		}

	}

	int Process(PTE* ptecpy,int* frametablecpy);

	void mmu_update(int pf_number);  //interface for update the information

	//when there is no need for replacement, updating the list
	void init(int pf_number)
	{
		if(mmu_type=='f')
		{
     fifo->push_back(pf_number);
		}
	}
};




#endif /* MMU_H_ */


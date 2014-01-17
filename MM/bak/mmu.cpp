/*
 * mmu.cpp
 *
 *  Created on: Nov 11, 2013
 *      Author: aaronwong
 */

#include "mmu.h"


int AARON_MMU::Process(PTE* ptecpy,int* frametablecpy)
{
	int index;
	if(mmu_type=='f')
	{
		index=fifo->front();
		fifo->erase(fifo->begin());
		fifo->push_back(index);
		return index;
	}

	printf("error!\n");
	exit(-1);
}

//update information when 
void AARON_MMU::mmu_update(int pf_number)
{
	if(mmu_type=='f')
	{
		return;
	}

}






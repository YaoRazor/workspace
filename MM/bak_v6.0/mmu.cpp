/*
 * mmu.cpp
 *
 *  Created on: Nov 11, 2013
 *      Author: aaronwong
 */

#include "mmu.h"

extern PTE newpte[64];
extern int global_pp_number;
extern int current;
extern int global_random;

int AARON_MMU::Process(PTE* ptecpy,int* frametablecpy)
{
	int index;
	index=fifo->front();
	fifo->erase(fifo->begin());
	fifo->push_back(index);
	return index;
	//printf("error!\n");
	//exit(-1);
}


int mmu_sc::Process(PTE* ptecpy,int* frametablecpy)
{
	int index;
	index=fifosc->front();
	
	//cnt is used for debug
	int cnt=0;
	while(newpte[frametablecpy[index]].f3==1)
	{
		cnt++;
		newpte[frametablecpy[index]].f3=0;
		fifosc->erase(fifosc->begin());
		fifosc->push_back(index);
		index=fifosc->front();
	}	
	fifosc->erase(fifosc->begin());
	fifosc->push_back(index);
	//sort(fifosc->begin(),fifosc->end());
	//printf("index=%d\n",index);
	//printf("cnt=%d\n",cnt);

	return index;
}


int mmu_lru::Process(PTE* ptecpy,int* frametablecpy)
{

	int min_index=0;
	int min_value=lru_tab[0];

	for(int i=1;i<global_pp_number;i++)
	{
		if(lru_tab[i]<min_value)
		{
			min_index=i;
			min_value=lru_tab[i];
		}

	}

	lru_tab[min_index]=current;

	return min_index;

}

void mmu_lru::mmu_update(int pf_number)
{
	lru_tab[pf_number]=current;
} 

//when there is no need for replacement, updating the list
void mmu_lru::init(int pf_number)
{
	 lru_tab[pf_number]=current;
	 //fifosc->push_back(pf_number);
}


int mmu_random::Process(PTE* ptecpy,int* frametablecpy)
{
	random_index=global_random;
  return random_index;
}


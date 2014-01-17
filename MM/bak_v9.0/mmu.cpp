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

/*Interface of mmu_aging*/
void mmu_aging::mmu_update(int pf_number)
{
	recording[pf_number]=1;

  //unsigned int aging_tab[64];
	//int recording[64];

}

int mmu_aging::Process(PTE* ptecpy,int* frametablecpy)
{
	for(int i=0;i<global_pp_number;i++)
	{
		aging_tab[i]=(aging_tab[i]>>1); //right shift all aging_tab
		if(recording[i]==1)
		{
			aging_tab[i] |= 1 << 31;    //if recording[i]==1, set highest bit as 1
		}
		recording[i]=0;               //reset corresponding recording bit
		newpte[frametablecpy[i]].f3=0;
	}

	int index=0;
	unsigned int min_value=aging_tab[0];

	for(int j=1;j<global_pp_number;j++)
	{
		if(aging_tab[j]<min_value)
		{
			index=j;
			min_value=aging_tab[j];
	    //printf("index=%d\n",j);
	    //printf("min_value=%x\n",aging_tab[index]);
		}
	}

	//printf("min_value=%x\n",aging_tab[index]);

	//printf("")

	aging_tab[index]=0;
	recording[index]=1;

	//printf("||");
	//for(int k=0;k<global_pp_number;k++)
	//{
		//printf("%d: %x ",k,aging_tab[k]);
	//}
	//printf("\n");

	return index;

}


/*Interface of mmu_Aging*/
void mmu_Aging::mmu_update(int pf_number)
{
	recording[pf_number]=1;

  //unsigned int aging_tab[64];
	//int recording[64];

}

int mmu_Aging::Process(PTE* ptecpy,int* frametablecpy)
{
	for(int i=0;i<global_pp_number;i++)
	{
		aging_tab[i]=(aging_tab[i]>>1); //right shift all aging_tab
		if(recording[i]==1)
		{
			aging_tab[i] |= 1 << 31;    //if recording[i]==1, set highest bit as 1
		}
		recording[i]=0;               //reset corresponding recording bit
		newpte[frametablecpy[i]].f3=0;
	}

	/* The difference between vp and pf*/
	int temp[global_pp_number];
	for(int k=0;k<global_pp_number;k++)
	{
		temp[k]=frametablecpy[k];   //temp[k] equals vp_number
	}
	sort(temp,temp+global_pp_number);

	int temp_index=0;

	unsigned int min_value=aging_tab[newpte[temp[0]].f5];

	for(int j=1;j<global_pp_number;j++)
	{
		if(aging_tab[newpte[temp[j]].f5]<min_value)
		{
			temp_index=j;
			min_value=aging_tab[newpte[temp[j]].f5];
	    //printf("index=%d\n",j);
	    //printf("min_value=%x\n",aging_tab[index]);
		}
	}

  int index=newpte[temp[temp_index]].f5;

	/* The difference between vp and pf*/

	aging_tab[index]=0;
	recording[index]=1;

	//printf("||");
	//for(int k=0;k<global_pp_number;k++)
	//{
		//printf("%d: %x ",k,aging_tab[k]);
	//}
	//printf("\n");

	return index;

}


int mmu_Clock::Process(PTE* ptecpy,int* frametablecpy)
{
	//int index;
	//index=fifosc->front();
	
	//cnt is used for debug
	while(1)
	{
		
		if(newpte[hand].f1==0)
		{
			hand++;
			hand=hand%64;
			continue;
		}
		else if(newpte[hand].f3==1)
		{
		  newpte[hand].f3=0;
			hand++;
			hand=hand%64;
		}
		else
		{
			break;
		}
	}	

	return newpte[hand].f5;

}






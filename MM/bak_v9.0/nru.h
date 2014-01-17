/*
 * nru.h
 *
 * Interface for generic paging alogorithm
 *  Created on: Nov 9, 2013
 *      Author: aaronwong
 */

#ifndef NRU_H_
#define NRU_H_

#include "common.h"
#include "baseclass.h"
#include "mmu.h"


class mmu_NRU: public MyMMU
{
	private:
		int* frametable;
		int pp_number;     // number of physical frame
		int cycle;         // for rest R bit every 10th replacement
		char mmutype;
		
	  vector<int> class1;
	  vector<int> class2;
		vector<int> class3;
		vector<int> class4;
		mmu_base* alicia;

	public:
	mmu_NRU(int numberofpage,char type)
	{
		mmutype=type;

		//deference
		if(mmutype!='N')
		{
			if(mmutype=='f')
			{
		   alicia=new AARON_MMU;
			}
			if(mmutype=='s')
			{
			 alicia=new mmu_sc;
			}
			if(mmutype=='l')
			{
				alicia=new mmu_lru;
			}
			if(mmutype=='r')
			{
				alicia=new mmu_random;
			}
			if(mmutype=='a')
			{
				alicia=new mmu_aging;
			}
			if(mmutype=='A')
			{
				alicia=new mmu_Aging;
			}
			if(mmutype=='C')
			{
				alicia=new mmu_Clock;
			}
		}

		cycle=0;
		pp_number=numberofpage; //get the number of physical frame
		frametable=new int[numberofpage];

		//init all the entries of frametable as -1
		for(int i=0;i<numberofpage;i++)
		{
			frametable[i]=-1;
		}


	};
	~mmu_NRU()
	{
		//deference
	  if(mmutype!='N')	
		{
		  delete alicia;
		}

		delete frametable;
	};

	void Process();      
	//void update(int op,int vp_num);
	int locatepage();
	int replacepage();
	void unmap_update(int instr_index,int vp_index, int pf_index);
	void map_update(int instr_index,int vp_index, int pf_index);
	void information_print();
	int find_index(int vp_index);
	void resetcycle();
};



#endif /* NRU_H_ */

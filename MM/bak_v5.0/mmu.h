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

class mmu_base
{
	public:
	mmu_base(){};
	virtual ~mmu_base(){};

	virtual int Process(PTE* ptecpy,int* frametablecpy)=0;

	virtual void mmu_update(int pf_number)=0;  //interface for update the information

	//when there is no need for replacement, updating the list
	virtual void init(int pf_number)=0;

};



class AARON_MMU: public mmu_base
{
	private:
		vector<int> *fifo;
	public:

	AARON_MMU()
	{
			fifo=new vector<int>;
	}

	~AARON_MMU()
	{
			delete fifo;
	}

	int Process(PTE* ptecpy,int* frametablecpy);

	void mmu_update(int pf_number){}  //interface for update the information

	//when there is no need for replacement, updating the list
	void init(int pf_number)
	{
     fifo->push_back(pf_number);
	}
};


class mmu_sc: public mmu_base
{
	private:
    vector<int> *fifosc;

	public:
		mmu_sc()
		{
			fifosc=new vector<int>;
		}
		
		~mmu_sc()
		{
			delete fifosc;
		}

	int Process(PTE* ptecpy,int* frametablecpy);

	void mmu_update(int pf_number){}  //interface for update the information

	//when there is no need for replacement, updating the list
	void init(int pf_number)
	{
     fifosc->push_back(pf_number);
	}
};


class mmu_lru: public mmu_base
{
	private:
    //vector<int> *fifosc;
  int lru_tab[64];

	public:
		mmu_lru()
		{
			//fifosc=new vector<int>;
			for(int i=0;i<64;i++)
			{
				lru_tab[i]=0;
			}
		}
		
		~mmu_lru()
		{
		}

	int Process(PTE* ptecpy,int* frametablecpy);

	void mmu_update(int pf_number);

	//when there is no need for replacement, updating the list
	void init(int pf_number);
};


#endif /* MMU_H_ */


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

struct IO_PAIR
{
	int index;
	int arrive_time;
	int track_number;
	int priority;
};

/*class which is used in main function*/
class mmu_NRU: public MyMMU
{
	private:
		int* frametable;
		int pp_number;     // number of physical frame
		int cycle;         // for rest R bit every 10th replacement
		char mmutype;
	
		/*IO information*/
		bool direction;
		int total_time;
		int total_movement;
		int total_turnaround;
		int total_wait;
		double avg_turnaround;
		double avg_waittime;
		int max_waittime;
		int system_time; 
		int current_position;
		

		vector<IO_PAIR> blockedlist;
		vector<IO_PAIR> readylist;
		IO_PAIR current_event;
		IO_PAIR finish_event;




		
	  vector<int> class1;
	  vector<int> class2;
		vector<int> class3;
		vector<int> class4;
		mmu_base* alicia;

	public:
	mmu_NRU(int numberofpage,char type)
	{
		/*IO Scheduler output*/
		mmutype=type;
		//printf("type=%c\n",mmutype);

		system_time=0;
		total_time=0;
		total_movement=0;
		avg_turnaround=0;
		avg_waittime=0;
		max_waittime=0;
		current_position=0;
		total_turnaround=0;
		total_wait=0;
		direction=true;



		/*IO Scheduler output*/

		

	  alicia=new AARON_MMU;

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
		//if(mmutype!='N')	
		//{
			delete alicia;
		//}

		//delete blockedlist;
		delete frametable;
	};

	void Process();      
	void ProcessIO();      
	//void update(int op,int vp_num);
	int locatepage();
	int replacepage();
	void unmap_update(int instr_index,int vp_index, int pf_index);
	void map_update(int instr_index,int vp_index, int pf_index);
	void information_print();
	void information_print1();
	int find_index(int vp_index);
	void resetcycle();
  
	/*IO Scheduler interface*/
  void getevent();
	void scheduler();
  void putevent();
	void init_io();
	IO_PAIR up();
	IO_PAIR down();
	IO_PAIR cscan();
  IO_PAIR locate_event();

	/*IO Scheduler interface*/
};



#endif /* NRU_H_ */

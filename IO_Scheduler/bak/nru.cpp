/*
 * nru.cpp
 *
 *  Created on: Nov 9, 2013
 *      Author: aaronwong
 */

#include "nru.h"

#define VERBOSE 
PTE newpte[64];
int global_pp_number;
int current;
int global_random;
int fuck;

extern bool o_option[6];

bool my_priority(IO_PAIR a, IO_PAIR b)
{
	return a.priority<b.priority;
}

bool my_priority1(IO_PAIR a, IO_PAIR b)
{
	return a.track_number<b.track_number;
}


void mmu_NRU::Process()      
{
	 //printf("instr_size=%d\n",instr_size);
	 
	 global_pp_number=pp_number;

	 for(int i=0;i<instr_size;i++)
	 {
		 current=i;

		 if(o_option[0])
		 {
      printf("==> inst: %d %d\n",instruction[i].arrive_time,instruction[i].track_number);
		 }

		 //if the vp has been mapped to pp
		 if(pte[instruction[i].track_number].f1==1)
			{
				update(instruction[i].arrive_time,instruction[i].track_number);
				if(mmutype!='N')
				{
					    // get the pf_number of this vp_num
					    int temp=pte[instruction[i].track_number].f5;
              alicia->mmu_update(temp);
				}
				//information
				information_print1();
				continue;
			}

		 //page fault
		 if(pte[instruction[i].track_number].f1==0)
		 {
			  int pf_index=locatepage(); //get the physical frame number, specified by algorithm
				int vp_index=frametable[pf_index]; //get the virtual page number

        if(vp_index!=-1) 
				{
				 unmap_update(i,vp_index,pf_index);
				}
         
				map_update(i,instruction[i].track_number,pf_index);

		 }
     
		 //information
		 information_print1();

	 }

	 information_print();
	 
   totalcost=(unsigned long long)(stats.maps+stats.umaps)*400+ (unsigned long long)(stats.ins+stats.outs)*3000+(unsigned long long)(stats.zeros)*150+(unsigned long long)instr_size*1;
	 
	 if(o_option[3])
	 {
	 printf("SUM %d U=%d M=%d I=%d O=%d Z=%d ===> %llu\n",instr_size,stats.umaps,stats.maps,stats.ins,stats.outs,stats.zeros,totalcost);
	 }

}

int mmu_NRU::locatepage()
{

	//test if there is free physical frame
	for(int i=0;i<pp_number;i++)
	{
		if(frametable[i]==-1)
		{
			//deference
			if(mmutype!='N')
			{
			  alicia->init(i);
			}
			return i;
		}
	}

	//default is NRU, if type is not NRU, use alicia to switch our algorithm
	//deference
	if(mmutype!='N')
	{
		global_random=myrandom(pp_number);
		//PTE *newpte=new PTE[64];
		for(int k=0;k<64;k++)
		{
			//printf("enter\n");
			newpte[k].f1=pte[k].f1;
			newpte[k].f2=pte[k].f2;
			newpte[k].f3=pte[k].f3;
			newpte[k].f4=pte[k].f4;
			newpte[k].f5=pte[k].f5;
		}
		//memcpy(newpte,pte,64*sizeof(PTE));
		int index= alicia->Process(pte,frametable);

	  for(int k=0;k<64;k++)
		{
			//printf("enter\n");
			pte[k].f1=newpte[k].f1;
			pte[k].f2=newpte[k].f2;
			pte[k].f3=newpte[k].f3;
			pte[k].f4=newpte[k].f4;
			pte[k].f5=newpte[k].f5;
		}

		return index;

	}

	class1.clear();
  class2.clear();	
	class3.clear();
	class4.clear();

	for(int i=0;i<pp_number;i++)
	{

		if((pte[frametable[i]].f2==0) &&(pte[frametable[i]].f3==0))
			class1.push_back(frametable[i]);

		if((pte[frametable[i]].f2==1) &&(pte[frametable[i]].f3==0))
			class2.push_back(frametable[i]);

		if((pte[frametable[i]].f2==0) &&(pte[frametable[i]].f3==1))
			class3.push_back(frametable[i]);

		if((pte[frametable[i]].f2==1) &&(pte[frametable[i]].f3==1))
			class4.push_back(frametable[i]);
	}

	cycle+=1;
	if(cycle==10)
		resetcycle();

  return replacepage();
}

int mmu_NRU::replacepage()
{
	int index=0;
	int size=0;
	 
	//if(test==9)
	//{
		//printf("%d,%d,%d,%d\n",class1.size(),class2.size(),class3.size(),class4.size());
	//}

	if(class1.size())
	{
		//vector<string>::iterator iter;
		sort(class1.begin(),class1.end());
		size=class1.size();
		index=class1[myrandom(size)];
		int pf_index=find_index(index);
		return pf_index;
	}
	if(class2.size())
	{
		sort(class2.begin(),class2.end());
		size=class2.size();
		index=class2[myrandom(size)];
		int pf_index=find_index(index);
		return pf_index;
	}
	if(class3.size())
	{
		sort(class3.begin(),class3.end());
		size=class3.size();
		index=class3[myrandom(size)];
		int pf_index=find_index(index);
		return pf_index;
	}
	if(class4.size())
	{
		sort(class4.begin(),class4.end());
		size=class4.size();
		index=class4[myrandom(size)];
		int pf_index=find_index(index);
		return pf_index;
	}

	printf("error!\n");
	exit(-1);
}

void mmu_NRU::unmap_update(int instr_index,int vp_index,int pf_index)
{
	  if(o_option[0])
		{
		 printf("%d: UNMAP %d %d\n",instr_index,vp_index,pf_index);
		}
		stats.umaps+=1;

		pte[vp_index].f1=0;    // set as unpresent
		pte[vp_index].f3=0;    // set as unreferenced
		//pte[vp_index].f2=0;    // set as unmodified 
		if(pte[vp_index].f2==1)
		{
			pte[vp_index].f2=0;    //set the paged out frame as unmodifed

			if(o_option[0])
			{
			  printf("%d: OUT %d %d\n",instr_index,vp_index,pf_index);
			}

			stats.outs+=1;
			pte[vp_index].f4=1;    //set the paged out frame as pagedout
		}

		frametable[pf_index]=-1;  //unmap operation

}



void mmu_NRU::map_update(int instr_index,int vp_index, int pf_index)
{
	pte[vp_index].f1=1;      //set as present
  pte[vp_index].f3=1;      //set as referenced
	pte[vp_index].f2=instruction[instr_index].arrive_time;

	if(pte[vp_index].f4==1)
	{
		if(o_option[0])
		{
		 printf("%d: IN %d %d\n",instr_index,vp_index,pf_index);
		}
		stats.ins+=1;
	}
	else
	{
		if(o_option[0])
		{
		 printf("%d: ZERO   %d\n",instr_index,pf_index);
		}
		stats.zeros+=1;
	}
	//pte[vp_index].f4       //to be determined
	
	frametable[pf_index]=vp_index;
	pte[vp_index].f5=pf_index;

	if(o_option[0])
	{
	  printf("%d: MAP %d %d\n",instr_index,vp_index,pf_index);
	}
	stats.maps+=1;
}


void mmu_NRU::information_print()
{
	if(o_option[1])
	{
	for(int i=0;i<64;i++)
	{
		if(pte[i].f1==0)
		{
			if(pte[i].f4==1)
				printf("#");
			else
				printf("*");

			printf(" ");
		}
		else
		{
			printf("%d:",i);
      //print R bit
			if(pte[i].f3==1)
			{
				printf("R");
			}
			else
			{
			  printf("-");
			}

      //print M bit
			if(pte[i].f2==1)
			{
				printf("M");
			}
			else
			{
			  printf("-");
			}

      //print S bit
			if(pte[i].f4==1)
			{
				printf("S");
			}
			else
			{
			  printf("-");
			}
      
			printf(" ");
			
		}
	}

	printf("\n");
	}


   
	if(o_option[2])
	{
		for(int i=0;i<pp_number;i++)
		{
			if(frametable[i]==-1)
				printf("* ");
			else
			printf("%d ",frametable[i]);
		}
	  printf("\n");

	}



}

int mmu_NRU::find_index(int vp_index)
{
	for(int i=0;i<pp_number;i++)
	{
		if(frametable[i]==vp_index)
		{
			return i;
		}
	}
	printf("error");
	exit(-1);
}


void mmu_NRU::resetcycle()
{
	for(int i=0;i<pp_number;i++)
	{
     pte[frametable[i]].f3=0;
	}
	cycle=0;
}


void mmu_NRU::information_print1()
{
	if(o_option[4])
	{
	for(int i=0;i<64;i++)
	{
		if(pte[i].f1==0)
		{
			if(pte[i].f4==1)
				printf("#");
			else
				printf("*");

			printf(" ");
		}
		else
		{
			printf("%d:",i);
      //print R bit
			if(pte[i].f3==1)
			{
				printf("R");
			}
			else
			{
			  printf("-");
			}

      //print M bit
			if(pte[i].f2==1)
			{
				printf("M");
			}
			else
			{
			  printf("-");
			}

      //print S bit
			if(pte[i].f4==1)
			{
				printf("S");
			}
			else
			{
			  printf("-");
			}
      
			printf(" ");
			
		}
	}

	printf("\n");

	}


  
	 if(o_option[5])
	 {
		for(int i=0;i<pp_number;i++)
		{
			if(frametable[i]==-1)
				printf("* ");
			else
			printf("%d ",frametable[i]);
		}

	  printf("\n");
	 }



}


void mmu_NRU::ProcessIO()      
{
	//printf("start main process!\n");
	//printf("blockedlist.size=%d\n",blockedlist.size());
	  
	init_io();
	/* for test */
		for(int i=0;i<instr_size;i++)
		{
			//instruction[i].arrive_time=atoi((*pointer_input)[2*i].c_str());
			//cout<<"blockedlist["<<i<<"].arrive_time="<<blockedlist[i].arrive_time<<endl;
			//instruction[i].track_number=atoi((*pointer_input)[2*i+1].c_str());
			//cout<<"blockedlist["<<i<<"].track_number="<<blockedlist[i].track_number<<endl;
		}
	/* for test */

	 for(int i=0;i<instr_size;i++)
	 {
		 getevent();
		 scheduler();
		 putevent();

		 // finish operation
#ifdef VERBOSE
		 printf("%d: %d finish %d\n",system_time,finish_event.index,system_time-finish_event.arrive_time);
#endif

	 }

	 total_time=total_movement+instruction[0].arrive_time;
	 avg_turnaround=(double)total_turnaround/instr_size;
	 avg_waittime=(double)total_wait/instr_size;


	 printf("SUM: %d %d %.2lf %.2lf %d\n",system_time,total_movement,avg_turnaround,avg_waittime,max_waittime);
	 //printf("total_time=%d\n",system_time);
	 //printf("total_movement=%d\n",total_movement);
	 //printf("avg_turnaround=%f\n",(double)total_turnaround/instr_size);
	 //printf("avg_waittime=%f\n",(double)total_wait/instr_size);

}

void mmu_NRU::getevent()
{
	if(readylist.size()>0)
	{
		current_event=locate_event();
		//current_event=*readylist.begin();
		//readylist.erase(readylist.begin());
	}
	else
	{
		current_event=*blockedlist.begin();
		system_time=current_event.arrive_time;
		putevent();
		getevent();
	}
}

void mmu_NRU::scheduler()
{
	// issue operation
#ifdef VERBOSE
	printf("%d: %d issue %d %d\n",system_time,current_event.index,current_event.track_number,current_position);
#endif
	//Calculate the wait_time of the specific IO operation
	int wait_time=system_time-current_event.arrive_time;
	max_waittime=max(max_waittime,wait_time);
	total_wait+=wait_time;

	int move_time=abs(current_position-current_event.track_number);
	fuck=move_time;
	total_movement+=move_time;

	current_position=current_event.track_number;
	system_time+=move_time;
	total_turnaround+=system_time-current_event.arrive_time;

	finish_event=current_event;


}

void mmu_NRU::putevent()
{
	/*Specified for FScan*/
  //if(mmutype=='F')
	//{
		//while(blockedlist.size()>0)
		//{
			//current_event=*blockedlist.begin();
			//if(current_event.arrive_time<=system_time)
			//{
				//// add operation
//#ifdef VERBOSE
				//printf("%d: %d add %d\n",current_event.arrive_time,current_event.index,current_event.track_number);
//#endif

				//readylist.push_back(current_event);
				//blockedlist.erase(blockedlist.begin());

			//}
			//else
			//{
				//break;
			//}
			
		//}

		//return;
	//}





	/*Specified for FScan*/




	/*put all the elements that are ready by now to the list*/
	while(blockedlist.size()>0)
	{
		current_event=*blockedlist.begin();
		if(current_event.arrive_time<=system_time)
		{
			// add operation
#ifdef VERBOSE
			printf("%d: %d add %d\n",current_event.arrive_time,current_event.index,current_event.track_number);
#endif

			readylist.push_back(current_event);
			blockedlist.erase(blockedlist.begin());

		}
		else
		{
			break;
		}
		
	}
	
}

void mmu_NRU::init_io()
{
	  IO_PAIR temp;
    for(int i=0;i<instr_size;i++)
		{
			temp.arrive_time=instruction[i].arrive_time;
			temp.track_number=instruction[i].track_number;
			temp.index=i;
			temp.priority=0;
			blockedlist.push_back(temp);
		}

}

IO_PAIR mmu_NRU::locate_event()
{
	IO_PAIR temp;
	if(mmutype=='f')
	{
		temp=*readylist.begin();
		readylist.erase(readylist.begin());
		return temp;
	}
	else if(mmutype=='s')
	{
		vector<IO_PAIR>::iterator it=readylist.begin();
		for (it = readylist.begin() ; it != readylist.end(); ++it)
		{
			it->priority=abs(it->track_number-current_position);
		}
		sort(readylist.begin(),readylist.end(),my_priority);

		temp=*readylist.begin();
		readylist.erase(readylist.begin());
		return temp;
	}
	else if(mmutype=='S')
	{
		if(direction==true)
		{
			temp=up();
			return temp;
		}
		else
		{
			temp=down();
			return temp;
		}
	}
	else if(mmutype=='C')
	{
		temp=cscan();
		return temp;
	}
	else
	{
		printf("prameter error! Please check the parameter for -s option\n");
		exit(0);
	}

}

IO_PAIR mmu_NRU::up()
{
	  IO_PAIR temp;
		vector<IO_PAIR>::iterator it=readylist.begin();
		vector<IO_PAIR>::iterator location=readylist.end();
		for (it = readylist.begin() ; it != readylist.end(); ++it)
		{
			if(it->track_number>=current_position)
			{
				it->priority=it->track_number-current_position;
				if(location==readylist.end())
				{
					location=it;
					continue;
				}
				else
				{
					if(it->priority<location->priority)
					{
						location=it;
						continue;
					}

				}
			}
		}

		if(location==readylist.end())
		{
			direction=false;
			temp=down();
			return temp;
		}
		else
		{
			temp=*location;
			readylist.erase(location);
			return temp;
		}

}

IO_PAIR mmu_NRU::down()
{
	IO_PAIR temp;
	vector<IO_PAIR>::iterator it=readylist.begin();
	vector<IO_PAIR>::iterator location=readylist.end();
	for (it = readylist.begin() ; it != readylist.end(); ++it)
	{
		if(it->track_number<=current_position)
		{
			it->priority=current_position-it->track_number;
			if(location==readylist.end())
			{
				location=it;
				continue;
			}
			else
			{
				if(it->priority<location->priority)
				{
					location=it;
					continue;
				}

			}
		}
	}

	if(location==readylist.end())
	{
		direction=true;
		temp=up();
		return temp;
	}
	else
	{
		temp=*location;
		readylist.erase(location);
		return temp;
	}
}


IO_PAIR mmu_NRU::cscan()
{
	  IO_PAIR temp;
		vector<IO_PAIR>::iterator it=readylist.begin();
		vector<IO_PAIR>::iterator location=readylist.end();
		for (it = readylist.begin() ; it != readylist.end(); ++it)
		{
			if(it->track_number>=current_position)
			{
				it->priority=it->track_number-current_position;
				if(location==readylist.end())
				{
					location=it;
					continue;
				}
				else
				{
					if(it->priority<location->priority)
					{
						location=it;
						continue;
					}

				}
			}
		}

		if(location==readylist.end())
		{
			sort(readylist.begin(),readylist.end(),my_priority1);
		  temp=*readylist.begin();
		  readylist.erase(readylist.begin());
			return temp;
		}
		else
		{
			temp=*location;
			readylist.erase(location);
			return temp;
		}

}

/*
 * nru.cpp
 *
 *  Created on: Nov 9, 2013
 *      Author: aaronwong
 */

#include "nru.h"

//int test;
PTE newpte[64];
int global_pp_number;
int current;
int global_random;

void mmu_NRU::Process()      
{
	 //printf("instr_size=%d\n",instr_size);
	 
	 global_pp_number=pp_number;

	 for(int i=0;i<instr_size;i++)
	 {
		 current=i;
     printf("==> inst: %d %d\n",instruction[i].op,instruction[i].vp_num);

		 //if the vp has been mapped to pp
		 if(pte[instruction[i].vp_num].f1==1)
			{
				update(instruction[i].op,instruction[i].vp_num);
				if(mmutype!='N')
				{
					    // get the pf_number of this vp_num
					    int temp=pte[instruction[i].vp_num].f5;
              alicia->mmu_update(temp);
				}
				//information
				//information_print();
				continue;
			}

		 //page fault
		 if(pte[instruction[i].vp_num].f1==0)
		 {
			  int pf_index=locatepage(); //get the physical frame number, specified by algorithm
				int vp_index=frametable[pf_index]; //get the virtual page number

        if(vp_index!=-1) 
				{
				 unmap_update(i,vp_index,pf_index);
				}
         
				map_update(i,instruction[i].vp_num,pf_index);

		 }
     
		 //information
		 //information_print();

	 }

	 information_print();
	 
   totalcost=(unsigned long long)(stats.maps+stats.umaps)*400+ (unsigned long long)(stats.ins+stats.outs)*3000+(unsigned long long)(stats.zeros)*150+(unsigned long long)instr_size*1;
	 printf("SUM %d U=%d M=%d I=%d O=%d Z=%d ===> %llu\n",instr_size,stats.umaps,stats.maps,stats.ins,stats.outs,stats.zeros,totalcost);

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
	// Get the information of 4 classes
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
		printf("%d: UNMAP %d %d\n",instr_index,vp_index,pf_index);
		stats.umaps+=1;

		pte[vp_index].f1=0;    // set as unpresent
		pte[vp_index].f3=0;    // set as unreferenced
		//pte[vp_index].f2=0;    // set as unmodified 
		if(pte[vp_index].f2==1)
		{
			pte[vp_index].f2=0;    //set the paged out frame as unmodifed
			printf("%d: OUT %d %d\n",instr_index,vp_index,pf_index);
			stats.outs+=1;
			pte[vp_index].f4=1;    //set the paged out frame as pagedout
		}

		frametable[pf_index]=-1;  //unmap operation

}



void mmu_NRU::map_update(int instr_index,int vp_index, int pf_index)
{
	pte[vp_index].f1=1;      //set as present
  pte[vp_index].f3=1;      //set as referenced
	pte[vp_index].f2=instruction[instr_index].op;

	if(pte[vp_index].f4==1)
	{
		printf("%d: IN %d %d\n",instr_index,vp_index,pf_index);
		stats.ins+=1;
	}
	else
	{
		printf("%d: ZERO   %d\n",instr_index,pf_index);
		stats.zeros+=1;
	}
	//pte[vp_index].f4       //to be determined
	
	frametable[pf_index]=vp_index;
	pte[vp_index].f5=pf_index;
	printf("%d: MAP %d %d\n",instr_index,vp_index,pf_index);
	stats.maps+=1;
}


void mmu_NRU::information_print()
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

	for(int i=0;i<pp_number;i++)
	{
		if(frametable[i]==-1)
			printf("* ");
		else
		printf("%d ",frametable[i]);
	}

	printf("\n");

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



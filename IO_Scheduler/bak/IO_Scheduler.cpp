//============================================================================
// Name        : Aaron_IO.cpp
// Author      : Yao Wang
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include "common.h" 
#include "baseclass.h"
#include "nru.h"
//#include "Schedule_FCFS.h"

bool verbose;
int selection;
bool o_option[6];

//void argument_checking(char* o_argument);

int main(int argc, char** argv) 
{
	/*input check*/
  //printf("argc=%d\n",argc);
	/*input check*/

	/*parameter information*/
	string typeofmmu;  //used to record optarg
	string input_number;
	verbose=false;
	int ch;
	int pageframe_number=32;        //if scheduler_type is R, store the quatum of round-robin
	selection=LRU;   //defalut LRU 
	char mmu_type='l';  //real variable store the type of schduler designated by user
	//printf("optind:%d opterr %d\n",optind,opterr);
	//printf("--------------------------\n");
	   while ((ch = getopt(argc, argv, "s:")) != -1)
	   {
	       //printf("optind: %d,argc:%d,argv[%d]:%s\n", optind,argc,optind,argv[optind]);
	       switch (ch) {
	       case 's':
	           //printf("HAVE option: -a\n\n");
						 //printf("optarg=%s\n",optarg);
						 typeofmmu=optarg;
						 mmu_type=typeofmmu[0];
						 //printf("mmu_type=%d\n",mmu_type);
	           break;
	       }
	   }

	   //printf("----------------------------\n");
		 //switch the type of MMU
		 /* for test*/
		 //switch(mmu_type)
		 //{
				 //case 'f':
					 //printf("f\n");
					 //break;
				 //case 's':
					 //printf("s\n");
					 ////selection=FCFS;
					 //break;
				 //case 'S':
					 //printf("S\n");
					 ////selection=LCFS;
					 //break;
				 //case 'C':
					 //printf("C\n");
					 //break;
				 //case 'F':
					 //printf("F\n");
					 //break;
				 //default:
					 //printf("input format error!\n");
					 //exit(0);
		 //}
		 /* for test*/

		 string inputfilename=argv[argc-1];
	/*parameter information*/

	MyMMU* aaron;
	aaron=new mmu_NRU(pageframe_number,mmu_type);

	// get random file for scheduler
	//for(int i=0;i<10000;i++)
	//{
		//fscanf(fp,"%d",aaron->ramdom_number+i);
		////ramdom_number[i]=temp;	
		//fprintf(fp_out,"%d\n",aaron->ramdom_number[i]);
	//}
	// get random file for scheduler

	ifstream in(inputfilename.c_str());
	istreambuf_iterator<char> beg(in), end;
	string input(beg,end);

	aaron->getString(input);
	aaron->GetSingleString();
	aaron->ProcessIO();

	delete aaron;

	return 0;

}


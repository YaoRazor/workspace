//============================================================================
// Name        : Aaron_Scheduler.cpp
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

int main(int argc, char** argv) 
{
	/*input check*/
  //printf("argc=%d\n",argc);
	/*input check*/

	/*parameter information*/
	string typeofmmu;
	string input_number;
	verbose=false;
	int ch;
	int pageframe_number=0;        //if scheduler_type is R, store the quatum of round-robin
	selection=LRU;   //defalut LRU 
	char mmu_type;  //store the type of schduler designated by user
	//printf("optind:%d opterr %d\n",optind,opterr);
	//printf("--------------------------\n");
	   while ((ch = getopt(argc, argv, "a:f:")) != -1)
	   {
	       //printf("optind: %d,argc:%d,argv[%d]:%s\n", optind,argc,optind,argv[optind]);
	       switch (ch) {
	       case 'a':
	           //printf("HAVE option: -a\n\n");
						 //printf("optarg=%s\n",optarg);
						 typeofmmu=optarg;
						 mmu_type=typeofmmu[0];
	           break;
	       case 'f':
	           //printf("HAVE option: -f\n");
						 input_number=optarg;
             pageframe_number=atoi(input_number.c_str());
	           //printf("pageframe_number=%d\n", pageframe_number);
	           break;
	       }
	   }

	   //printf("----------------------------\n");
		 //switch the type of MMU
		 switch(mmu_type)
		 {
	       case 'N':
					 //printf("RR %d\n",quantum);
					 //printf("NRU\n");
					 break;
	       case 'l':
					 //printf("LRU\n");
					 //selection=FCFS;
					 break;
	       case 'r':
					 //printf("RONDOM\n");
					 //selection=LCFS;
					 break;
	       case 'f':
					 //printf("FIFO\n");
					 break;
	       case 's':
					 //printf("SC\n");
					 break;
	       case 'c':
					 //printf("CLOCK_P\n");
					 break;
	       case 'C':
					 //printf("CLOCK_V\n");
					 break;
	       case 'a':
					 //printf("AG_P\n");
					 break;
	       case 'A':
					 //printf("AG_V\n");
					 break;
				 default:
					 //printf("input format error!\n");
					 exit(0);
		 }

		 //printf("input directory is %s\n",argv[argc-2]);  //inputfile directory
		 //printf("rfile directory is %s\n",argv[argc-1]);  //randfile directory

		 string inputfilename=argv[argc-2];
		 string randomfilename=argv[argc-1];
	/*parameter information*/

	//get random values
	FILE* fp=fopen(randomfilename.c_str(),"r");

	if(fp==NULL)
	{
		printf("open file error!Please get rfile and all the input files in upper directory\n");
		exit(0);
	}

	FILE* fp_out=fopen("../rfile_out","w");

	int numberofrandom;
	fscanf(fp,"%d",&numberofrandom);  //read the first line of random file to get the length

	MyMMU* aaron;
	aaron=new mmu_NRU(pageframe_number,mmu_type);

	// get random file for scheduler
	for(int i=0;i<10000;i++)
	{
		fscanf(fp,"%d",aaron->ramdom_number+i);
		//ramdom_number[i]=temp;	
		fprintf(fp_out,"%d\n",aaron->ramdom_number[i]);
	}
	// get random file for scheduler

	ifstream in(inputfilename.c_str());
	istreambuf_iterator<char> beg(in), end;
	string input(beg,end);
	aaron->getString(input);
	aaron->GetSingleString();
	//aaron->GetToDoList();
	aaron->Process();
	delete aaron;

	fclose(fp);
	fclose(fp_out);

	return 0;

}

/*
 * baseclass.h
 *
 *  Created on: Oct 12, 2013
 *      Author: aaronwong
 */

#ifndef BASECLASS_H_
#define BASECLASS_H_

#include "common.h"

#define Running 0
#define Blocked 1
#define Ready 2


#define NRU 0
#define LRU 1
#define RANDOM 2
#define FIFO 3
#define SC 4
#define CLOCK_P 5
#define CLOCK_V 6
#define AG_P 7
#define AG_V 8

struct PTE
{
	unsigned f1:1;// present bit
	unsigned f2:1;// modified bit
	unsigned f3:1;// referenced bit
	unsigned f4:1;// pageout bit
	unsigned f5:6;// index bit
};


struct STATS
{
	int umaps;
	int maps;
	int ins;
	int outs;
	int zeros;
};

struct INSTRUCTION_PAIR
{
	int arrive_time;
	int track_number;
};

//int instr_size;

class MyMMU
{
	public:
    int ramdom_number[10000];    //random number;
		PTE pte[64];      //pagetable
		STATS stats;      // stats information
		unsigned long long totalcost;   //variable for storing the total cost
		int instr_size;
		int random_index;  //index of random variable to generate CPU burst
		string word; //store the whole strings in the file
		vector<string> *pointer_input; //store the single string in the file
		INSTRUCTION_PAIR *instruction;

	public:
	MyMMU()
	{
		totalcost=0;
		memset(&stats,0,sizeof(STATS));

		instr_size=0;
		memset(pte,0,sizeof(PTE)*64);// Initialize all pte as zero

		/*debug*/
		//for(int i=0;i<64;i++)
		//{
			//printf("present[%d]=%d\n",i,pte[i].f1);
			//printf("modified[%d]=%d\n",i,pte[i].f2);
			//printf("referened[%d]=%d\n",i,pte[i].f3);
			//printf("pageout[%d]=%d\n",i,pte[i].f4);
			//printf("pageframe[%d]=%d\n",i,pte[i].f5);
			//printf("\n");
		//}
		

		random_index=0;

		pointer_input=new vector<string>;
		pointer_input->resize(1);
	};

	virtual ~MyMMU() 
	{
		//delete frametable;
		delete instruction;
		delete pointer_input;
		//cout<<"the destructed function of base class has been called"<<endl;
	};

  void getString(string input)
	{
		word=input;
	}

	void GetSingleString()
	{
		//int word_number=word.length();
		//cout<<word_number<<endl;
		//cout<<word<<endl;

		vector<string> temp=*pointer_input;

		unsigned int i=0; //used to track the whole file
		int j=0; //used to get the index of every single symbol
		int start_of_token=1;
		int offset_within_line=1;
		int flag=0;
		int numberofline=1;
	 
		//while(i<word.length())
		//{
		//}

		while (i<word.length())
		{
			if(word[i]=='#')
			{
				i++;
				while(word[i]!='\n')
				{
					i++;
				}
				i++;
				continue;
			}

			if((word[i]!=' ')&&(word[i]!='\t')&&(word[i]!='\n'))
			{
				if(flag==0)
				{
					start_of_token=1;
				}
				flag=1;  //flag is used for dismiss multi space or multi tab
				temp[j]+=word[i];
				//cout<<"word["<<i<<"]="<<word[i]<<endl;
				//cout<<"i="<<i<<endl;
			}
			else
			{
				if(word[i]=='\n')
				{
					numberofline++;
					//printf("test\\n");
					offset_within_line=0;
				}
				//cout<<"before j= "<<j<<endl;
				if(flag==1)
				{
				//cout<<"temp["<<j<<"]="<<temp[j]<<endl;
				j++;
				temp.resize(j+1);
				flag=0;
				}
				//cout<<"when i= "<<i<<" j plus 1"<<endl;
				//cout<<"after j= "<<j<<endl;
			}
			//cout<<"outer loop excuted"<<j<<"time"<<endl;
			//cout<<"temp["<<j<<"]="<<temp[j]<<endl;
			i++;
			offset_within_line++;
			//cout<<"i="<<i<<endl;
			//cout<<"before next loop j= "<<j<<endl;
		}
		temp.pop_back();

    
    *pointer_input=temp;	

		int instruction_number=pointer_input->size()/2;
		//printf("instruction_number=%d\n",instruction_number);
		instr_size=instruction_number;
		instruction=new INSTRUCTION_PAIR[instruction_number];
		
		for(int i=0;i<instruction_number;i++)
		{
			instruction[i].arrive_time=atoi((*pointer_input)[2*i].c_str());
			//cout<<"instruction["<<i<<"].op="<<instruction[i].arrive_time<<endl;
			instruction[i].track_number=atoi((*pointer_input)[2*i+1].c_str());
			//cout<<"instruction["<<i<<"].vp_num="<<instruction[i].track_number<<endl;
		}

		//printf("instr_size=%d\n",instr_size);

	};

	int myrandom(int burst)
	{
	  if(random_index==10000)	
		{
			random_index=0;
		}

		/*debug*/
		//printf("index=%d\n",random_index);
		//printf("random_number=%d\n",ramdom_number[random_index]);
		/*debug*/
		
		//int returnvalue=1+(ramdom_number[random_index])%burst;
		int returnvalue=(ramdom_number[random_index])%burst;
		//printf("returnvalue=%d",returnvalue);
		random_index++;
		return returnvalue; 
	};
	
  virtual void Process(){};      

	//update the present, referenced, pageout bit
	void update(int op,int vp_num)
	{
		pte[vp_num].f1=1;   //set present bit as 1

		if(op==1)
		{
			pte[vp_num].f2=op;  //if op==1, set modified as 1, if op==0, stay the same
		}

		pte[vp_num].f3=1;  //set referenced bit as 1 
	};

  virtual void ProcessIO(){};      
	//virtual void GetToDoList(){};  //Extract whole process information from the file
	//virtual void getevent(){};
	//virtual void processing(){};
	//virtual void putevent(){};
	//virtual void informationprint(){};
};





#endif /* BASECLASS_H_ */

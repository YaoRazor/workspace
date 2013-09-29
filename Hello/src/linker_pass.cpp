/*
 * linker_pass.cpp
 *
 *  Created on: Sep 23, 2013
 *      Author: aaronwong
 */
#include "linker_pass.h"

#define Map_Val(a,b) ( map<string,int>::value_type( (a),(b)) )

int linker_pass(vector<string> token)
{
	Module temp;
  vector<Module> module(1);
  
  map<string,int> symboltab;	
  TableSymbol temp_symbol;
  
	//bool isempty;
	//if(module.empty()==true)
	//{
		//cout<<"test failed"<<endl;
	//}
	//else
	//{
		//cout<<"the size of module is "<<module.size()<<endl;
	//}

	unsigned int i=0;
	int j=0;
	int offset=0;
	bool is_defcount=true;
	//bool is_deflist=false;
	bool is_usecount=false;
	//bool is_uselist=false;
	bool is_codecount=false;
	//bool is_codelist=false;

	cout<<"token size= "<<token.size()<<endl;

	while(i<token.size())
	{
		/*get deflist*/
		if(is_defcount==true)
		{
			module[j].offset=offset;
			//number of defined symbol
			 module[j].deflist.defcount=atoi(token[i].c_str());
			 i++;
			 assert(i<token.size());
			 for(int k=0;k<module[j].deflist.defcount;k++)
			 {
			   module[j].deflist.defsymbol[k].symbolname=token[i+k*2];
			   temp_symbol.symbolname=token[i+k*2];
			   module[j].deflist.defsymbol[k].symboloffest=atoi(token[i+k*2+1].c_str())+offset;
			   temp_symbol.offset=atoi(token[i+k*2+1].c_str())+offset;
				 symboltab.insert(Map_Val(temp_symbol.symbolname,temp_symbol.offset));
			 }
			 i+=module[j].deflist.defcount*2;
			 is_defcount=false;
			 is_usecount=true;
		}

		cout<<"deflist is OK"<<endl;
		/*get uselist*/
		if(is_usecount==true)
		{
			//number of used symbol
			 module[j].uselist.usecount=atoi(token[i].c_str());
			 i++;
			 assert(i<token.size());
			 for(int k=0;k<module[j].uselist.usecount;k++)
			 {
			   module[j].uselist.usesymbol[k].symbolname=token[i+k];
			 }
			 i+=module[j].uselist.usecount;
			 is_usecount=false;
			 is_codecount=true;
		}
		cout<<"uselist is OK"<<endl;

		
		/*get codelist*/
		if(is_codecount==true)
		{
			//number of instructions
			 module[j].codelist.codecount=atoi(token[i].c_str());
			 offset+=module[j].codelist.codecount;
			 i++;
			 assert(i<token.size());
			 for(int k=0;k<module[j].codelist.codecount;k++)
			 {
			   module[j].codelist.instpair[k].type=token[i+k*2];
				 string opcode;
				 string operand;

				 //module[j].codelist.instpair[k].opcode=token[i+k*2+1].substr(0,1);
				 //module[j].codelist.instpair[k].opcode=token[i+k*2+1].substr(1);
         
				 //record the size of instruction
				 module[j].codelist.instpair[k].size=token[i+k*2+1].size();
			   opcode=token[i+k*2+1].substr(0,1);
			   operand=token[i+k*2+1].substr(1);

				 module[j].codelist.instpair[k].opcode=atoi(opcode.c_str());
				 module[j].codelist.instpair[k].operand=atoi(operand.c_str());
			 }
			 i+=module[j].codelist.codecount*2;
			 //j++;
			 is_codecount=false;
			 is_defcount=true;
		}
		//cout<<"codelist is OK"<<endl;
		

		
	  if(i<token.size())
		{
			//cout<<"i= "<<i<<endl;
      //add a new module
			module.push_back(temp);
		}
		j++;
		//cout<<"loop is OK"<<endl;
	
	}
	
	cout<<"j="<<j<<endl;
	cout<<"module.size="<<module.size()<<endl;

	/* Test information*/
	//for(unsigned int p=0;p<module.size();p++)
	//{
			//cout<<"module ["<<p<<"] defcount="<<module[p].deflist.defcount<<endl;
      //for(int s=0;s<module[p].deflist.defcount;s++)
      //{
           //cout<<"defsymbol["<<s<<"] is "<<module[p].deflist.defsymbol[s].symbolname<<endl;
           //cout<<"defsymbol["<<s<<"] offset is "<<module[p].deflist.defsymbol[s].symboloffest<<endl;
      //}

			//cout<<"module ["<<p<<"] usecount="<<module[p].uselist.usecount<<endl;
      //for(int s=0;s<module[p].uselist.usecount;s++)
      //{
          //cout<<"usesymbol["<<s<<"] is "<<module[p].uselist.usesymbol[s].symbolname<<endl;
          ////cout<<"usesymbol["<<s<<"]"<<module[p].deflist.defsymbol[s].symboloffest<<endl;

      //}
	
			//cout<<"module ["<<p<<"] codecount="<<module[p].codelist.codecount<<endl;
      //for(int s=0;s<module[p].codelist.codecount;s++)
      //{
          //cout<<"instrpair["<<s<<"] type is "<<module[p].codelist.instpair[s].type<<endl;
          //cout<<"instrpair["<<s<<"] size is "<<module[p].codelist.instpair[s].size<<endl;
          //cout<<"instrpair["<<s<<"] opcode is "<<module[p].codelist.instpair[s].opcode<<endl;
          //cout<<"instrpair["<<s<<"] operand is "<<module[p].codelist.instpair[s].operand<<endl;

      //}
	//}
	/* Test information*/


  cout<<"module size= "<<module.size()<<endl;
	
	/*print Symbol Table*/
  cout<<"Symbol Table"<<endl;
  map<string,int>::iterator iter;	
	for(iter=symboltab.begin();iter!=symboltab.end();iter++)
	{
		printf("%s=%d\n",(iter->first).c_str(),iter->second);
		//cout<<iter->first<<"="<<iter->second<<endl;
	
	}
	/*print Symbol Table*/
	cout<<endl;

	/*print memory tab*/
  cout<<"Memory Table"<<endl;
	int memory_count=0;
	for(unsigned int p=0;p<module.size();p++)
	{
      for(int s=0;s<module[p].codelist.codecount;s++)
      {
					if(module[p].codelist.instpair[s].type=="I")
					{
						printf("%03d:",memory_count);
						cout<<module[p].codelist.instpair[s].opcode<<module[p].codelist.instpair[s].operand<<endl;
					  memory_count++;
					}

					if(module[p].codelist.instpair[s].type=="A")
					{
						printf("%03d:",memory_count);
						int temp=module[p].codelist.instpair[s].opcode*pow(10,module[p].codelist.instpair[s].size-1);
						temp+=module[p].codelist.instpair[s].operand;
						cout<<temp<<endl;
					  memory_count++;
					}

					if(module[p].codelist.instpair[s].type=="R")
					{
						printf("%03d:",memory_count);
						int temp=module[p].codelist.instpair[s].opcode*pow(10,module[p].codelist.instpair[s].size-1);
						temp=temp+module[p].codelist.instpair[s].operand+module[p].offset;
						cout<<temp<<endl;
					  memory_count++;
					}

					if(module[p].codelist.instpair[s].type=="E")
					{
						printf("%03d:",memory_count);


						int temp=module[p].codelist.instpair[s].opcode*pow(10,module[p].codelist.instpair[s].size-1);
						int index=module[p].codelist.instpair[s].operand;
						string a=module[p].uselist.usesymbol[index].symbolname;
            map<string,int>::iterator it=symboltab.find(a);
            temp+=it->second;
						cout<<temp<<endl;
					  memory_count++;
					}



      }
	}
	/*print memory tab*/
 
  return 0;

}




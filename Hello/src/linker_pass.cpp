/*
 * linker_pass.cpp
 *
 *  Created on: Sep 23, 2013
 *      Author: aaronwong
 */
#include "linker_pass.h"
#include "error_processing.h"
#include "warning_and_error.h"

extern Token_Info token_info[TOKEN_SIZE]; //record the line and offset information of each token

#define Map_Val(a,b) ( map<string,int>::value_type( (a),(b)) )

int linker_pass(vector<string> token)
{
	Module temp;
  vector<Module> module(1);

	vector<string> repetion_symbol;
  
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
	int total_instr=0;
	bool is_defcount=true;
	bool is_usecount=false;
	bool is_codecount=false;


  //cout<<"token size= "<<token.size()<<endl;
	//cout<<"blow is pass_one"<<endl<<endl;
	
	/*below while loop is for pass one*/

	while(i<token.size())
	{
		/*get deflist*/
		if(is_defcount==true)
		{
			module[j].offset=offset;
			//number of defined symbol
       
			//check if defcount is number
		   errorcheck_IsDigit(token[i],token_info[i].line_number,token_info[i].line_offset);

			 module[j].deflist.defcount=atoi(token[i].c_str());
      //check if there is too many symbols in definition
      errorcheck_ToManyDef(module[j].deflist.defcount,token_info[i].line_number,token_info[i].line_offset);

			 i++;
			 assert(i<token.size());
			 for(int k=0;k<module[j].deflist.defcount;k++)
			 {
				 //check if def symbol starts with alpha
				 //cout<<token[i+k*2]<<" is for debug, and token line is"<<token_info[i+k*2].line_number<<endl;
		     errorcheck_IsSymbol(token[i+k*2],token_info[i+k*2].line_number,token_info[i+k*2].line_offset);
				 //check if symbol name is too long
				 errorcheck_SymToLong(token[i+k*2],token_info[i+k*2].line_number,token_info[i+k*2].line_offset);

			   module[j].deflist.defsymbol[k].symbolname=token[i+k*2];
			   temp_symbol.symbolname=token[i+k*2];

					//check if def symbol offset is number
				 errorcheck_IsDigit(token[i+k*2+1],token_info[i+k*2+1].line_number,token_info[i+k*2+1].line_offset);

			   module[j].deflist.defsymbol[k].symboloffest=atoi(token[i+k*2+1].c_str())+offset;
			   temp_symbol.offset=atoi(token[i+k*2+1].c_str())+offset;

				 //
				 pair<map< string,int >::iterator,bool > ret;

				 ret=symboltab.insert(Map_Val(temp_symbol.symbolname,temp_symbol.offset));

				 //test if symbol is mutidefined
					if( !ret.second )
					{
						  repetion_symbol.push_back(temp_symbol.symbolname);
							//cout<<"reption symbol name is "<<temp_symbol.symbolname<<endl;
					}
				 //test if symbol is mutidefined

			 }
			 i+=module[j].deflist.defcount*2;
			 is_defcount=false;
			 is_usecount=true;
		}

		//cout<<"deflist is OK"<<endl;
		
		/*get uselist*/
		if(is_usecount==true)
		{
			//check if usecount is number
			 errorcheck_IsDigit(token[i],token_info[i].line_number,token_info[i].line_offset);
			//number of used symbol
			 module[j].uselist.usecount=atoi(token[i].c_str());
			// check if there is too many used symbols in uselist
      errorcheck_ToManyUse(module[j].uselist.usecount,token_info[i].line_number,token_info[i].line_offset);
			 i++;
			 assert(i<token.size());
			 for(int k=0;k<module[j].uselist.usecount;k++)
			 {
				 //check if use symbol starts with alpha
		     errorcheck_IsSymbol(token[i+k],token_info[i+k].line_number,token_info[i+k].line_offset);
				 //check if symbol name is too long
		     errorcheck_SymToLong(token[i+k],token_info[i+k].line_number,token_info[i+k].line_offset);
         
				 // get usesymbol name
			   module[j].uselist.usesymbol[k].symbolname=token[i+k];
				 //Init the information of usage
			   module[j].uselist.usesymbol[k].IsUsed=0;
			 }
			 i+=module[j].uselist.usecount;
			 is_usecount=false;
			 is_codecount=true;
		}
		//cout<<"uselist is OK"<<endl;

		
		/*get codelist*/
		if(is_codecount==true)
		{
			// check if codecount is number
			 errorcheck_IsDigit(token[i],token_info[i].line_number,token_info[i].line_offset);
			//number of instructions
			 module[j].codelist.codecount=atoi(token[i].c_str());

			 //check if the total number of instr is above 512
			 total_instr+=module[j].codelist.codecount;
			 errorcheck_ToManyInstr(total_instr,token_info[i].line_number,token_info[i].line_offset);

			 //get the offset of the module
			 offset+=module[j].codelist.codecount;
			 i++;
			 assert(i<token.size());
			 for(int k=0;k<module[j].codelist.codecount;k++)
			 {
				 //check if type is IARE

			   errorcheck_IsADDR(token[i+k*2],token_info[i+k*2].line_number,token_info[i+k*2].line_offset);
			   module[j].codelist.instpair[k].type=token[i+k*2];
				 string opcode;
				 string operand;

				 //module[j].codelist.instpair[k].opcode=token[i+k*2+1].substr(0,1);
				 //module[j].codelist.instpair[k].opcode=token[i+k*2+1].substr(1);
         
				 // check if instr is number
				 errorcheck_IsDigit(token[i+k*2+1],token_info[i+k*2+1].line_number,token_info[i+k*2+1].line_offset);
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
	
	//cout<<"j="<<j<<endl;
  //cout<<"module.size="<<module.size()<<endl;

	/* check the validity of the def symbols*/
	for(unsigned int p=0;p<module.size();p++)
	{
			//cout<<"module ["<<p<<"] defcount="<<module[p].deflist.defcount<<endl;
			for(int s=0;s<module[p].deflist.defcount;s++)
			{
				if((module[p].deflist.defsymbol[s].symboloffest-module[p].offset)>module[p].codelist.codecount-1)
				{
					printf("Warning: Module %d: %s to big %d (max=%d) assume zero relative\n",p+1,module[p].deflist.defsymbol[s].symbolname.c_str(),module[p].deflist.defsymbol[s].symboloffest,module[p].codelist.codecount-1); //rule5
          map<string,int>::iterator iter;	
          iter=symboltab.find(module[p].deflist.defsymbol[s].symbolname);
					iter->second=0;
          //module[p].deflist.defsymbol[s].symboloffest-1;
				}
					 //cout<<"defsymbol["<<s<<"] is "<<module[p].deflist.defsymbol[s].symbolname<<endl;
					 //cout<<"defsymbol["<<s<<"] offset is "<<module[p].deflist.defsymbol[s].symboloffest<<endl;
			}

			//cout<<"module ["<<p<<"] usecount="<<module[p].uselist.usecount<<endl;
			//for(int s=0;s<module[p].uselist.usecount;s++)
			//{
					//cout<<"usesymbol["<<s<<"] is "<<module[p].uselist.usesymbol[s].symbolname<<endl;
			//}
	
			//cout<<"module ["<<p<<"] codecount="<<module[p].codelist.codecount<<endl;
			//for(int s=0;s<module[p].codelist.codecount;s++)
			//{
					//cout<<"instrpair["<<s<<"] type is "<<module[p].codelist.instpair[s].type<<endl;
					//cout<<"instrpair["<<s<<"] size is "<<module[p].codelist.instpair[s].size<<endl;
					//cout<<"instrpair["<<s<<"] opcode is "<<module[p].codelist.instpair[s].opcode<<endl;
					//cout<<"instrpair["<<s<<"] operand is "<<module[p].codelist.instpair[s].operand<<endl;

			//}
	}
	/* check the validity of the def symbols*/


  //cout<<"module size= "<<module.size()<<endl;
	//cout<<"below is pass_two"<<endl<<endl;
	
	/*print Symbol Table*/
  cout<<"Symbol Table"<<endl;
  map<string,int>::iterator iter;	
	vector<string>::iterator find_symbol;
	for(iter=symboltab.begin();iter!=symboltab.end();iter++)
	{
		printf("%s=%d",(iter->first).c_str(),iter->second);
    
		//check if it is redefined
		find_symbol=find(repetion_symbol.begin(),repetion_symbol.end(),iter->first);
		if(find_symbol!=repetion_symbol.end())   //redefined
		{
       printf(" Error: This variable is multiple times defined; first value used\n" );
		}
		else
		{
       printf("\n");
		}
		//cout<<iter->first<<"="<<iter->second<<endl;
	
	}
	/*print Symbol Table*/
	cout<<endl;

	/*print memory tab*/
  cout<<"Memory Map"<<endl;
  vector<string> use_tab;
	int memory_count=0;
	for(unsigned int p=0;p<module.size();p++)
	{
		  for(int s=0;s<module[p].uselist.usecount;s++)
			{
				use_tab.push_back(module[p].uselist.usesymbol[s].symbolname);
			}

      for(int s=0;s<module[p].codelist.codecount;s++)
      {
					if(module[p].codelist.instpair[s].type=="I")
					{
						printf("%03d: ",memory_count);

						//rule 10 
						if(module[p].codelist.instpair[s].size>4)
						{
							cout<<"9999 ";
							_noparseerror(IIV);
					    memory_count++;
							continue;
						}
						
						int temp=module[p].codelist.instpair[s].opcode*pow(10,module[p].codelist.instpair[s].size-1);
						temp+=module[p].codelist.instpair[s].operand;
						cout<<temp<<endl;
						//cout<<module[p].codelist.instpair[s].opcode<<module[p].codelist.instpair[s].operand<<endl;
					  memory_count++;
					}

					if(module[p].codelist.instpair[s].type=="A")
					{
						printf("%03d: ",memory_count);

						//rule 11 
						if(module[p].codelist.instpair[s].size>4)
						{
							cout<<"9999"<<endl;
							_noparseerror(IO);
					    memory_count++;
							continue;
						}

						int temp=module[p].codelist.instpair[s].opcode*pow(10,module[p].codelist.instpair[s].size-1);

						//rule8
						if(module[p].codelist.instpair[s].operand>512)
						{
							cout<<temp<<" ";
							_noparseerror(AAEM);
						}
						else
						{
							temp+=module[p].codelist.instpair[s].operand;
							cout<<temp<<endl;
						}
					  memory_count++;
					}

					if(module[p].codelist.instpair[s].type=="R")
					{
						printf("%03d: ",memory_count);

						//rule 11 
						if(module[p].codelist.instpair[s].size>4)
						{
							cout<<"9999 ";
							_noparseerror(IO);
					    memory_count++;
							continue;
						}

						int temp=module[p].codelist.instpair[s].opcode*pow(10,module[p].codelist.instpair[s].size-1);

						if(module[p].codelist.instpair[s].operand>module[p].codelist.codecount-1)
						{
							temp+=module[p].offset;
							cout<<temp<<" ";
							_noparseerror(RAEM);
						}
						else
						{
							temp=temp+module[p].codelist.instpair[s].operand+module[p].offset;
							cout<<temp<<endl;
						}
					  memory_count++;
					}

					if(module[p].codelist.instpair[s].type=="E")
					{
						printf("%03d: ",memory_count);

						//rule 11 
						if(module[p].codelist.instpair[s].size>4)
						{
							cout<<"9999 ";
							_noparseerror(IO);
					    memory_count++;
							continue;
						}

						int temp=module[p].codelist.instpair[s].opcode*pow(10,module[p].codelist.instpair[s].size-1);
						int index=module[p].codelist.instpair[s].operand;

					  //rule6, check if externer address exceeds the length of the uselist
            if(index>module[p].uselist.usecount-1) 
						{
							temp+=index;
							cout<<temp<<" ";
							printf("Error: External address exceeds length of uselist; treated as immediate\n");
						}
						else
						{
							string a=module[p].uselist.usesymbol[index].symbolname;
							module[p].uselist.usesymbol[index].IsUsed=1;
							map<string,int>::iterator it=symboltab.find(a);
							if(it==symboltab.end())
							{
								//check if used symbol has been defined
								temp+=0;
								cout<<temp<<" ";
								printf("Error: %s is not defined; zero used\n",a.c_str());
							}
							else
							{
								temp+=it->second;
								cout<<temp<<endl;
							}

						}

					  memory_count++;
					}



      }
	}
	/*print memory tab*/


	/*check the relation between defintion(deflist,uselist) and actual usage*/
	//cout<<use_tab.size()<<endl;
	cout<<endl;
	for(unsigned int p=0;p<module.size();p++)
	{
		  //check if there is any symbol which is defined but not used
		  for(int s=0;s<module[p].deflist.defcount;s++)
			{
				vector<string>::iterator iter;
				iter=find(use_tab.begin(),use_tab.end(),module[p].deflist.defsymbol[s].symbolname);
				if(iter==use_tab.end())
				{
					printf("Warning: %s was defined in module %d but never used\n",module[p].deflist.defsymbol[s].symbolname.c_str(),p+1);
				}

			}

		  //check if there is any symbol which appeared in uselist but was not actually used, rule7
		  for(int s=0;s<module[p].uselist.usecount;s++)
			{
				if(module[p].uselist.usesymbol[s].IsUsed==0)
				{
					printf("Warning: In Module %d %s appeared in the uselist but was not actually used\n",p+1,module[p].uselist.usesymbol[s].symbolname.c_str());
				}

			}

	
	}
	cout<<endl;

	/*check the relation between defintion(deflist,uselist) and actual usage*/
 
  return 0;

}




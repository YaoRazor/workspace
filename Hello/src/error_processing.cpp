/*
 * error_processing.cpp
 *
 *  Created on: Sep 28, 2013
 *      Author: aaronwong
 */
#include "error_processing.h"

void _parseerror(int linenum, int lineoffset, int errcode)
{
 static char* errstr[] = { 
 "NUM_EXPECTED", // Number expect 
 "SYM_EXPECTED", // Symbol Expected 
 "ADDR_EXPECTED", // Addressing Expected 
 "SYM_TOLONG", // Symbol Name is to long 
 "TO_MANY_DEF_IN_MODULE", // more than 16 symbols in definiton 
 "TO_MANY_USE_IN_MODULE", // more than 16 symbols in uselist
 "TO_MANY_SYMBOLS",    
 "TO_MANY_INSTR",    
 }; 
 printf("Parse Error line %d offset %d: %s\n", linenum, lineoffset, errstr[errcode]); 

}

void errorcheck_IsDigit(string& str,int linenum,int lineoffset)
{
 //bool flag=true;
 for(unsigned int i=0; i<str.length(); i++)
 {
  while(!isdigit(str[i]))
  {
    _parseerror(linenum,lineoffset,NUM_EXPECTED);
		exit(0);
   //flag=false;
   //break;
  }
 }
}


void errorcheck_IsSymbol(string& str,int linenum,int lineoffset)
{
 //bool flag;
 //char temp=str[0];
 //printf("str[0]=%c\n",temp);
 if(false==isalpha(str[0]))
 {
    //printf("enter errorprocessing,linenum=%d,lineoffset=%d\n",linenum,lineoffset);
    _parseerror(linenum,lineoffset,SYM_EXPECTED);
		exit(0);
 }
 
}

void errorcheck_IsADDR(string& str,int linenum,int lineoffset)
{
 //bool flag;
 //char temp=str[0];
 //printf("str[0]=%c\n",temp);
 if(str!="A" && str!="I" && str!="R" && str!="E")
 {
    //printf("enter errorprocessing,linenum=%d,lineoffset=%d\n",linenum,lineoffset);
    _parseerror(linenum,lineoffset,ADDR_EXPECTED);
		exit(0);
 }
 
}

void errorcheck_SymToLong(string& str,int linenum,int lineoffset)
{
	  int sizeofsymbol=str.size();
		if(sizeofsymbol>16)
		{
    _parseerror(linenum,lineoffset,SYM_TOLONG);
		exit(0);
		}
}

void errorcheck_ToManyDef(int defcount,int linenum,int lineoffset)
{
		if(defcount>16)
		{
    _parseerror(linenum,lineoffset,TO_MANY_DEF_IN_MODULE);
		exit(0);
		}
}

void errorcheck_ToManyUse(int usecount,int linenum,int lineoffset)
{
		if(usecount>16)
		{
    _parseerror(linenum,lineoffset,TO_MANY_USE_IN_MODULE);
		exit(0);
		}
}

void errorcheck_ToManyInstr(int totalnumber,int linenum,int lineoffset)
{
		if(totalnumber>512)
		{
    _parseerror(linenum,lineoffset,TO_MANY_INSTR);
		exit(0);
		}
}

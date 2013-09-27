/*
 * linker_pass.h
 *
 *  Created on: Sep 23, 2013
 *      Author: aaronwong
 */

#ifndef LINKER_PASS_H_
#define LINKER_PASS_H_

#include "common.h"

//information for defined symbol
struct DefSymbol
{
string symbolname;
int symboloffest;
};

//defined symbol list for a specific model
struct DefList 
{
	int defcount;
	DefSymbol defsymbol[16];
};

//information for used symbol
struct UseSymbol
{
	string symbolname;
	bool IsUsed;
};

//used symbol list for a specific model
struct UseList
{
	int usecount;
	UseSymbol usesymbol[16];
};

//Instruction pair
struct InstrPair
{
  string type;	
	int size;
	int opcode;
	int operand;
};

//Code text
struct CodeList 
{
	int codecount;  //Max 512 for machine size
	InstrPair instpair[512];
};

struct Module
{
	DefList deflist;
	UseList uselist;
	CodeList codelist;
};

int linker_pass_one(vector<string> temp);
int linker_pass_two();
int print_symbol_table();


#endif /* LINKER_PASS_H_ */

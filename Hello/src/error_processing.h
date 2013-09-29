/*
 * error_processing.h
 *
 *  Created on: Sep 28, 2013
 *      Author: aaronwong
 */

#ifndef ERROR_PROCESSING_H_
#define ERROR_PROCESSING_H_

#include "common.h"

#define NUM_EXPECTED 0
#define SYM_EXPECTED 1 
#define ADDR_EXPECTED 2 
#define SYM_TOLONG 3 
#define TO_MANY_DEF_IN_MODULE 4 
#define TO_MANY_USE_IN_MODULE 5 
#define TO_MANY_SYMBOLS 6 
#define TO_MANY_INSTR 7 

void _parseerror(int linenum, int lineoffset, int errcode);
void errorcheck_IsDigit(string& str,int linenum,int lineoffset);
void errorcheck_IsSymbol(string& str,int linenum,int lineoffset);
void errorcheck_IsADDR(string& str,int linenum,int lineoffset);
void errorcheck_SymToLong(string& str,int linenum,int lineoffset);
void errorcheck_ToManyDef(int defcount,int linenum,int lineoffset);
void errorcheck_ToManyUse(int usecount,int linenum,int lineoffset);
void errorcheck_ToManyInstr(int totalnumber,int linenum,int lineoffset);


#endif /* ERROR_PROCESSING_H_ */

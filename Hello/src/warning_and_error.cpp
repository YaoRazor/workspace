/*
 * warning_and_error.cpp
 *
 *  Created on: Sep 29, 2013
 *      Author: aaronwong
 */

#include "warning_and_error.h"

void _noparseerror(int errcode)
{
 static char* errstr[] = { 
	  "Error: Absolute address exceeds machine size; zero used",
	  "Error: Relative address exceeds module size; zero used",
	  "Error: External address exceeds length of uselist; treated as immediate",
	  "Error: This variable is multiple times defined; first value used", 
		"Error: Illegal immediate value; treated as 9999",
		"Error: Illegal opcode; treated as 9999", 
 }; 
 printf("%s\n",errstr[errcode]); 

}


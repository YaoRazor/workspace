/*
 * common.h
 *
 *  Created on: Sep 23, 2013
 *      Author: aaronwong
 */

#ifndef COMMON_H_
#define COMMON_H_

#include <iostream>
#include <map>
#include <fstream>
#include <stdio.h>
#include <vector>
#include <assert.h>
#include <string>
#include <math.h>
#include <ctype.h>
#include <cstdlib>
#include <algorithm>


#define TOKEN_SIZE 1024

struct Token_Info
{
	int line_number;
	int line_offset;
};

using namespace std;

#endif /* COMMON_H_ */

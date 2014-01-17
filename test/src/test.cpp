//============================================================================
// Name        : test.cpp
// Author      : Yao Wang
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================
#include <unistd.h>
#include <stdio.h>
#include <string>
#include <iostream>
#include <stdlib.h>
#include <vector>
#include <bitset>
using namespace std;

class Solution {
public:
    int singleNumber(int A[], int n) {
		    int result=0;
				while(n!=0)
				{
					result^=A[n-1];
					n--;
				}

				return result;
        
    }
};

int main(int argc, char * argv[])
{
	int a[]={1,2,3,4,5,5,4,3,1};
	int result=0;
	Solution b;
	result=b.singleNumber(a,9);
	printf("result=%d\n",result);

   return 0;

}

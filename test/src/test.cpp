//============================================================================
// Name        : test.cpp
// Author      : Yao Wang
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
#include <vector>
#include <fstream>
using namespace std;

int main(int argc, char** argv)
{
	ifstream infile(argv[1]);
	    infile>>noskipws;

	string word;
	string temp[4];
	getline(infile,word); 
	cout<<word<<endl<<endl;
	int word_number=word.length();
	cout<<"word number is "<<word_number<<endl;
	
	//}
	int i=0;
	int j=0;
	while (j<4)
	{
		if((word[i]!=' ')&&(i<word.length()))
		{
			temp[j]+=word[i];
			//cout<<"word["<<i<<"]="<<word[i]<<endl;
			//cout<<"temp["<<j<<"]="<<temp[j]<<endl;
			//cout<<"i="<<i<<endl;
		}
		else
		{
			//cout<<"before j= "<<j<<endl;
			j++;
			//cout<<"when i= "<<i<<" j plus 1"<<endl;
			//cout<<"after j= "<<j<<endl;
		}
		//cout<<"outer loop excuted"<<j<<"time"<<endl;
		//cout<<"temp["<<j<<"]="<<temp[j]<<endl;
		i++;
		//cout<<"i="<<i<<endl;
		//cout<<"before next loop j= "<<j<<endl;
	}

	for(int i=0;i<4;i++)
	{
		cout<<"temp["<<i<<"]="<<temp[i]<<endl;
	}

	return 0;
}

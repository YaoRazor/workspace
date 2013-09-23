//============================================================================
// Name        : Hello.cpp
// Author      : 
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
#include <fstream>
#include <stdio.h>
#include <vector>
//#include <sstrem.h>
//#include <>
using namespace std;

int GetSingleString(string& word);

int main(int argc, char** argv)
{

//[>test input<]	  
//    int i;
//    printf("argc=%d\n",argc);
//    for (i=0; i < argc; i++)
//        printf("Argument %d is %s.\n", i, argv[i]);
//[>test input<]	 

		string a=argv[1];
		cout << a <<endl;

//		[>read all text of one file to string<]
		//ifstream in(a.c_str(), ios::in);  //original usage
		ifstream in(a.c_str());
		istreambuf_iterator<char> beg(in), end;
		string strdata(beg, end);
		cout << strdata <<endl;
		cout << "what the hell"<<endl;
		GetSingleString(strdata);
//		[>read all text of one file to string<]

		in.close();
		return 0;

}

int GetSingleString(string& word)
{

	//string word;
	vector<string> temp(1);
  //	cout<<word<<endl<<endl;
	int word_number=word.length();
	cout<<"word number is "<<word_number<<endl;
	//}
	int i=0;
	int j=0;
	int flag=0;
	int numberofline=0;
	while (i<word.length())
	{
		if((word[i]!=' ')&&(word[i]!='\t')&&(word[i]!='\n'))
		{
			flag=1;
			temp[j]+=word[i];
			//cout<<"word["<<i<<"]="<<word[i]<<endl;
			//cout<<"temp["<<j<<"]="<<temp[j]<<endl;
			//cout<<"i="<<i<<endl;
		}
		else
		{
			if(word[i]=='\n')
			{
				numberofline++;
			}
			//cout<<"before j= "<<j<<endl;
			if(flag==1)
			{
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
		//cout<<"i="<<i<<endl;
		//cout<<"before next loop j= "<<j<<endl;
	}

  temp.pop_back();
	for(int i=0;i<temp.size();i++)
	{
		cout<<"temp["<<i<<"]="<<temp[i]<<endl;
	}
  
  cout<<"temp.size="<<temp.size()<<endl;
	cout<<"numberofline="<<numberofline<<endl;

	return 0;
}


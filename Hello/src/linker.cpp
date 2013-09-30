//============================================================================
// Name        : Hello.cpp
// Author      : 
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include "common.h"
#include "linker_pass.h"

Token_Info token_info[TOKEN_SIZE];

int GetSingleString(string& word, vector<string> &temp);

int main(int argc, char** argv)
{

//[>test input<]	  
//    int i;
//    printf("argc=%d\n",argc);
//    for (i=0; i < argc; i++)
//        printf("Argument %d is %s.\n", i, argv[i]);
//[>test input<]	 

		string a=argv[1];
	//	cout << a <<endl;

//		[>read all text of one file to string<]

		//ifstream in(a.c_str(), ios::in);  //original usage
		ifstream in(a.c_str());
		istreambuf_iterator<char> beg(in), end;
		string strdata(beg, end);

		//Debug information
		//cout << strdata <<endl;

	  vector<string> allstring(1);  //construct one empty element

		GetSingleString(strdata,allstring);
    
		linker_pass(allstring);

		//Debug informaiton
		//for(int i=0;i<temp.size();i++)
		//{
			//cout<<"temp["<<i<<"]="<<temp[i]<<endl;
		//}

//		read all text of one file to string

		in.close();
		return 0;

}

int GetSingleString(string& word, vector<string> &temp)
{

	//string word;
	//vector<string> temp(1);
  //	cout<<word<<endl<<endl;
	int word_number=word.length();
	//cout<<"word number is "<<word_number<<endl;
	//}
	int i=0; //used to track the whole file
	int j=0; //used to get the index of every single symbol
	int start_of_token=1;
	int offset_within_line=1;
	int flag=0;
	int numberofline=1;
	while (i<word.length())
	{
		if((word[i]!=' ')&&(word[i]!='\t')&&(word[i]!='\n'))
		{
			if(flag==0)
			{
				start_of_token=1;
				token_info[j].line_offset=offset_within_line;
				token_info[j].line_number=numberofline;
			}
			flag=1;  //flag is used for dismiss multi space or multi tab
			temp[j]+=word[i];
			//cout<<"word["<<i<<"]="<<word[i]<<endl;
			//cout<<"i="<<i<<endl;
		}
		else
		{
			if(word[i]=='\n')
			{
				numberofline++;
				//printf("test\\n");
				offset_within_line=0;
			}
			//cout<<"before j= "<<j<<endl;
			if(flag==1)
			{
			//cout<<"temp["<<j<<"]="<<temp[j]<<endl;
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
		offset_within_line++;
		//cout<<"i="<<i<<endl;
		//cout<<"before next loop j= "<<j<<endl;
	}
  temp.pop_back();

	if((numberofline==token_info[j-1].line_number) ||(numberofline-1)==token_info[j-1].line_number)
  {
		//printf("enter\n");
	  token_info[j].line_offset=token_info[j-1].line_offset+temp[j-1].size();
	  token_info[j].line_number=token_info[j-1].line_number;
	}
  else
  {
		//printf("enter\n");
	  token_info[j].line_offset=1;
	  token_info[j].line_number=numberofline-1;
	}


  //printf("j=%d\n",j);
	//cout<<"numberofline="<<numberofline<<endl;
  //cout<<token_info[j].line_number<<endl;
  //cout<<"previous line number is "<<token_info[j-1].line_number<<endl;
	//cout<<temp[j-1]<<endl;
  //cout<<token_info[j].line_offset<<endl;

	//for(int i=0;i<temp.size();i++)
	//{
		//cout<<"token["<<i<<"]="<<temp[i]<<endl;
		//cout<<"linenumber is "<<token_info[i].line_number<<endl;
		//cout<<"offet within the line is "<<token_info[i].line_offset<<endl;
	//}
  //cout<<"temp.size="<<temp.size()<<endl;

	return 0;
}

using namespace std;

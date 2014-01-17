#include <map>
#include <iostream>

#define SIZE 0
#define SIZE2 0

int main(){

    std::map< int,int > ll;
    ll.insert( std::pair< int,int >(1,2) );

    std::pair< std::map< int,int >::iterator,bool > ret;
    ret=ll.insert( std::pair< int,int >(1,3) );

    if( ret.second ){
        std::cout<<"成功"<<std::endl;
    }
    else{
        std::cout<<"失败"<<std::endl;
    }

    return 0;
}



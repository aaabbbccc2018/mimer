/*************************************************************************
	> File Name: main.cpp
	> Author: skybosi
	> Mail: skybosi_ok@163.com 
	> Created Time: 2017年10月26日 星期四 17时30分24秒
 ************************************************************************/

#include<iostream>
#include"Stream.h"
using namespace std;

int main(int argc,char** argv)
{
	if (argc < 2){
		printf("%s filename\n",argv[0]);
		exit(0);
	}
	if(Stream::Create(argv[1]))
	{
		printf("create file: %s OK\n", argv[1]);
	}
	else
	{
		printf("create file: %s failed\n", argv[1]);
	}
	return 0;
}

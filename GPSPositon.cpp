// GPSPositon.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "RinexNavRead.h"
#include "RinexObsRead.h"
#include "stdlib.h"
#include "ComputeSatPosition.h"
#include "DataManage.h"
#include <iostream>

using namespace std;

int _tmain(int argc, _TCHAR* argv[])
{
 char strn[100],stro[100];
cout<<"单点定位与导航："<<endl;
cout<<"请输入导航文件名：";
gets(strn);
cout<<"请输入观测值文件名：";
gets(stro);

string path(stro);
GMO worun;
worun=ReadRinexObsFile(path);
string pathnav(strn);
vector<GMNREC>  nav;
 nav=ReadRinexNavFile(pathnav);
vector<PPONERESULT>  all;
all=PP(worun,nav);
 ResultOut(all);
	return 0;
}


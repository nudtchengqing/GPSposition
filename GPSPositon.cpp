// GPSPositon.cpp : �������̨Ӧ�ó������ڵ㡣
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
cout<<"���㶨λ�뵼����"<<endl;
cout<<"�����뵼���ļ�����";
gets(strn);
cout<<"������۲�ֵ�ļ�����";
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


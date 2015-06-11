#ifndef _RINEX_NAV_READ_H
#define _RINEX_NAV_READ_H


#include "stdio.h"
#include "TimeConvert.h"

#include <vector>
#include <iostream>
#include <string>
#include   <fstream>
using namespace std;


typedef struct tagGMNREC{
//////////The Zero Line
	int PRN;          //���Ǻ�

	GPSTIME  TOC;//�����ӵĲο�ʱ��
    GPSTIME  TOE;//�����Ĳο�ʱ��

	double a0;        //�����Ӳ����
	double a1;        //�����Ӳ�Ư����
	double a2;        //�����Ӳ�Ư��������
	/////The first line
	double IODE;      //�������ݵ�����
	double Crs;       //�ڹ�������������ڸ�������������
	double deltn;     //ƽ����ǵĳ��ڱ仯
	double M0;        //�ο�ʱ�̵�ƽ�����
	////The second line                       
	double Cuc;       //�ڹ���Ӽ����������ڸ�������������
	double e;         //ƫ����
	double Cus;       //�ڹ���Ӽ����������ڸ�������������
	double SqrtA;    //������
	////The third line
	 //double toe;       //�����ο�ʱ��
	double Cic;       //���������ڸ�������������
	double omiga0;    //�ο�ʱ��������ྭ������
	double Cis;       //���������ڸ�������������
	////The fourth line
	double i0;        //�ο�ʱ�̹�����
	double Crc;       //�ڹ�������������ڸ�������������
	double omiga;         //���ص�Ǿ�
	double omigaDot;     //������ྭ�ڳ��ƽ���еĳ��ڱ仯
	////The fifth line
	double iDot;         //�����Ǳ仯��
	double CodesOnL2Chanel;    //L2�ϴ�����Щ��ָʾ
	  //double weekno;    //GPS������
	double L2PdataFlag;    //L2��P��α��ָʾ
	////The sixth line
	double	SVAccuracy;     //���㲥��������ָ��
	double  SVHealth;    //�����Ƿ񽡿�ָ��
	double tgd;       //�����Ⱥ�ӳٸ�������
	double todc;      //��������������
	////The seventh line
	double TransTimeofMsg;      //��Ϣ����ʱ��
	double spare1;//����
	double spare2;
	double spare3;
}GMNREC;

typedef GMNREC *PGMNREC;

vector<GMNREC> ReadRinexNavFile(string fp);
#endif
#ifndef _RENIX_OBS_READ_H
#define _RENIX_OBS_READ_H

#include "TimeConvert.h"
#include <vector>
#include "stdio.h"
#include <iostream>
#include <string>
#include   <fstream>
using namespace std;

const int MAXNUM=12;
const int MAXOBS=9;

typedef struct tagGMOHDR
{						   
  string  FormatVersion,      //�汾��          /* A6,14X */
            FileTypeObsStr,    //�ļ�����           /* A1,19X ;first char must be O */
            PositionSystem;    //�۲�������������ϵͳ           /* A1,19X; first char G--GPS  

  double AppX,  //��־�Ľ������꣨WGS-84��
	     AppY,
	    AppZ;
  double AntHeight,//���߸�
	     AntEast,      //������������ڱ�־�ڶ����ƫ��
	     AntWest;      //������������ڱ�־�������ƫ��
  int WaveFract,  //��������
      SDFreq;
  int MeasureTypeNum;//�������ļ������洢��ͬ�۲�ֵ���͵�����
  string ObsType[MAXOBS]; //�۲�ֵ�б�
  double MeasureInterval; //�۲�ֵ����Ԫ���
  COMMONTIME   m_startTime;
  
   string timeSystem;
} GMOHDR;//�۲��ļ��ļ�ͷ

typedef GMOHDR  *PGMOHDR; 

typedef struct tagGMOREC{

	COMMONTIME epochtime;//�۲���Ԫʱ��
	int flag; //��Ԫ��־
	int satsum;//������
	string  PRN_list[MAXNUM];//�����б�
	double sat_time_bias;//���ջ�ʱ��ƫ��
	double  obsValue[MAXNUM][MAXOBS];//�۲�ֵ
	
}GMOREC;//�۲��ļ�������
typedef GMOREC *PGMOREC;

typedef struct tagGMO{
 GMOHDR hdr;
 vector<GMOREC>  obs;

}GMO;

typedef GMO *PGMO;

GMO ReadRinexObsFile(string fp);

#endif

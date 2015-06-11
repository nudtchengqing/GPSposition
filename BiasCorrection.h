#ifndef _BIASCORRECTION_H_
#define _BIASCORRECTION_H_

#include "CoordCovert.h"

const double H0=0;//�ο����ϵĸ߶�,�¶ȣ���ѹ��ʪ��
const double T0=20;
const double P0=1013.25;
const double RH0=0.50;

typedef struct tagMETEODATA{
	double pressure;
	double temperature;
	double RH;
	double height;

}METEODATA;

typedef METEODATA *PMETEODATA;

double GetTropDelay(PCRDCARTESIAN pcrdSite,PCRDCARTESIAN pcrdSat);
//pcrdSite����վ����
//pcrdSat����������
//����ֵ���������ӳ���

#endif

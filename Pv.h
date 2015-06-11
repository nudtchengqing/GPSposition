#ifndef _PV_H_
#define _PV_H_

#include "Pp.h"

typedef struct tagSITEV{
	double xv;
	double yv;
	double zv;
}SITEV;

typedef struct tagPVONERESULT{
	COMMONTIME        epoch;
	SITEV             m_sitev;            
	//double            clk_biasDOT;

}PVONERESULT;

typedef PVONERESULT *PPVONERESULT;


bool PVOne(GMOREC gmoRecord,/*�۲�ֵ��Ϣ��¼*/
		   GMO gmo,/*�۲�ֵ�ļ�*/
		   vector<GMNREC> navRecord,/*���������ļ�*/
		   PPVONERESULT pvresult/*������*/);

#endif
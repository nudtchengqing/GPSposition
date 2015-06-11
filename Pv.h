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


bool PVOne(GMOREC gmoRecord,/*观测值信息记录*/
		   GMO gmo,/*观测值文件*/
		   vector<GMNREC> navRecord,/*导航电文文件*/
		   PPVONERESULT pvresult/*输出结果*/);

#endif
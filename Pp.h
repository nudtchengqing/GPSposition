#ifndef _PP_H_
#define _PP_H_

#include "RinexObsRead.h"
#include "ComputeSatPosition.h"

typedef struct tagSITEV{
	double xv;
	double yv;
	double zv;
}SITEV;

typedef struct tagPPONERESULT{
	COMMONTIME        epoch;//历元时刻
	int               sat_num;//参与解算的卫星数
	CRDCARTESIAN      crd;//测站坐标
	double             clk_bias;//接收机钟差
	double            PDOP;
	SITEV             m_sitev;//测站速度
	int               flag;//是否能进行速度解算，0代表不能，1代表能
}PPONERESULT;

typedef PPONERESULT *PPPONERESULT;

  

bool PPOne(GMOREC gmoRecord,/*观测值信息记录*/
		   GMO gmo,/*观测值文件*/
		   vector<GMNREC> navRecord,/*导航电文文件*/
		   PPPONERESULT presult);

vector<PPONERESULT> PP(GMO gmo,/*观测值文件*/
		               vector<GMNREC> navRecord/*导航电文文件*/ );
 
#endif

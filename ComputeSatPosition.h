#ifndef _COMPUTESAT_POSITION_H
#define _COMPUTESAT_POSITION_H

#include "TimeConvert.h"
#include "CoordCovert.h"
#include "RinexNavRead.h"
#include "set.h"

typedef struct tagUtilParam {
	double n;//平均角速度
	double E;//偏近点角
	double u0;
	double vk;//真近点角
	double uk;//经过改正的升交角距
	double r;//经过改正的向径
	double i;//经过改正的轨道倾角
	double L;//改正后的升交点经度
	
} UtilParam;

typedef UtilParam *PUtilParam;

//得到最靠近所给时刻的历元单位
GMNREC GetBestGMNREC(vector<GMNREC> navRecord,
				   int nPRN,PCOMMONTIME pctEpoch);

//得到有关计算卫星空间位置，发射时刻改正，卫星速度有关的参数
//避免代码重复
void GetUtilParameter(vector<GMNREC> navRecord,
				   int nPRN,PCOMMONTIME pctEpoch,PUtilParam pParam);

//计算卫星在笛卡尔坐标系中的位置
void GetOrbNClk(vector<GMNREC> navRecord,int nPRN,
PCOMMONTIME pctEpoch, PCRDCARTESIAN pcrdOrb/*,double* pdSVClkBias*/);

//计算卫星C/A码信号发射时刻的改正
void GetSVClkBias(vector<GMNREC> navRecord,int nPRN,
PCOMMONTIME pctEpoch,double* pdSVClkBias,double *detj);

//计算卫星的速度
void GetSatVelocity(vector<GMNREC> navRecord,int nPRN,
PCOMMONTIME pctEpoch,PSatVel psatv);

#endif
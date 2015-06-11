#ifndef _COMPUTESAT_POSITION_H
#define _COMPUTESAT_POSITION_H

#include "TimeConvert.h"
#include "CoordCovert.h"
#include "RinexNavRead.h"
#include "set.h"

typedef struct tagUtilParam {
	double n;//ƽ�����ٶ�
	double E;//ƫ�����
	double u0;
	double vk;//������
	double uk;//���������������Ǿ�
	double r;//������������
	double i;//���������Ĺ�����
	double L;//������������㾭��
	
} UtilParam;

typedef UtilParam *PUtilParam;

//�õ��������ʱ�̵���Ԫ��λ
GMNREC GetBestGMNREC(vector<GMNREC> navRecord,
				   int nPRN,PCOMMONTIME pctEpoch);

//�õ��йؼ������ǿռ�λ�ã�����ʱ�̸����������ٶ��йصĲ���
//��������ظ�
void GetUtilParameter(vector<GMNREC> navRecord,
				   int nPRN,PCOMMONTIME pctEpoch,PUtilParam pParam);

//���������ڵѿ�������ϵ�е�λ��
void GetOrbNClk(vector<GMNREC> navRecord,int nPRN,
PCOMMONTIME pctEpoch, PCRDCARTESIAN pcrdOrb/*,double* pdSVClkBias*/);

//��������C/A���źŷ���ʱ�̵ĸ���
void GetSVClkBias(vector<GMNREC> navRecord,int nPRN,
PCOMMONTIME pctEpoch,double* pdSVClkBias,double *detj);

//�������ǵ��ٶ�
void GetSatVelocity(vector<GMNREC> navRecord,int nPRN,
PCOMMONTIME pctEpoch,PSatVel psatv);

#endif
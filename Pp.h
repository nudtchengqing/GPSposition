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
	COMMONTIME        epoch;//��Ԫʱ��
	int               sat_num;//��������������
	CRDCARTESIAN      crd;//��վ����
	double             clk_bias;//���ջ��Ӳ�
	double            PDOP;
	SITEV             m_sitev;//��վ�ٶ�
	int               flag;//�Ƿ��ܽ����ٶȽ��㣬0�����ܣ�1������
}PPONERESULT;

typedef PPONERESULT *PPPONERESULT;

  

bool PPOne(GMOREC gmoRecord,/*�۲�ֵ��Ϣ��¼*/
		   GMO gmo,/*�۲�ֵ�ļ�*/
		   vector<GMNREC> navRecord,/*���������ļ�*/
		   PPPONERESULT presult);

vector<PPONERESULT> PP(GMO gmo,/*�۲�ֵ�ļ�*/
		               vector<GMNREC> navRecord/*���������ļ�*/ );
 
#endif

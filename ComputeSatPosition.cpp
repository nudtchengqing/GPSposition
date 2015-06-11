#include "ComputeSatPosition.h"
#include "math.h"




GMNREC GetBestGMNREC(vector<GMNREC> navRecord,
				   int nPRN,PCOMMONTIME pctEpoch)
{
    bool flag=false;
	bool satPRN=false;
	int n=0;
	
	JULIANDAY sat_toe,sat_epoch;
    
	CommonTimeToJulianDay (pctEpoch,&sat_epoch);

	for(int i=0;i<(int)navRecord.size();i++)
	{
		if(navRecord[i].PRN==nPRN)
		{
			satPRN=true;
			GPSTimeToJulianDay (&navRecord[i].TOE, &sat_toe);

			if(GetTimeDelta (&sat_epoch,&sat_toe)<0)
			{
				return navRecord[i];
                flag=true;
				break;
			}
			else if(fabs(GetTimeDelta (&sat_epoch,&sat_toe))*_DAY_IN_SECOND<60*60)
			{
				return navRecord[i];
				flag=true;
				break;
				
			}
			n=i;

		}
		
	
	}
	if(!satPRN)
	{
		cout<<"the satPRN didn't exsit!"<<endl;
      
	}
	if(!flag)
         return navRecord[n];

	

}

double EofMe(double M,double e,double tol)
{
	double E0,E1;
	E0=M;
	E1=M+e*sin(E0);
	while(fabs(E1-E0)>tol)
	{
		E0=E1;
		E1=M+e*sin(E0);
     }
	return E1;

}

double Get_atan(double z,double y)
{
   double x;
   if (z==0) x=PI/2;
   else{
	if (y==0) x=PI;
	else{
	      x=atan(fabs(y/z));
	      if ((y>0)&&(z<0)) x=PI-x;
	      else if ((y<0)&&(z<0)) x=PI+x;
		   else if((y<0)&&(z>0)) x=2*PI-x;
	     }
       }
   return x;
}


void GetUtilParameter(vector<GMNREC> navRecord,
				   int nPRN,PCOMMONTIME pctEpoch,PUtilParam pParam)
{
	GMNREC  theBestGMN;

	theBestGMN=GetBestGMNREC(navRecord,nPRN,pctEpoch);

	//��������ƽ�����ٶ�
	double n0=sqrt(GM)/ pow(theBestGMN.SqrtA,3);

	//��������������ο���Ԫ��ʱ��

    JULIANDAY toe,epoch;
	CommonTimeToJulianDay (pctEpoch,&epoch);
	GPSTimeToJulianDay (&theBestGMN.TOE, &toe);

	double tk;
	tk=GetTimeDelta (&epoch,&toe)*_DAY_IN_SECOND;
	if(tk>302400)
		tk-=604800;
	else if(tk<-302400)
		tk+=604800;
	else
		tk=tk;

	//��ƽ�����ٶȽ��и���
	double n=n0+theBestGMN.deltn;

	(*pParam).n=n;

	//����ƽ�����
	double M=theBestGMN.M0+n*tk;

	//��ƫ�����
    double E;
	E=EofMe(M,theBestGMN.e,1e-10);

	(*pParam).E=E;

	//����������
	double vk,cosvk,sinvk;
	cosvk=(cos(E)-theBestGMN.e)/(1-theBestGMN.e*cos(E));
	sinvk=(sqrt(1-theBestGMN.e*theBestGMN.e)*sin(E))/(1-theBestGMN.e*cos(E));

//	vk=atan2(sinvk,cosvk);
//	if(vk<0)
//	 vk+=2*PI;

	vk=Get_atan(cosvk,sinvk);
	 
	(*pParam).vk=vk;
	 //���������Ǿ�
	 double u0;
	 u0=theBestGMN.omiga+vk;
     (*pParam).u0=u0;

	 //������׵��͸�����
	     //1.���������Ǿ�ĸ�����
	 double detU=theBestGMN.Cus*sin(2*u0)+theBestGMN.Cuc*cos(2*u0);
	     //2.�����򾶵ĸ�����
	 double detR=theBestGMN.Crs*sin(2*u0)+theBestGMN.Crc*cos(2*u0);
	     //3.��������Ǹ�����
	 double detI=theBestGMN.Cis*sin(2*u0)+theBestGMN.Cic*cos(2*u0);


	 //���㾭�������������Ǿ�
	 double uk=u0+detU;
	 (*pParam).uk=uk;

	 //���㾭����������
	 double r=theBestGMN.SqrtA*theBestGMN.SqrtA*(1-theBestGMN.e*cos(E))+detR;
	 (*pParam).r=r;

	 //���㾭�������Ĺ�����
	 double i=theBestGMN.i0+detI+theBestGMN.iDot*tk;
	 (*pParam).i=i;


	 //���������������㾭��
	 double L=theBestGMN.omiga0+(theBestGMN.omigaDot-we)*tk
		 -we*(theBestGMN.TOE.tow.sn+theBestGMN.TOE.tow.tos);
	 (*pParam).L=L;



}

void GetOrbNClk(vector<GMNREC> navRecord,int nPRN,
PCOMMONTIME pctEpoch, PCRDCARTESIAN pcrdOrb)
{
    
    UtilParam pParam;
    GetUtilParameter(navRecord,nPRN,pctEpoch,&pParam);

	double n=pParam.n;
	double E=pParam.E;
	double vk=pParam.vk;
	double uk=pParam.uk;
	double r=pParam.r;
	double i=pParam.i;
	double L=pParam.L;
	double u0=pParam.u0;

	//���������ڹ��ƽ���ϵ�λ��
	 double x1=r*cos(uk);
	 double y1=r*sin(uk);
	 double z1=0;

	 //�����ڵع�����ϵ�µ�λ��
	 pcrdOrb->x=cos(L)*x1-cos(i)*sin(L)*y1;
	 pcrdOrb->y=sin(L)*x1+cos(i)*cos(L)*y1;
	 pcrdOrb->z=sin(i)*y1;

	 
}

void GetSVClkBias(vector<GMNREC> navRecord,int nPRN,
PCOMMONTIME pctEpoch,double* pdSVClkBias,double *detj)
{
	UtilParam pParam;
    GetUtilParameter(navRecord,nPRN,pctEpoch,&pParam);

	GMNREC  theBestGMN;

	theBestGMN=GetBestGMNREC(navRecord,nPRN,pctEpoch);

	double E=pParam.E;
	
    JULIANDAY/* toe,*/epoch;
	CommonTimeToJulianDay (pctEpoch,&epoch);
	//GPSTimeToJulianDay (&theBestGMN.TOE, &toe);

	 //����C/A���źŷ���ʱ�̵ĸ���
	 double dettr=F*theBestGMN.e*theBestGMN.SqrtA*sin(E);

	 JULIANDAY toc;
	 GPSTimeToJulianDay (&theBestGMN.TOC, &toc);

	 double dettoc=GetTimeDelta (&epoch,&toc)*_DAY_IN_SECOND;

	 *pdSVClkBias=theBestGMN.a0+theBestGMN.a1*dettoc
		 +theBestGMN.a2*dettoc*dettoc+dettr;

	 *detj=theBestGMN.a0+theBestGMN.a1*dettoc
		 +theBestGMN.a2*dettoc*dettoc;
	 
}

 void GetSatVelocity(vector<GMNREC> navRecord,int nPRN,
PCOMMONTIME pctEpoch,PSatVel psatv)
{
	UtilParam pParam;
    GetUtilParameter(navRecord,nPRN,pctEpoch,&pParam);

	GMNREC  theBestGMN;
    theBestGMN=GetBestGMNREC(navRecord,nPRN,pctEpoch);

	double n=pParam.n;
	double E=pParam.E;
	double vk=pParam.vk;
	double uk=pParam.uk;
	double r=pParam.r;
	double i=pParam.i;
	double L=pParam.L;
	double u0=pParam.u0;

	//���������ڹ��ƽ���ϵ�λ��
	 double x1=r*cos(uk);
	 double y1=r*sin(uk);
	 double z1=0;

	

	 double Edot=n/(1-theBestGMN.e*cos(E));

     double de=sqrt(1-theBestGMN.e*theBestGMN.e)*cos(E)*(cos(E)-theBestGMN.e)+
	                sqrt(1-theBestGMN.e*theBestGMN.e)*sin(E)*sin(E);
     double dde=pow(cos(E)-theBestGMN.e,2)+(1-theBestGMN.e*theBestGMN.e)*sin(E)*sin(E);
	 double U0dot=(de/dde)*Edot;
//U0dotʵ����vk�� t����

	 double ukdot=(1+2*theBestGMN.Cus*cos(2*u0)-2*theBestGMN.Cuc*sin(2*u0))*U0dot;

	 double rkdot=theBestGMN.SqrtA*theBestGMN.SqrtA*theBestGMN.e*sin(E)*Edot
		 +2*(theBestGMN.Crs*cos(2*u0)-theBestGMN.Crc*sin(2*u0))*U0dot;

	 double Ikdot=theBestGMN.iDot+2*(theBestGMN.Cis*cos(2*u0)-theBestGMN.Cic*sin(2*u0))*U0dot;

	 double kdot=theBestGMN.omigaDot-we;//////


	 (*psatv).xv=cos(L)*(rkdot*cos(uk)-r*ukdot*sin(uk))
		      -sin(L)*cos(i)*(rkdot*sin(uk)+r*ukdot*cos(uk))
			  -(x1*sin(L)+y1*cos(L)*cos(i))*kdot
			  +y1*sin(L)*sin(i)*Ikdot;
	 (*psatv).yv=sin(L)*(rkdot*cos(uk)-r*ukdot*sin(uk))
		       +cos(L)*cos(i)*(rkdot*sin(uk)+r*ukdot*cos(uk))
			   +(x1*cos(L)-y1*sin(L)*cos(i))*kdot
			   -y1*cos(L)*sin(i)*Ikdot;
	 (*psatv).zv=sin(i)*(rkdot*sin(uk)+r*ukdot*cos(uk))
		        +y1*cos(i)*Ikdot;
}

#include "Pv.h"
#include"time.h"
#include"matrix.h"

#ifndef _NO_NAMESPACE
using namespace std;
using namespace math;
#define STD std
#else
#define STD
#endif

#ifndef _NO_TEMPLATE
typedef matrix<double> Matrix;
#else
typedef matrix Matrix;
#endif

#ifndef _NO_EXCEPTION
#  define TRYBEGIN()	try {
#  define CATCHERROR()	} catch (const STD::exception& e) { \
						cerr << "Error: " << e.what() << endl; }
#else
#  define TRYBEGIN()
#  define CATCHERROR()
#endif

bool PVOne(GMOREC gmoRecord,/*�۲�ֵ��Ϣ��¼*/
		   GMO gmo,/*�۲�ֵ�ļ�*/
		   vector<GMNREC> navRecord,/*���������ļ�*/
		   PPVONERESULT pvresult/*������*/)
{
	//	CRDCARTESIAN crdSite;//��վ����
		CRDCARTESIAN crdSat;//��������

	//double Xsat,Ysat,Zsat;//��������
//	double Xsite,Ysite,Zsite;//��վ����
		SatVel satv;//���ǵ��ٶ�
	//double Xsatv,Ysatv,Zsatv;//�����ٶ�
	double Xsitev=0,Ysitev=0,Zsitev=0;//��վ�ٶ�
	double Xsitev2=0,Ysitev2=0,Zsitev2=0;//��վ�ٶ�

	double dx,dy,dz;

	PPONERESULT m_presult;
//	PVONERESULT m_vresult;

	double Pdot=0;
	double D=0;
	double R=0;
    
	GMNREC m_nearRec;

	int len=0;

	if(PPOne(gmoRecord,gmo,navRecord,&m_presult))
		len=m_presult.sat_num;
    else
			return false;

	Matrix B(len,4),W(len,1),Result(4,1);

	COMMONTIME   tj;
    tj.year=m_presult.epoch.year;
	tj.month=m_presult.epoch.month;
	tj.day=m_presult.epoch.day;
	tj.hour=m_presult.epoch.hour;
	tj.minute=m_presult.epoch.minute;
    tj.second=m_presult.epoch.second;

	pvresult->epoch.year=m_presult.epoch.year;
	pvresult->epoch.month=m_presult.epoch.month;
	pvresult->epoch.day=m_presult.epoch.day;
	pvresult->epoch.hour=m_presult.epoch.hour;
	pvresult->epoch.minute=m_presult.epoch.minute;
	pvresult->epoch.second=m_presult.epoch.second;
	///////////////////////////////////////////////////
    
	int m_value=0;//�õ��Ĺ۲��ļ�������α��C1,P1,P2
	int m_p1=0;
	int m_p2=0;
    
	//ȷ��ʹ��α�����ڵ�λ��
	int tf=0;
	for(int type=0;type<gmo.hdr.MeasureTypeNum;type++)
	{
		if(gmo.hdr.ObsType[type].substr(0,2).compare(0,2,"D1")==0)
		{
			m_value=type;
			tf=1;
			break;
		}

		 
	}

	if(tf==0)
		return false;

	//ȷ����ʹ�õ�������Ŀ
    int bz=0,vp=0,vh=0,t=0;
	int sat_value[MAXNUM]={0};
	int sat_pos[MAXNUM]={0};
	int PN;
    
	int pos,PRN;//��Ч�����ڹ۲������е�λ��

	for(int j=0;j<gmoRecord.satsum;j++)
	{
		PN=atoi(gmoRecord.PRN_list[j].substr(1,2).c_str());
		for(int k=0;k<navRecord.size();k++)
		{
			if(PN==navRecord[k].PRN&&gmoRecord.obsValue[j][m_value]!=0)
			{
				sat_value[vp]=PN;
				vp++;
				sat_pos[vh]=j;
				vh++;
				bz=1;
				break;
			}

		}
		if(bz==0)
			t++;
		else
			bz=0;
	}

	int sat_valuesum=gmoRecord.satsum-t;
	int flag=0;
	/////////////////////////////////////////////////////
	if(sat_valuesum>=4)
	{
	 do{
		 flag++;
		 Xsitev=Xsitev2;
		 Ysitev=Ysitev2;
		 Zsitev=Zsitev2;

	//	len=sat_valuesum;

		 for(int i=0;i<len;i++)
		 {
			 PRN=sat_value[i];
			 GetOrbNClk(navRecord,PRN,&tj, &crdSat);//�������ǵ�λ��
			 GetSatVelocity(navRecord,PRN,&tj,&satv);//�������ǵ��ٶ�

			 dx=crdSat.x-m_presult.crd.x;
			 dy=crdSat.y-m_presult.crd.y;
			 dz=crdSat.z-m_presult.crd.z;

			 R=sqrt(dx*dx+dy*dy+dz*dz);
			 B(i,0)=-dx/R;
			 B(i,1)=-dy/R;
			 B(i,2)=-dz/R;
			 B(i,3)=1;

             pos=sat_pos[i];  
			  //�õ��������ʱ�̵���Ԫ��λ
             m_nearRec=GetBestGMNREC(navRecord,PRN,&tj);//���ڵõ�a1
             
			 D=(dx*satv.xv+dy*satv.yv+dz*satv.zv)/R;
             
			 Pdot=gmoRecord.obsValue[pos][m_value]*0.1903;
             
			 cout<<"�ڼ������ǣ�"<<pos<<"  ";
			 cout<<"D1:"<<gmoRecord.obsValue[pos][m_value]<<"  ";
             cout<<"��������" <<sat_valuesum<<endl;

			 W(i,0)=Pdot-D+c*m_nearRec.a1;

            cout<<"W(i,0):"<<W(i,0)<<"   "<<"Pdot:"<<Pdot<<"   "<<"D:"<<D<<endl;
			 cout<<"flag:"<<flag<<endl;
		 }

		 Result=!(~B*B)*~B*W;

         Xsitev=Result(0,0);
		 Ysitev=Result(1,0);
		 Zsitev=Result(2,0);

		
       if(flag>10)
		   break;

	}while(fabs(Zsitev-Zsitev2)>0.1);
		 
		 

		///////////////////////////////
		
	pvresult->m_sitev.xv=Xsitev;
	pvresult->m_sitev.yv=Ysitev;
	pvresult->m_sitev.zv=Zsitev;

     return true;
	}
  else
	  return false;
	

}
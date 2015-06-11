#include "Pp.h"
#include "BiasCorrection.h"
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




bool PPOne(GMOREC gmoRecord,/*�۲�ֵ��Ϣ��¼*/
		   GMO gmo,/*�۲�ֵ�ļ�*/
		   vector<GMNREC> navRecord,/*���������ļ�*/
		   PPPONERESULT presult)
{
	
	double toprelay;//�������ӳ�
    int flag=0;//�ж��Ƿ���D1���ܷ���в���
	int ncount=0;//��������
	////////////////////////////////////////////////////////////////////////////////
	int m_value=-1;//�õ��Ĺ۲��ļ�������α��C1,P1,P2��D1
	int m_p1=-1;
	int m_p2=-1;
	int m_d1=-1;
    
	//ȷ��ʹ��α�����ڵ�λ��
	for(int type=0;type<gmo.hdr.MeasureTypeNum;type++)
	{
		if(gmo.hdr.ObsType[type].substr(0,2).compare(0,2,"C1")==0)
		{
			m_value=type;
	
		}
		 if(gmo.hdr.ObsType[type].substr(0,2).compare(0,2,"P1")==0)
		{
			m_p1=type;
		
		}
		 if(gmo.hdr.ObsType[type].substr(0,2).compare(0,2,"P2")==0)
		{
			m_p2=type;
		
		}
		 if(gmo.hdr.ObsType[type].substr(0,2).compare(0,2,"D1")==0)
		{
			m_d1=type;
			flag=1;
	
		}
}

	//ȷ����ʹ�õ�������Ŀ
    int bz=0,vp=0,vh=0,t=0;
	int sat_value[MAXNUM]={0};
	int sat_pos[MAXNUM]={0};
	int PN;

	int pre=0;//�ж�α��۲�ֵ����P1,P2����C1,Ϊ0ʱ��ʾ��P1,P2.

if(m_p1!=-1&&m_p2!=-1)//�����P1,P2�Ͳ���˫Ƶ����ģ���������������ӳ�
{   
	for(int j=0;j<gmoRecord.satsum;j++)
	{
		PN=atoi(gmoRecord.PRN_list[j].substr(1,2).c_str());
		for(int k=0;k<navRecord.size();k++)
		{
			if(PN==navRecord[k].PRN&&gmoRecord.obsValue[j][m_p1]!=0&&gmoRecord.obsValue[j][m_p2]!=0)
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
}
else//���û��P1��P2����ֱ����C1����α�࣬���Ե����Ӱ��
{   
	pre=1;
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

}
	 
    int sat_valuesum=gmoRecord.satsum-t;//��������������
	 int len=sat_valuesum;
	////////////////////////////////////////////////////////////////
   
	SatVel satv;//���ǵ��ٶ�
	GMNREC m_nearRec;//�����Ԫ
	double dxx=0,dyy=0,dzz=0;
     double D,Pdot;
	 double Xsitev=0,Ysitev=0,Zsitev=0;//��վ�ٶ�
////////////////////////////////////////////////////////

	if(sat_valuesum>=4)
	{
		double X0,Y0,Z0,DetTi;
		double dx,dy,dz,dt;
		X0=gmo.hdr.AppX;
		Y0=gmo.hdr.AppY;
		Z0=gmo.hdr.AppZ;

        if(X0==0)
		{
		    X0=1000000;
		    Y0=1000000;
		    Z0=1000000;
		}
		DetTi=0;//��ʼ��ֵ
        Matrix B(len,4),W(len,1),Result(4,1),Q(4,4);
		Matrix BB(len,4),WW(len,1),RResult(4,1);
        
		double m_dop=0;
		double X,Y,Z;
		double dett2,dett1;
		double tbias,detj,R=20000000;

	   	 COMMONTIME   tj;//�źŷ���ʱ��
		 tj.year=gmoRecord.epochtime.year;
		 tj.month=gmoRecord.epochtime.month;
		 tj.day=gmoRecord.epochtime.day;
		 tj.hour=gmoRecord.epochtime.hour;
		 tj.minute=gmoRecord.epochtime.minute;
		 tj.second=gmoRecord.epochtime.second;

		 
		 JULIANDAY tjJ;//����ʱ��
		 JULIANDAY epo;//��Ԫʱ��

		///////////////////////////////
		 //��������������ӳ�
		CRDCARTESIAN crdSite;//��վ����
		CRDCARTESIAN crdSat;//��������
   
		CRDCARTESIAN pcrdOrb1;

		int PRN;//��Ч���ǵ����Ǻ�		
		double p0;//α��
	    int pos;//��Ч�����ڹ۲������е�λ��
		////////////////////////////////////////////////////
		do{
		ncount++;
		 crdSite.x =X0;
		 crdSite.y=Y0;
		 crdSite.z=Z0;
         for(int i=0;i<len;i++)
		 {
			
			 PRN=sat_value[i];
			 //��ʼ�������Ӳ�
             GetSVClkBias(navRecord,PRN,&gmoRecord.epochtime,&tbias,&detj);

             pos=sat_pos[i];
			
			 if(pre==0)//��P1,P2
                 p0=gmoRecord.obsValue[pos][m_p1]*2.54573
				     -gmoRecord.obsValue[pos][m_p2]*1.54573;
			 else//��C1
				 p0=gmoRecord.obsValue[pos][m_value];
		
			 dett2=p0/c;//����ʱ��
           
			 do
			 {
				
				 dett1=dett2;
				 //ʱ�����
				 CommonTimeToJulianDay(&gmoRecord.epochtime, &epo);
                 SetTimeDelta (&tjJ, &epo, (-tbias-dett1));
                 JulianDayToCommonTime (&tjJ, &tj);
                

				 //���������ڵѿ�������ϵ�е�λ��
		         GetOrbNClk(navRecord,PRN,&tj,&pcrdOrb1);
                 //��������C/A���źŷ���ʱ�̵ĸ���
                 GetSVClkBias(navRecord,PRN,&tj,&tbias,&detj);

		        //������ת�ĸ���
		         X=cos(dett1*we)*pcrdOrb1.x+sin(dett1*we)*pcrdOrb1.y;
		         Y=-sin(dett1*we)*pcrdOrb1.x+cos(dett1*we)*pcrdOrb1.y;
		         Z=pcrdOrb1.z;

		         crdSat.x=X;
		         crdSat.y=Y;
		         crdSat.z=Z;

		         R=sqrt((X-X0)*(X-X0)+(Y-Y0)*(Y-Y0)+(Z-Z0)*(Z-Z0));//�������
		  
		         dett2=R/c;//����ʱ��

            }while(fabs(dett2-dett1)>1e-9);

			 B(i,0)=(X0-X)/R;
			 B(i,1)=(Y0-Y)/R;
			 B(i,2)=(Z0-Z)/R;
			 B(i,3)=1;
         
			 toprelay=GetTropDelay(&crdSite,&crdSat);//�������ӳ�

			 W(i,0)=p0-R+c*detj-c*DetTi +toprelay;
		
				
//////////////////////////////////////////////////////////////
			 if(flag==1)//�Ƿ�����ٶȼ���
			 {
			  GetSatVelocity(navRecord,PRN,&tj,&satv);//�������ǵ��ٶ�
              m_nearRec=GetBestGMNREC(navRecord,PRN,&tj);//���ڵõ�a1
			  dxx=X-X0;
			  dyy=Y-Y0;
			  dzz=Z-Z0;

			 BB(i,0)=-dxx/R;
			 BB(i,1)=-dyy/R;
			 BB(i,2)=-dzz/R;
			 BB(i,3)=1;

             D=(dxx*satv.xv+dyy*satv.yv+dzz*satv.zv)/R;
			 Pdot=gmoRecord.obsValue[pos][m_d1]*c/f1;
             
			// if(fabs(Pdot-D)>50)
			//	 Pdot=-Pdot;

              WW(i,0)=-Pdot-D+c*m_nearRec.a1;
			 }
///////////////////////////////////////////////////////////////
		 
		 }
     
      
        Result=!(~B*B)*~B*W;
		Q=!(~B*B);

		dx=Result(0,0);
		dy=Result(1,0);
		dz=Result(2,0);
		dt=Result(3,0);
		X0+=dx;
		Y0+=dy;
		Z0+=dz;
        DetTi+=dt/c;

		if(ncount>10)//����10 �����ϵĵ�����Ϊʧ��
		{	
			return false;
			break;
		    }
		}while(fabs(dx)>0.1||fabs(dy)>0.1||fabs(dz)>0.1);

		for(int q=0;q<3;q++)
			m_dop+=Q(q,q);

		if(flag==1)
		{
				RResult=!(~BB*BB)*~BB*WW;
				 Xsitev=RResult(0,0);
				 Ysitev=RResult(1,0);
				 Zsitev=RResult(2,0);
		}
				 
      presult->sat_num=len;
      presult->clk_bias=DetTi;

	  presult->crd.x=X0;
	  presult->crd.y=Y0;
	  presult->crd.z=Z0;

	  presult->epoch.year=gmoRecord.epochtime.year;
	  presult->epoch.month=gmoRecord.epochtime.month;
	  presult->epoch.day=gmoRecord.epochtime.day;
	  presult->epoch.hour=gmoRecord.epochtime.hour;
	  presult->epoch.minute=gmoRecord.epochtime.minute;
	  presult->epoch.second=gmoRecord.epochtime.second;

	  presult->PDOP=sqrt(m_dop);
      
	  presult->m_sitev.xv=Xsitev;
	  presult->m_sitev.yv=Ysitev;
	  presult->m_sitev.zv=Zsitev;
      presult->flag=flag;   
	  
	   return true;
	
	}

	
	else
	{
		return false;
	}
  

}


vector<PPONERESULT> PP(GMO gmo,/*�۲�ֵ�ļ�*/
		               vector<GMNREC> navRecord/*���������ļ�*/
					   )
{
	vector<PPONERESULT> m_result;

	PPONERESULT m_oneresult;
    

	int len=gmo.obs.size();
    int k=0;
	cout<<"please wait...���ݴ�����"<<endl;
	for(int i=0;i<len-1;i++)
	{
       if(PPOne(gmo.obs[i], gmo, navRecord,&m_oneresult/*������*/))
	   {  m_result.push_back(m_oneresult);
	     

	      cout<<"k="<<k<<endl;
	   k++;
	   }
	}
	cout<<"���ݴ�����ϣ�"<<endl;

 return m_result;
}


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




bool PPOne(GMOREC gmoRecord,/*观测值信息记录*/
		   GMO gmo,/*观测值文件*/
		   vector<GMNREC> navRecord,/*导航电文文件*/
		   PPPONERESULT presult)
{
	
	double toprelay;//对流层延迟
    int flag=0;//判断是否有D1，能否进行测速
	int ncount=0;//迭代次数
	////////////////////////////////////////////////////////////////////////////////
	int m_value=-1;//用到的观测文件的哪种伪距C1,P1,P2，D1
	int m_p1=-1;
	int m_p2=-1;
	int m_d1=-1;
    
	//确定使用伪距所在的位置
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

	//确定能使用的卫星数目
    int bz=0,vp=0,vh=0,t=0;
	int sat_value[MAXNUM]={0};
	int sat_pos[MAXNUM]={0};
	int PN;

	int pre=0;//判断伪距观测值是用P1,P2还是C1,为0时表示用P1,P2.

if(m_p1!=-1&&m_p2!=-1)//如果有P1,P2就采用双频改正模型来给正电离层的延迟
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
else//如果没有P1或P2，就直接用C1计算伪距，忽略电离层影响
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
	 
    int sat_valuesum=gmoRecord.satsum-t;//参与解算的卫星数
	 int len=sat_valuesum;
	////////////////////////////////////////////////////////////////
   
	SatVel satv;//卫星的速度
	GMNREC m_nearRec;//最近历元
	double dxx=0,dyy=0,dzz=0;
     double D,Pdot;
	 double Xsitev=0,Ysitev=0,Zsitev=0;//测站速度
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
		DetTi=0;//初始化值
        Matrix B(len,4),W(len,1),Result(4,1),Q(4,4);
		Matrix BB(len,4),WW(len,1),RResult(4,1);
        
		double m_dop=0;
		double X,Y,Z;
		double dett2,dett1;
		double tbias,detj,R=20000000;

	   	 COMMONTIME   tj;//信号发射时间
		 tj.year=gmoRecord.epochtime.year;
		 tj.month=gmoRecord.epochtime.month;
		 tj.day=gmoRecord.epochtime.day;
		 tj.hour=gmoRecord.epochtime.hour;
		 tj.minute=gmoRecord.epochtime.minute;
		 tj.second=gmoRecord.epochtime.second;

		 
		 JULIANDAY tjJ;//发射时间
		 JULIANDAY epo;//历元时间

		///////////////////////////////
		 //用来计算对流层延迟
		CRDCARTESIAN crdSite;//测站坐标
		CRDCARTESIAN crdSat;//卫星坐标
   
		CRDCARTESIAN pcrdOrb1;

		int PRN;//有效卫星的卫星号		
		double p0;//伪距
	    int pos;//有效卫星在观测星历中的位置
		////////////////////////////////////////////////////
		do{
		ncount++;
		 crdSite.x =X0;
		 crdSite.y=Y0;
		 crdSite.z=Z0;
         for(int i=0;i<len;i++)
		 {
			
			 PRN=sat_value[i];
			 //初始化卫星钟差
             GetSVClkBias(navRecord,PRN,&gmoRecord.epochtime,&tbias,&detj);

             pos=sat_pos[i];
			
			 if(pre==0)//用P1,P2
                 p0=gmoRecord.obsValue[pos][m_p1]*2.54573
				     -gmoRecord.obsValue[pos][m_p2]*1.54573;
			 else//用C1
				 p0=gmoRecord.obsValue[pos][m_value];
		
			 dett2=p0/c;//传播时间
           
			 do
			 {
				
				 dett1=dett2;
				 //时间计算
				 CommonTimeToJulianDay(&gmoRecord.epochtime, &epo);
                 SetTimeDelta (&tjJ, &epo, (-tbias-dett1));
                 JulianDayToCommonTime (&tjJ, &tj);
                

				 //计算卫星在笛卡尔坐标系中的位置
		         GetOrbNClk(navRecord,PRN,&tj,&pcrdOrb1);
                 //计算卫星C/A码信号发射时刻的改正
                 GetSVClkBias(navRecord,PRN,&tj,&tbias,&detj);

		        //地球自转的改正
		         X=cos(dett1*we)*pcrdOrb1.x+sin(dett1*we)*pcrdOrb1.y;
		         Y=-sin(dett1*we)*pcrdOrb1.x+cos(dett1*we)*pcrdOrb1.y;
		         Z=pcrdOrb1.z;

		         crdSat.x=X;
		         crdSat.y=Y;
		         crdSat.z=Z;

		         R=sqrt((X-X0)*(X-X0)+(Y-Y0)*(Y-Y0)+(Z-Z0)*(Z-Z0));//计算距离
		  
		         dett2=R/c;//传播时间

            }while(fabs(dett2-dett1)>1e-9);

			 B(i,0)=(X0-X)/R;
			 B(i,1)=(Y0-Y)/R;
			 B(i,2)=(Z0-Z)/R;
			 B(i,3)=1;
         
			 toprelay=GetTropDelay(&crdSite,&crdSat);//对流层延迟

			 W(i,0)=p0-R+c*detj-c*DetTi +toprelay;
		
				
//////////////////////////////////////////////////////////////
			 if(flag==1)//是否进行速度计算
			 {
			  GetSatVelocity(navRecord,PRN,&tj,&satv);//计算卫星的速度
              m_nearRec=GetBestGMNREC(navRecord,PRN,&tj);//用于得到a1
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

		if(ncount>10)//超过10 次以上的迭代视为失败
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


vector<PPONERESULT> PP(GMO gmo,/*观测值文件*/
		               vector<GMNREC> navRecord/*导航电文文件*/
					   )
{
	vector<PPONERESULT> m_result;

	PPONERESULT m_oneresult;
    

	int len=gmo.obs.size();
    int k=0;
	cout<<"please wait...数据处理中"<<endl;
	for(int i=0;i<len-1;i++)
	{
       if(PPOne(gmo.obs[i], gmo, navRecord,&m_oneresult/*输出结果*/))
	   {  m_result.push_back(m_oneresult);
	     

	      cout<<"k="<<k<<endl;
	   k++;
	   }
	}
	cout<<"数据处理完毕！"<<endl;

 return m_result;
}


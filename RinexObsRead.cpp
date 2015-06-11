#include "RinexObsRead.h"
//头文件
const int OBS_COL_VERSION    =    0;   const int OBS_LEN_VERSION   =   9;
const int OBS_COL_TYPE       =    20;  const int OBS_LEN_TYPE      =   1;
const int OBS_COL_SYSTEM     =    40;  const int OBS_LEN_SYSTEM    =   1;
const int OBS_COL_X          =    0;   const int OBS_LEN_X         =   14;
const int OBS_COL_Y          =    14;  const int OBS_LEN_Y         =   14;
const int OBS_COL_Z          =    28;  const int OBS_LEN_Z         =   14;
const int OBS_COL_DELTAH     =    0;   const int OBS_LEN_DELTAH    =   14;
const int OBS_COL_DELTAE     =    14;  const int OBS_LEN_DELTAE    =   14;
const int OBS_COL_DELTAW     =    28;  const int OBS_LEN_DELTAW    =   14;
const int OBS_COL_WAVEL1     =    0;   const int OBS_LEN_WAVEL1    =   6;
const int OBS_COL_WAVEL2     =    6;   const int OBS_LEN_WAVEL2    =   6;

const int OBS_COL_OBSSUM     =    0;   const int OBS_LEN_OBSSUM    =   6;
const int OBS_COL_OBS1       =    10;  const int OBS_LEN_OBS1      =   2;

const int OBS_COL_INTERVAL   =    0;   const int OBS_LEN_INTERVAL  =   10;
const int OBS_COL_YEAR       =    0;   const int OBS_LEN_YEAR      =   6;
const int OBS_COL_MONTH      =    6;   const int OBS_LEN_MONTH     =   6;
const int OBS_COL_DAY        =    12;  const int OBS_LEN_DAY       =   6;
const int OBS_COL_HOUR       =    18;  const int OBS_LEN_HOUR      =   6;
const int OBS_COL_MINUTE     =    24;  const int OBS_LEN_MINUTE    =   6;
const int OBS_COL_SECOND     =    30;  const int OBS_LEN_SECOND    =   13;
const int OBS_COL_TIMESYSTEM =    48;  const int OBS_LEN_TIMESYSTEM=   3;
//观测值
const int OBS_COL_VALUE_YEAR      =    1;   const int OBS_LEN_VALUE_YEAR      =   2;
const int OBS_COL_VALUE_MONTH     =    4;   const int OBS_LEN_VALUE_MONTH     =   2;
const int OBS_COL_VALUE_DAY       =    7;   const int OBS_LEN_VALUE_DAY       =   2;
const int OBS_COL_VALUE_HOUR      =    10;  const int OBS_LEN_VALUE_HOUR      =   2;
const int OBS_COL_VALUE_MINUTE    =    13;  const int OBS_LEN_VALUE_MINUTE    =   2;
const int OBS_COL_VALUE_SECOND    =    15;  const int OBS_LEN_VALUE_SECOND    =   11;

const int OBS_COL_VALUE_FLAG      =    28;  const int OBS_LEN_VALUE_FLAG      =   1;
const int OBS_COL_VALUE_SATNUM    =    29;  const int OBS_LEN_VALUE_SATNUM    =   3;
const int OBS_COL_VALUE_SAT1      =    32;  const int OBS_LEN_VALUE_SAT1      =   3;

const int OBS_COL_VALUE_TIMEBIAS  =    68;  const int OBS_LEN_VALUE_TIMEBIAS  =   12;



GMO ReadRinexObsFile(string fp)
{
	 GMO obsData;
	 obsData.hdr.MeasureInterval=0;//因为在星历中可有可无，防止出错先赋值
     int obstypesum=0;
	 int satsum=0;
		ifstream inf;//C++方式打开星历文件
		inf.open(fp.c_str());//注意路径

		if(!inf)
		 {
		 cout<<"文件打开错误";
		 exit(0);
		 }

		string obs_header;
		getline(inf,obs_header);//读文件头

		while(obs_header.substr(60,72).compare(0,13,"END OF HEADER")!=0)
			
		{
			
			if(obs_header.substr(60,79).compare(0,20,"RINEX VERSION / TYPE")==0)
			{
				obsData.hdr.FormatVersion   =  obs_header.substr(OBS_COL_VERSION,OBS_LEN_VERSION);
				obsData.hdr.FileTypeObsStr  =  obs_header.substr(OBS_COL_TYPE,OBS_LEN_TYPE);
				obsData.hdr.PositionSystem  =  obs_header.substr(OBS_COL_SYSTEM,OBS_LEN_SYSTEM);
				//cout<<obsData.hdr.PositionSystem<<endl;
				
			}
			if(obs_header.substr(60,78).compare(0,19,"APPROX POSITION XYZ")==0)
			{
				obsData.hdr.AppX =  atof(obs_header.substr(OBS_COL_X,OBS_LEN_X).c_str());
				obsData.hdr.AppY =  atof(obs_header.substr(OBS_COL_Y,OBS_LEN_Y).c_str());
				obsData.hdr.AppZ =  atof(obs_header.substr(OBS_COL_Z,OBS_LEN_Z).c_str());
				//cout<<obsData.hdr.AppX<<"   "<<obsData.hdr.AppY<<"   "<<obsData.hdr.AppZ<<endl;
			
			}
			if(obs_header.substr(60,79).compare(0,20,"ANTENNA: DELTA H/E/N")==0)
			{
				obsData.hdr.AntHeight  =  atof(obs_header.substr(OBS_COL_DELTAH,OBS_LEN_DELTAH).c_str());
				obsData.hdr.AntEast    =  atof(obs_header.substr(OBS_COL_DELTAE,OBS_LEN_DELTAE).c_str());
				obsData.hdr.AntWest    =  atof(obs_header.substr(OBS_COL_DELTAW,OBS_LEN_DELTAW).c_str());
				//cout<<obsData.hdr.AntHeight<<"   "<<obsData.hdr.AntEast<<"   "<<obsData.hdr.AntWest<<endl;
			
			}
			if(obs_header.substr(60,79).compare(0,20,"WAVELENGTH FACT L1/2")==0)
			{
				obsData.hdr.WaveFract  =   atoi(obs_header.substr(OBS_COL_WAVEL1,OBS_LEN_WAVEL1).c_str());
				obsData.hdr.SDFreq     =   atoi(obs_header.substr(OBS_COL_WAVEL2,OBS_LEN_WAVEL2).c_str());
				//cout<<obsData.hdr.WaveFract<<"   "<<obsData.hdr.SDFreq<<endl;
			
			}
			if(obs_header.substr(60,78).compare(0,19,"# / TYPES OF OBSERV")==0)
			{
				obsData.hdr.MeasureTypeNum  =  atoi(obs_header.substr(OBS_COL_OBSSUM,OBS_LEN_OBSSUM).c_str());

                obstypesum=obsData.hdr.MeasureTypeNum;
				for(int i=0;i<obstypesum;i++)
				{
					obsData.hdr.ObsType[i]  =  obs_header.substr(10+i*6,2);
				
				}
				//cout<<obsData.hdr.MeasureTypeNum<<endl;
				//cout<<obsData.hdr.ObsType[0]<<"   "<<obsData.hdr.ObsType[1]<<endl;
			}
			if(obs_header.substr(60,67).compare(0,8,"INTERVAL")==0)
			{
				obsData.hdr.MeasureInterval  =  atof(obs_header.substr(OBS_COL_INTERVAL,OBS_LEN_INTERVAL).c_str());
				//cout<<obsData.hdr.MeasureInterval<<endl;
			}
			if(obs_header.substr(60,76).compare(0,17,"TIME OF FIRST OBS")==0)
			{
				obsData.hdr.m_startTime.year   =  atoi(obs_header.substr(OBS_COL_YEAR,OBS_LEN_YEAR).c_str());
				obsData.hdr.m_startTime.month  =  atoi(obs_header.substr(OBS_COL_MONTH,OBS_LEN_MONTH).c_str());
				obsData.hdr.m_startTime.day    =  atoi(obs_header.substr(OBS_COL_DAY,OBS_LEN_DAY).c_str());
				obsData.hdr.m_startTime.hour   =  atoi(obs_header.substr(OBS_COL_HOUR,OBS_LEN_HOUR).c_str());
				obsData.hdr.m_startTime.minute =  atoi(obs_header.substr(OBS_COL_MINUTE,OBS_LEN_MINUTE).c_str());
				obsData.hdr.m_startTime.second =  atof(obs_header.substr(OBS_COL_SECOND,OBS_LEN_SECOND).c_str());

				obsData.hdr.timeSystem         =  obs_header.substr(OBS_COL_TIMESYSTEM,OBS_LEN_TIMESYSTEM);
				//cout<<obsData.hdr.m_startTime.year<<"  "<<obsData.hdr.m_startTime.month<<"  ";
				//cout<<obsData.hdr.m_startTime.day<<"   "<<obsData.hdr.m_startTime.hour<<"   ";
				//cout<<obsData.hdr.m_startTime.minute<<"   "<<obsData.hdr.m_startTime.second<<endl;
			}

			getline(inf,obs_header);
			
		}
		
		GMOREC obs;
		int sum;//卫星的数目
		string obsvalue_header,obs_str;

		while(inf.peek() != EOF)
		{
			/*obs.epochtime.year   =0;
			obs.epochtime.month  =0;
			obs.epochtime.day    =0;
			obs.epochtime.hour   =0;
			obs.epochtime.minute =0;
			obs.epochtime.second =0;*/
			memset(&obs.epochtime,0,sizeof(obs.epochtime));
			memset(obs.PRN_list,0,sizeof(string)*12);
			memset(obs.obsValue,0,sizeof(double)*108);
			obs.flag          = 0;
			obs.sat_time_bias = 0;
			obs.satsum        = 0;//初始化，避免赋值错误
			

			//第一行
           getline(inf,obsvalue_header);
		   obs.epochtime.year=atoi(obsvalue_header.substr(OBS_COL_VALUE_YEAR,OBS_LEN_VALUE_YEAR).c_str());
		   
		   if(obs.epochtime.year>90)
			   obs.epochtime.year+=1900;
		   else
			   obs.epochtime.year+=2000;

		   obs.epochtime.month   =  atoi(obsvalue_header.substr(OBS_COL_VALUE_MONTH,OBS_LEN_VALUE_MONTH).c_str());
		   obs.epochtime.day     =  atoi(obsvalue_header.substr(OBS_COL_VALUE_DAY,OBS_LEN_VALUE_DAY).c_str());
		   obs.epochtime.hour    =  atoi(obsvalue_header.substr( OBS_COL_VALUE_HOUR,OBS_LEN_VALUE_HOUR).c_str());
		   obs.epochtime.minute  =  atoi(obsvalue_header.substr(OBS_COL_VALUE_MINUTE,OBS_LEN_VALUE_MINUTE).c_str());
		   obs.epochtime.second  =  atof(obsvalue_header.substr(OBS_COL_VALUE_SECOND,OBS_LEN_VALUE_SECOND).c_str());

		   obs.flag              =  atoi(obsvalue_header.substr(OBS_COL_VALUE_FLAG, OBS_LEN_VALUE_FLAG).c_str());
		   obs.satsum            =  atoi(obsvalue_header.substr(OBS_COL_VALUE_SATNUM,OBS_LEN_VALUE_SATNUM).c_str());

		   //cout<<obs.epochtime.year<<"  "<<obs.epochtime.month<<"   "<<obs.epochtime.day<<"   ";
		  // cout<<obs.epochtime.hour<<"   "<<obs.epochtime.minute<<"  "<<obs.epochtime.second<<endl;

		  // cout<<obs.flag<<endl;
		  // cout<<obs.satsum<<endl;
           sum  =  obs.satsum;

		   for(int sat=0;sat<sum;sat++)//读卫星列表
		   {
			   obs.PRN_list[sat] =  obsvalue_header.substr(32+sat*3,3);
			   
		   }
		  // cout<<obs.PRN_list[0]<<"   "<<obs.PRN_list[sum-1]<<endl;

		   if(obsvalue_header.size()>OBS_COL_VALUE_TIMEBIAS)//判断是否有时间偏差的数据
		        obs.sat_time_bias = atof(obsvalue_header.substr(OBS_COL_VALUE_TIMEBIAS,OBS_LEN_VALUE_TIMEBIAS).c_str());

		  // cout<<obs.sat_time_bias<<endl;

          if(obstypesum<=5)//小于5个观测值类型的情况
		  {
			  for(int satID=0;satID<sum;satID++)
			  {
				  getline(inf,obs_str);
					  for(int satValue=0;satValue<obstypesum;satValue++)
					  {
						  if((int)obs_str.size()>16*satValue)
						       obs.obsValue[satID][satValue] = atof(obs_str.substr(16*satValue,14).c_str());
						 // cout<<obs.obsValue[satID][satValue]<<endl;

					  }
			  }
		  
		  }

		  else//大于五个观测值类型的情况
		  {
			  for(int satID2=0;satID2<sum;satID2++)
			  {
				  getline(inf,obs_str);
				  for(int satValue=0;satValue<5;satValue++)
					  {
						  if((int)obs_str.size()>16*satValue)
						    obs.obsValue[satID2][satValue] = atof(obs_str.substr(16*satValue,14).c_str());

					  }
				  getline(inf,obs_str);
				  for(int satValue2=5;satValue2<obstypesum;satValue2++)
					  {
						  if((int)obs_str.size()>16*(satValue2-5))
						     obs.obsValue[satID2][satValue2] = atof(obs_str.substr(16*(satValue2-5),14).c_str());

					  }
 
			  }
		  
		  }


	 obsData.obs.push_back(obs);

		}
		
		return obsData;

}
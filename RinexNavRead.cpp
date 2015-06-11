#include "RinexNavRead.h"


//第一行数据
const int NAV_COL_PRN    =   0;   const int NAV_LEN_PRN    =  2;
const int NAV_COL_YEAR   =   3;   const int NAC_LEN_YEAR   =  2;
const int NAV_COL_MONTH  =   6;   const int NAV_LEN_MONTH  =  2;
const int NAV_COL_DAY    =   9;   const int NAV_LEN_DAY    =  2;
const int NAV_COL_HOUR   =   12;  const int NAV_LEN_HOUR   =  2;
const int NAV_COL_MINUTE =   15;  const int NAV_LEN_MINUTE =  2;
const int NAV_COL_SECONDS=   17;  const int NAV_LEN_SECONDS=  5;
const int NAV_COL_A0     =   22;  const int NAV_LEN_A0     =  19;
const int NAV_COL_A1     =   41;  const int NAV_LEN_A1     =  19;
const int NAV_COL_A2     =   60;  const int NAV_LEN_A2     =  19;

//2-7行数据
const int NAV_COL_FIRST  =   3;   const int NAV_LEN_FIRST  =  19;
const int NAV_COL_SECOND =   22;  const int NAV_LEN_SECOND =  19;
const int NAV_COL_THIRD  =   41;  const int NAV_LEN_THIRD  =  19;
const int NAV_COL_FOUR   =   60;  const int NAV_LEN_FOUR   =  19;

vector<GMNREC> ReadRinexNavFile(string fp)
{
	    GMNREC navdata;
		vector<GMNREC> navRecord;

		COMMONTIME timetran;
        double toed; 

		ifstream inf;//C++方式打开星历文件
		inf.open(fp.c_str());//注意路径

		if(!inf)
		 {
		 cout<<"文件打开错误";
		 exit(0);
		 }

		string str_header;
		getline(inf,str_header);//读文件头

		while(str_header.substr(60,72).compare(0,13,"END OF HEADER")!=0)//忽略文件头
			
		{
			getline(inf,str_header);
			
		}
		string strData0,strData1,strData2,strData3,strData4,strData5,strData6,strData7;

		

		while(inf.peek() != EOF)
		{   
					getline(inf,strData0);//读第0行
					navdata.PRN    =    atoi(strData0.substr(NAV_COL_PRN,NAV_LEN_PRN).c_str());
					/*navdata.year   =    atoi(strData0.substr(NAV_COL_YEAR,NAC_LEN_YEAR).c_str());
					     if(navdata.year>80)
								navdata.year+=1900;
							else
								navdata.year+=2000;
					navdata.month  =    atoi(strData0.substr(NAV_COL_MONTH,NAV_LEN_MONTH).c_str());
					navdata.day    =    atoi(strData0.substr(NAV_COL_DAY,NAV_LEN_DAY).c_str());
					navdata.hour   =    atoi(strData0.substr(NAV_COL_HOUR,NAV_LEN_HOUR).c_str());
					navdata.minute =    atoi(strData0.substr(NAV_COL_MINUTE,NAV_LEN_MINUTE).c_str());
					navdata.second =    atof(strData0.substr(NAV_COL_SECONDS,NAV_LEN_SECONDS).c_str());
					*/
					timetran.year   =    atoi(strData0.substr(NAV_COL_YEAR,NAC_LEN_YEAR).c_str());
					     if(timetran.year>80)
								timetran.year+=1900;
							else
								timetran.year+=2000;
					timetran.month  =    atoi(strData0.substr(NAV_COL_MONTH,NAV_LEN_MONTH).c_str());
					timetran.day    =    atoi(strData0.substr(NAV_COL_DAY,NAV_LEN_DAY).c_str());
					timetran.hour   =    atoi(strData0.substr(NAV_COL_HOUR,NAV_LEN_HOUR).c_str());
					timetran.minute =    atoi(strData0.substr(NAV_COL_MINUTE,NAV_LEN_MINUTE).c_str());
					timetran.second =    atof(strData0.substr(NAV_COL_SECONDS,NAV_LEN_SECONDS).c_str());
                    
					CommonTimeToGPSTime (&timetran, &navdata.TOC);

					navdata.a0     =    atof(strData0.substr(NAV_COL_A0,NAV_LEN_A0).c_str());
					navdata.a1     =    atof(strData0.substr(NAV_COL_A1,NAV_LEN_A1).c_str());
					navdata.a2     =    atof(strData0.substr(NAV_COL_A2,NAV_LEN_A2).c_str());

					getline(inf,strData1);//读第一行
					navdata.IODE   =    atof(strData1.substr(NAV_COL_FIRST,NAV_LEN_FIRST).c_str());
					navdata.Crs    =    atof(strData1.substr(NAV_COL_SECOND,NAV_LEN_SECOND).c_str());
					navdata.deltn  =    atof(strData1.substr(NAV_COL_THIRD,NAV_LEN_THIRD).c_str());
					navdata.M0     =    atof(strData1.substr(NAV_COL_FOUR,NAV_LEN_FOUR).c_str());

					getline(inf,strData2);//读第二行
					navdata.Cuc    =    atof(strData2.substr(NAV_COL_FIRST,NAV_LEN_FIRST).c_str());
					navdata.e      =    atof(strData2.substr(NAV_COL_SECOND,NAV_LEN_SECOND).c_str());
					navdata.Cus    =    atof(strData2.substr(NAV_COL_THIRD,NAV_LEN_THIRD).c_str());
					navdata.SqrtA  =    atof(strData2.substr(NAV_COL_FOUR,NAV_LEN_FOUR).c_str());

					getline(inf,strData3);//读第三行
					//navdata.toe    =    atof(strData3.substr(NAV_COL_FIRST,NAV_LEN_FIRST).c_str());
					toed           =    atof(strData3.substr(NAV_COL_FIRST,NAV_LEN_FIRST).c_str());
					navdata.TOE.tow.sn=static_cast<long>(toed);
					navdata.TOE.tow.tos=toed-static_cast<long>(toed);

					navdata.Cic    =    atof(strData3.substr(NAV_COL_SECOND,NAV_LEN_SECOND).c_str());
					navdata.omiga0 =    atof(strData3.substr(NAV_COL_THIRD,NAV_LEN_THIRD).c_str());
					navdata.Cis    =    atof(strData3.substr(NAV_COL_FOUR,NAV_LEN_FOUR).c_str());

					getline(inf,strData4);//读第四行
					navdata.i0       =  atof(strData4.substr(NAV_COL_FIRST,NAV_LEN_FIRST).c_str());
					navdata.Crc      =  atof(strData4.substr(NAV_COL_SECOND,NAV_LEN_SECOND).c_str());
					navdata.omiga    =  atof(strData4.substr(NAV_COL_THIRD,NAV_LEN_THIRD).c_str());
					navdata.omigaDot =  atof(strData4.substr(NAV_COL_FOUR,NAV_LEN_FOUR).c_str());

					getline(inf,strData5);//读第五行
					navdata.iDot              =  atof(strData5.substr(NAV_COL_FIRST,NAV_LEN_FIRST).c_str());
					navdata.CodesOnL2Chanel   =  atof(strData5.substr(NAV_COL_SECOND,NAV_LEN_SECOND).c_str());
					//navdata.weekno            =  atof(strData5.substr(NAV_COL_THIRD,NAV_LEN_THIRD).c_str());
					navdata.TOE.wn            =  static_cast<int>(atof(strData5.substr(NAV_COL_THIRD,NAV_LEN_THIRD).c_str()));
					navdata.L2PdataFlag       =  atof(strData5.substr(NAV_COL_FOUR,NAV_LEN_FOUR).c_str());

					getline(inf,strData6);//读第六行
					navdata.SVAccuracy  =  atof(strData6.substr(NAV_COL_FIRST,NAV_LEN_FIRST).c_str());
					navdata.SVHealth    =  atof(strData6.substr(NAV_COL_SECOND,NAV_LEN_SECOND).c_str());
					navdata.tgd         =  atof(strData6.substr(NAV_COL_THIRD,NAV_LEN_THIRD).c_str());
					navdata.todc        =  atof(strData6.substr(NAV_COL_FOUR,NAV_LEN_FOUR).c_str());

					getline(inf,strData7);//读第七行
					navdata.TransTimeofMsg  =  atof(strData7.substr(NAV_COL_FIRST,NAV_LEN_FIRST).c_str());
					navdata.spare1          =  atof(strData7.substr(NAV_COL_SECOND,NAV_LEN_SECOND).c_str());
		            navdata.spare2          =  0;
		            navdata.spare3          =  0;
//
					navRecord.push_back(navdata);

				
       
	   
		  }

         inf.close();//关闭文件


  return navRecord;

}
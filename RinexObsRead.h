#ifndef _RENIX_OBS_READ_H
#define _RENIX_OBS_READ_H

#include "TimeConvert.h"
#include <vector>
#include "stdio.h"
#include <iostream>
#include <string>
#include   <fstream>
using namespace std;

const int MAXNUM=12;
const int MAXOBS=9;

typedef struct tagGMOHDR
{						   
  string  FormatVersion,      //版本号          /* A6,14X */
            FileTypeObsStr,    //文件类型           /* A1,19X ;first char must be O */
            PositionSystem;    //观测数据所属卫星系统           /* A1,19X; first char G--GPS  

  double AppX,  //标志的近似坐标（WGS-84）
	     AppY,
	    AppZ;
  double AntHeight,//天线高
	     AntEast,      //天线中心相对于标志在东向的偏心
	     AntWest;      //天线中心相对于标志在西向的偏心
  int WaveFract,  //波长因子
      SDFreq;
  int MeasureTypeNum;//本数据文件中所存储不同观测值类型的数量
  string ObsType[MAXOBS]; //观测值列表
  double MeasureInterval; //观测值的历元间隔
  COMMONTIME   m_startTime;
  
   string timeSystem;
} GMOHDR;//观测文件文件头

typedef GMOHDR  *PGMOHDR; 

typedef struct tagGMOREC{

	COMMONTIME epochtime;//观测历元时刻
	int flag; //历元标志
	int satsum;//卫星数
	string  PRN_list[MAXNUM];//卫星列表
	double sat_time_bias;//接收机时钟偏差
	double  obsValue[MAXNUM][MAXOBS];//观测值
	
}GMOREC;//观测文件数据体
typedef GMOREC *PGMOREC;

typedef struct tagGMO{
 GMOHDR hdr;
 vector<GMOREC>  obs;

}GMO;

typedef GMO *PGMO;

GMO ReadRinexObsFile(string fp);

#endif

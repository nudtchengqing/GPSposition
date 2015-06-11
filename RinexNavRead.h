#ifndef _RINEX_NAV_READ_H
#define _RINEX_NAV_READ_H


#include "stdio.h"
#include "TimeConvert.h"

#include <vector>
#include <iostream>
#include <string>
#include   <fstream>
using namespace std;


typedef struct tagGMNREC{
//////////The Zero Line
	int PRN;          //卫星号

	GPSTIME  TOC;//卫星钟的参考时刻
    GPSTIME  TOE;//星历的参考时刻

	double a0;        //卫星钟差常数项
	double a1;        //卫星钟差漂移项
	double a2;        //卫星钟差漂移速率项
	/////The first line
	double IODE;      //星历数据的年龄
	double Crs;       //在轨道径向方向上周期改正正弦项的振幅
	double deltn;     //平近点角的长期变化
	double M0;        //参考时刻的平近点角
	////The second line                       
	double Cuc;       //在轨道延迹方向上周期改正余弦项的振幅
	double e;         //偏心率
	double Cus;       //在轨道延迹方向上周期改正正弦项的振幅
	double SqrtA;    //长半轴
	////The third line
	 //double toe;       //星历参考时刻
	double Cic;       //轨道倾角周期改正余弦项的振幅
	double omiga0;    //参考时刻升交点赤经的主项
	double Cis;       //轨道倾角周期改正正弦项的振幅
	////The fourth line
	double i0;        //参考时刻轨道倾角
	double Crc;       //在轨道径向方向上周期改正余弦项的振幅
	double omiga;         //近地点角距
	double omigaDot;     //升交点赤经在赤道平面中的长期变化
	////The fifth line
	double iDot;         //轨道倾角变化率
	double CodesOnL2Chanel;    //L2上存在哪些码指示
	  //double weekno;    //GPS星期数
	double L2PdataFlag;    //L2上P码伪距指示
	////The sixth line
	double	SVAccuracy;     //本广播星历精度指标
	double  SVHealth;    //卫星是否健康指标
	double tgd;       //典礼层群延迟改正参数
	double todc;      //卫星钟数据年龄
	////The seventh line
	double TransTimeofMsg;      //信息传送时间
	double spare1;//备用
	double spare2;
	double spare3;
}GMNREC;

typedef GMNREC *PGMNREC;

vector<GMNREC> ReadRinexNavFile(string fp);
#endif
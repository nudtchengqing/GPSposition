#include "TimeConvert.h"
#include "stdlib.h"

double FRAC(double morigin)
{
	return morigin-long(morigin);

}

double mod(double a,int b)
{
return (a-b*long(a/b));
}

void CommonTimeToJulianDay(PCOMMONTIME pct, PJULIANDAY pjd) 
{
	int		m;
	int		y;
	double	ut;

	ut	= pct->hour + pct->minute/(double)_HOUR_IN_MINUTE
		+ pct->second/(double)_HOUR_IN_SECOND;

	if (pct->month <= 2) {
		y	= pct->year - 1;
		m	= pct->month + 12;
	}
	else {
		y	= pct->year;
		m	= pct->month;
	}

	pjd->day		= (long)(365.25*y) + (long)(30.6001*(m+1))
		+ pct->day + (long)(ut/24 + 1720981.5);
	pjd->tod.sn		= ((pct->hour+12)%_DAY_IN_HOUR)*_HOUR_IN_SECOND 
		+ pct->minute*_MINUTE_IN_SECOND	+ (long)pct->second;
	pjd->tod.tos	= pct->second - (long)pct->second;
}


void JulianDayToCommonTime (PJULIANDAY pjd, PCOMMONTIME pct)
{

	int a,b,c,d,e;
	double JD;
	JD=pjd->day+(pjd->tod.sn+pjd->tod.tos)/_DAY_IN_SECOND;

	a=static_cast<int>(JD+0.5);
	b=a+1537;
	c=static_cast<int>((b-122.1)/365.25);
	d=static_cast<int>(365.25*c);
	e=static_cast<int>((b-d)/30.6001);
	/*pct->day=short(b-d-(long)(30.6001*e)+FRAC(JD+0.5));
	pct->month=short(e-1-12*(long)(e/14));
	pct->year=short(c-4715-(long)((7+pct->month)/10));

	pct->hour=(short)(pjd->tod.sn/_HOUR_IN_SECOND)+12;
    if(pct->hour>=24)
		pct->hour-=24;
	pct->minute=(short)((pjd->tod.sn%_HOUR_IN_SECOND)/_MINUTE_IN_SECOND);*/
	double day=b-d-(long)(30.6001*e)+JD+0.5-a;
	pct->day=int(day);
	pct->month=e-1-12*(int)(e/14);
	pct->year=c-4715-(int)((7+pct->month)/10);

	/*pct->hour=pjd->tod.sn/_HOUR_IN_SECOND+12;
    if(pct->hour>=24)
		pct->hour-=24;
	pct->minute=(pjd->tod.sn%_HOUR_IN_SECOND)/_MINUTE_IN_SECOND;
	pct->second=(pjd->tod.sn%_HOUR_IN_SECOND)%_MINUTE_IN_SECOND+pjd->tod.tos;
*/
	pct->hour=int((day-pct->day)*24.0);
    pct->minute=(int)(((day-pct->day)*24.0-pct->hour)*60.0);
	pct->second=pjd->tod.sn+pjd->tod.tos-(int((pjd->tod.sn+pjd->tod.tos)/60))*60.0;

	
}


void JulianDayToGPSTime (PJULIANDAY pjd, PGPSTIME pgt)
{
	double JD;
	JD=pjd->day+(pjd->tod.sn+pjd->tod.tos)/_DAY_IN_SECOND;
	pgt->wn=int((JD-2444244.5)/7);

	pgt->tow.sn=long((JD-2444244.5-pgt->wn*7)*_DAY_IN_SECOND);
	pgt->tow.tos=pjd->tod.tos;




}

void GPSTimeToJulianDay (PGPSTIME pgt, PJULIANDAY pjd)
{
	double JD;
	JD=pgt->wn*7+(pgt->tow.sn+pgt->tow.tos)/86400+2444244.5;
	pjd->day=long(JD);
	//pjd->tod.sn=long((JD-long(JD))*_DAY_IN_SECOND);

	pjd->tod.sn =long(pgt->tow.sn+(pgt->wn*7+2444244.5-pjd->day)*86400.0);
	pjd->tod.tos=pgt->tow.tos;

}

void CommonTimeToGPSTime (PCOMMONTIME pct, PGPSTIME pgt)
{
	PJULIANDAY pjd;
	pjd=(PJULIANDAY)malloc(sizeof(JULIANDAY));

	CommonTimeToJulianDay(pct,pjd);

	JulianDayToGPSTime (pjd,pgt);
	free(pjd);

}

void GPSTimeToCommonTime (PGPSTIME pgt, PCOMMONTIME pct)
{
	PJULIANDAY pjd;
	pjd=(PJULIANDAY)malloc(sizeof(JULIANDAY));

	GPSTimeToJulianDay (pgt, pjd);
	JulianDayToCommonTime (pjd, pct);
	free(pjd);

}

void CommonTimeToDOY (PCOMMONTIME pct, PDOY pdoy)
{
	PCOMMONTIME pcto;
	pcto=(PCOMMONTIME)malloc(sizeof(COMMONTIME));
	pcto->year=pct->year;
	pcto->month=1;
	pcto->day=1;
	pcto->hour=0;
	pcto->minute=0;
	pcto->second=0;

	PJULIANDAY pjdo;
	pjdo=(PJULIANDAY)malloc(sizeof(JULIANDAY));

	double JD,JDO;
	CommonTimeToJulianDay(pcto,pjdo);
	JDO=pjdo->day+(pjdo->tod.sn+pjdo->tod.tos)/_DAY_IN_SECOND;

	PJULIANDAY pjd;
	pjd=(PJULIANDAY)malloc(sizeof(JULIANDAY));
	CommonTimeToJulianDay(pct,pjd);

	JD=pjd->day+(pjd->tod.sn+pjd->tod.tos)/_DAY_IN_SECOND;

	pdoy->day=short(JD-JDO+1);
	pdoy->year=pct->year;

	pdoy->tod.sn=long(pct->hour*_HOUR_IN_SECOND
		+pct->minute*_MINUTE_IN_SECOND+pct->second);
    pdoy->tod.tos=pct->hour*_HOUR_IN_SECOND
		+pct->minute*_MINUTE_IN_SECOND+pct->second-pdoy->tod.sn;
    
	free(pcto);
	free(pjdo);
	free(pjd);

}

void DOYToCommonTime (PDOY pdoy, PCOMMONTIME pct)
{
	PCOMMONTIME pcto;
	pcto=(PCOMMONTIME)malloc(sizeof(COMMONTIME));
	pcto->year=pdoy->year;
	pcto->month=1;
	pcto->day=1;
	pcto->hour=0;
	pcto->minute=0;
	pcto->second=0;

	PJULIANDAY pjdo;
	pjdo=(PJULIANDAY)malloc(sizeof(JULIANDAY));

    double JD,JDO;
	CommonTimeToJulianDay(pcto,pjdo);
	JDO=pjdo->day+(pjdo->tod.sn+pjdo->tod.tos)/_DAY_IN_SECOND;

	JD=JDO+pdoy->day+(pdoy->tod.sn+pdoy->tod.tos)/_DAY_IN_SECOND-1;
    
	long a,b,c,d,e;
	
	a=(long)(JD+0.5);
	b=a+1537;
	c=(long)((b-122.1)/365.25);
	d=(long)(365.25*c);
	e=(long)((b-d)/30.6001);
	pct->day=short(b-d-(long)(30.6001*e)+FRAC(JD+0.5));
	pct->month=short(e-1-12*(long)(e/14));
	pct->year=short(c-4715-(long)((7+pct->month)/10));

	pct->hour=short((pdoy->tod.sn+pdoy->tod.tos)/_HOUR_IN_SECOND);
	pct->minute=short((pdoy->tod.sn+pdoy->tod.tos
		-pct->hour*_HOUR_IN_SECOND)/_MINUTE_IN_SECOND);
	pct->second=pdoy->tod.sn+pdoy->tod.tos
		-pct->hour*_HOUR_IN_SECOND-pct->minute*_MINUTE_IN_SECOND;

	free(pcto);
	free(pjdo);

}

void GPSTimeToDOY (PGPSTIME pgt, PDOY pdoy)
{
	PJULIANDAY pjd;
	pjd=(PJULIANDAY)malloc(sizeof(JULIANDAY));

	GPSTimeToJulianDay (pgt, pjd);
    
	PCOMMONTIME pct;
	pct=(PCOMMONTIME)malloc(sizeof(COMMONTIME));
	JulianDayToCommonTime (pjd,pct);

    CommonTimeToDOY (pct,pdoy);
	free(pjd);
	free(pct);

}

void DOYToGPSTime (PDOY pdoy, PGPSTIME pgt)
{
    PCOMMONTIME pct;
	pct=(PCOMMONTIME)malloc(sizeof(COMMONTIME));
	DOYToCommonTime (pdoy,pct);

    CommonTimeToGPSTime (pct, pgt);
	free(pct);

}

void JulianDayToDOY (PJULIANDAY pjd, PDOY pdoy)
{ 
	PCOMMONTIME pct;
	pct=(PCOMMONTIME)malloc(sizeof(COMMONTIME));
	JulianDayToCommonTime (pjd, pct);

	CommonTimeToDOY (pct,pdoy);
	free(pct);
}

void DOYToJulianDay (PDOY pdoy, PJULIANDAY pjd)
{
	PCOMMONTIME pct;
	pct=(PCOMMONTIME)malloc(sizeof(COMMONTIME));
	DOYToCommonTime (pdoy,pct);
   
	CommonTimeToJulianDay(pct,pjd);
	free(pct);
}

//
//IF pjd1>pjd2 OUT delta=pjd1-pjd2

double GetTimeDelta (PJULIANDAY pjd1, PJULIANDAY pjd2)
{

	double JD1,JD2;
	JD1=pjd1->day+(pjd1->tod.sn+pjd1->tod.tos)/_DAY_IN_SECOND;
	JD2=pjd2->day+(pjd2->tod.sn+pjd2->tod.tos)/_DAY_IN_SECOND;

	return JD1-JD2;
	
}

//pjdNew=pjdOld+dDelta
void SetTimeDelta (PJULIANDAY pjdNew, PJULIANDAY pjdOld, double dDelta)
{
	double JDold,JDnew;
	JDold=pjdOld->day+(pjdOld->tod.sn+pjdOld->tod.tos)/_DAY_IN_SECOND;
	JDnew=JDold+dDelta/_DAY_IN_SECOND;

	pjdNew->day=long(JDnew);
	pjdNew->tod.sn=long((JDnew-long(JDnew))*_DAY_IN_SECOND);
	pjdNew->tod.tos=(JDnew-long(JDnew))*_DAY_IN_SECOND
		-long((JDnew-long(JDnew))*_DAY_IN_SECOND);

}


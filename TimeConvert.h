#ifndef	__TIMECONVERT_H
#define	__TIMECONVERT_H


const long	_MINUTE_IN_SECOND	= 60L;
const long	_HOUR_IN_MINUTE		= 60L;
const long	_DAY_IN_HOUR		= 24L;
const long	_WEEK_IN_DAY		= 7L;
const long	_HOUR_IN_SECOND		= _HOUR_IN_MINUTE * _MINUTE_IN_SECOND;
const long	_DAY_IN_SECOND		= _DAY_IN_HOUR * _HOUR_IN_SECOND;
const long	_WEEK_IN_SECOND		= _WEEK_IN_DAY * _DAY_IN_SECOND;


typedef struct tagCOMMONTIME {
	unsigned short	year;
	unsigned short	month;
	unsigned short	day;
	unsigned short	hour;
	unsigned short	minute;
	
	/*unsigned char	month;
	unsigned char	day;
	unsigned char	hour;
	unsigned char	minute;*/
	double			second;
} COMMONTIME;

typedef COMMONTIME	*PCOMMONTIME;


typedef struct tagTOD {
	long	sn;
	double	tos;
} TOD;

typedef TOD	*PTOD;


typedef struct tagJULIANDAY {
	long	day;
	TOD		tod;
} JULIANDAY;

typedef JULIANDAY	*PJULIANDAY;


typedef struct tagMJulianDay {
	long	day;
	TOD		tod;
} MJulianDay;

typedef MJulianDay	*PMJulianDay;


typedef struct tagTOW {
	long	sn;
	double	tos;
} TOW;

typedef TOW	*PTOW;


typedef struct tagGPSTIME {
	int		wn;
	TOW		tow;
} GPSTIME;

typedef GPSTIME	*PGPSTIME;


typedef struct tagDOY {
	unsigned short	year;
	unsigned short	day;
	TOD				tod;
} DOY;

typedef DOY	*PDOY;


void CommonTimeToJulianDay (PCOMMONTIME pct, PJULIANDAY pjd);
void JulianDayToCommonTime (PJULIANDAY pjd, PCOMMONTIME pct);
void JulianDayToGPSTime (PJULIANDAY pjd, PGPSTIME pgt);
void GPSTimeToJulianDay (PGPSTIME pgt, PJULIANDAY pjd);
void CommonTimeToGPSTime (PCOMMONTIME pct, PGPSTIME pgt);
void GPSTimeToCommonTime (PGPSTIME pgt, PCOMMONTIME pct);
void CommonTimeToDOY (PCOMMONTIME pct, PDOY pdoy);
void DOYToCommonTime (PDOY pdoy, PCOMMONTIME pct);
void GPSTimeToDOY (PGPSTIME pgt, PDOY pdoy);
void DOYToGPSTime (PDOY pdoy, PGPSTIME pgt);
void JulianDayToDOY (PJULIANDAY pjd, PDOY pdoy);
void DOYToJulianDay (PDOY pdoy, PJULIANDAY pjd);
double GetTimeDelta (PJULIANDAY pjd1, PJULIANDAY pjd2);
void SetTimeDelta (PJULIANDAY pjdNew, PJULIANDAY pjdOld, double dDelta);


#endif


#ifndef _SAT_H_
#define _SAT_H_

const double PI=3.1415926535898;//圆周率
const double c=2.99792458e+08;//光速
const double GM=3.986005e+14;//重力常数
const double we=7.2921151467e-05;//地球自转角速度
const double F=-4.442807633e-10;
const double f1=1575.42e+6;

typedef struct tagSatVelocity {
	double xv;
	double yv;
	double zv;
} SatVel;

typedef SatVel	*PSatVel;


#endif
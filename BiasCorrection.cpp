#include "BiasCorrection.h"
#include "math.h"
#include "set.h"

double GetTropDelay(PCRDCARTESIAN pcrdSite,PCRDCARTESIAN pcrdSat)
{
	CRDGEODETIC crdSite;
	CartesianToGeodetic (&crdSite, pcrdSite,a,flattening);

	double H=crdSite.height;

	CRDTOPOCENTRIC  pct;

	CartesianToTopocentric (&pct,pcrdSat,pcrdSite,a,flattening);
    
	CRDTOPOCENTRICPOLAR site;
    TopocentricToTopocentricPolar (&site,&pct);

	double E=site.elevation;
	double delta=0;

	if(fabs(H)<40000)
	{
	
	double T=T0-0.0065*(H-H0)+273.16;
	double P=P0*pow((1-0.0000226*(H-H0)),5.225);
	double RH=RH0*exp(-0.0006396*(H-H0));

	double e=RH*exp(-37.2465+0.213166*T-0.000256908*T*T);
	double hw=11000;
	double hd=40136+148.72*(T-273.16);

	double Kw=(155.2e-7*4810*e*(hw-H))/(T*T);
	double Kd=(155.2e-7*P*(hd-H))/T;
	 delta=Kd/(sin(sqrt(E*E+6.25)))+Kw/(sin(sqrt(E*E+2.25)));

	}
	else
     delta=0;
	return delta;
}
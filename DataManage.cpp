#include "DataManage.h"

void ResultOut(vector<PPONERESULT> m_result)
{
	FILE *fp;
	if((fp=fopen("Result.dat","w"))==NULL)
	{
	    printf("失败\n");
	    exit(0);
	}
    
	char dis1[100]="1#如果观测值星历中有P1,P2则采用双频改正电离层误差，\n如果没有则直接用C1观测值，忽略电离层误差\n";
	char dis2[150]="2#如果速度项值为0，则表示观测值星历中没有D1观测值，\n无法进行速度解算，速度初始为0.值为1表示能正常解算速度\n";
    char year[10]="年  ,";
	char month[10]="月,";
	char day[10]="日,";
	char hour[10]="时,";
	char minute[10]="分,";
	char second[10]=" 秒 ,";

	char X[20]="      X(m)       ";
	char Y[20]="      Y(m)       ";
	char Z[20]="      Z(m)       ";

	char flag[20]="速度";
	
	char xv[20]="  XV(m/s) ";
	char yv[20]=" YV(m/s) ";
	char zv[20]="  ZV(m/s)  ";
	char PDOP[10]=" PDOP  ";
	char num[10]="卫星数";
	char hang[2]="\n"; 
	 
	fputs(dis1,fp);
	fputs(dis2,fp);

	fputs(year,fp);
	fputs(month,fp);
	fputs(day,fp);
	fputs(hour,fp);
	fputs(minute,fp);
	fputs(second,fp);
	fputs(X,fp);fputs(Y,fp);fputs(Z,fp);
	fputs(flag,fp);
	fputs(xv,fp);fputs(yv,fp);fputs(zv,fp);
	fputs(PDOP,fp);fputs(num,fp);
		fputs(hang,fp);

	int len=m_result.size();
	cout<<"please wait...文件件写入中"<<endl;

	for(int i=0;i<len;i++)
	{
		fprintf(fp,"%4d,%2d,%2d,%2d,%2d,%6.3lf,",m_result[i].epoch.year,
			m_result[i].epoch.month,m_result[i].epoch.day,m_result[i].epoch.hour,
			m_result[i].epoch.minute,m_result[i].epoch.second);
		fprintf(fp,"%14.3lf,%14.3lf,%14.3lf ",m_result[i].crd.x,m_result[i].crd.y,
			m_result[i].crd.z);
		//fprintf(fp,"%14.10lf ",m_result[i].clk_bias);
		fprintf(fp,"%6d",m_result[i].flag);
		fprintf(fp,"%10.3lf",m_result[i].m_sitev.xv);
		fprintf(fp,"%10.3lf",m_result[i].m_sitev.yv);
		fprintf(fp,"%10.3lf",m_result[i].m_sitev.zv);
		fprintf(fp,"%8.3lf",m_result[i].PDOP);
		fprintf(fp,"%3d\n",m_result[i].sat_num);
       //cout<<i<<endl;
	
	}
	

	 cout<<"文件生成完毕!"<<endl;
	fclose(fp);

}
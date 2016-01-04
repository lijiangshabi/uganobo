#include<stdio.h>
#include<math.h>
#include<stdlib.h>
#define PI 3.141592653
double fact[72][2];
double theory[72][2];
int  Revit_degree1[3]={120,165,180},Revit_degree2[3]={210,215,250},Revit_degree3[3]={300,
315,330};       //已知参数
double Revit_h[3]={28,30,35},Revit_rb[3]={14,19,21};/*推杆行程*/
double Revit_A1[3]={30,30,35},Revit_A2[3]={70,75,65};
double Revit_P[3]={13,18.2,24.3},Revit_e[3]={5,10,14};/*偏距*/
double Revit_r[3]={10,14,18};/*滚子半径*/
double b=1;
int k=0;           //选择组别 0-A 1-B 2-C
int ang1= Revit_degree1[k],ang2= Revit_degree2[k],ang3= Revit_degree3[k];
double h=Revit_h[k], rb=Revit_rb[k];
double A1=Revit_A1[k]*PI/180,A2=Revit_A2[k]*PI/180;
double r=Revit_r[k],e=Revit_e[k],P=Revit_P[k];
double So;

double S(int I)
{
	double s;
	double A;
	double B;
	if(I<=ang1/2)
	{
		A=I*PI/180;
		B=ang1*PI/180;
		s=2*h*pow(A/B,2);
	}
	else if((I>ang1/2)&&(I<=ang1))
	{
		A=I*PI/180;
		B=ang1*PI/180;
		s=h-2*h*pow((B-A)/B,2);
	}
	else if(I<=ang2)s=h;
	      else if(I<=ang3)
		      {
			     A=(I-ang2)*PI/180;
				 B=(ang3-ang2)*PI/180;
				 s=h*(1+cos(PI*A/B))/2;
		      }
		      else s=0;
	return(s);
}
double ds(int Q)
{
	double A,B,C;
	if(Q<=ang1/2)
	{
		A=Q*PI/180;
		B=ang1*PI/180;
		C=4*h*A/(B*B);
	}
	else if((Q>ang1/2)&&(Q<=ang1))
	{
		A=Q*PI/180;
		B=ang1*PI/180;
		C=4*h*(B-A)/(B*B);
	}
	else if(Q<=ang2) C=0;
	    else if(Q<=ang3)
		{
	    A=(Q-ang2)*PI/180;
		B=(ang3-ang2)*PI/180;
		C=-PI*h*sin(PI*A/B)/(2*B);
		}
	  else C=0;
	  return C;
}
double dss(int B3)
{
	double A,B,C;
	if(B3<=ang1/2)
	{
		A=B3*PI/180;
		C=ang1*PI/180;
		B=4*h/(C*C);
	}
	else if(B3>ang1/2&&B3<=ang1)
	{
		A=B3*PI/180;
		C=ang1*PI/180;
		B=-4*h/(C*C);
	}
	else if(B3<=ang2)B=0;
	    else if(B3<=ang3)
		{
		A=(B3-ang2)*PI/180;
		C=(ang3-ang2)*PI/180;
		B=-PI*PI*h*cos(PI*A/C)/(2*C*C);
		}
        else B=0;
	return(B);
}
void xy(int ang)        //计算点坐标
{
	double A,B,C,E,F,dx,dy;
	A=ang*PI/180;
	B=S(ang);
	C=ds(ang);
    dx=(So+B)*cos(A)+sin(A)*(C-e);
    dy=-sin(A)*(So+B)+cos(A)*(C-e);
	E=-r*dy/sqrt(dx*dx+dy*dy);
	F=r*dx/sqrt(dx*dx+dy*dy);
	theory[ang/5][0]=(So+B)*sin(A)+e*cos(A);
	theory[ang/5][1]=(So+B)*cos(A)-e*sin(A);
	fact[ang/5][0]=theory[ang/5][0]-E;
	fact[ang/5][1]=theory[ang/5][1]-F;
}
double a(int B1)       //计算压力角
{
	double A,B;
	A=sqrt((ds(B1)-e)*(ds(B1)-e));
	B=S(B1);
	return atan(A/(B+So));
}
double p(int B2)   //计算曲率半径
{
	double dx,dy,dxx,dyy;
	double A,B,C,D,E;
	A=B2*PI/180;
	B=ds(B2);
	C=S(B2);
    D=dss(B2);
	dx=(So+C)*cos(A)+sin(A)*B-e*sin(A);
    dy=-sin(A)*(So+C)+B*cos(A)-e*cos(A);
	dxx=-(C+So)*sin(A)+cos(A)*B+D*sin(A)+(C-e)*cos(A);
	dyy=-cos(A)*(So+C)+D*cos(A)-sin(A)*B+(e-B)*sin(A);
	E=sqrt(pow(dx*dx+dy*dy,3))/sqrt(pow((dx*dyy-dxx*dy),2));
	return(E);
}
int main()
{   FILE *fp;

	int i=0;
	int k,h,l;
	double angle1max=0,angle2max=0,pmin=1000;

 if((fp=fopen("text.txt","w"))==NULL)
   {
		printf("Cann't open this file.\n");
		exit(0);
	}
   	fprintf(fp,"           理论坐标                       实际坐标     \n");
	fprintf(fp,"      x               y               x'               y' ");

for(;i!=360;)     //选择基圆半径
{
		rb=rb+b;
		So=sqrt(rb*rb-e*e);
		for(i=0;i<=ang1;i=i+5)
		{
			if(a(i)>A1||p(i)<P)
			break;
		}
		if(ang1+5-i)continue;
		for(i=ang1+5;i<=ang2;i=i+5)
		{
			if(p(i)<P)break;

		}
		if(ang2+5-i)continue;
		for(i=ang2+5;i<=ang3;i=i+5)
		{
			if(a(i)>A2||p(i)<P)
			break;
		}
		if(ang3+5-i)continue;
		for(i=ang3+5;i<360;i=i+5)
		{
			if(p(i)<P)
				break;
		}
}
	for(i=0;i<360;i=i+5)   //写入数组数据
	{
		xy(i);
	}
	for(i=0;i<=ang1;i=i+5)  //选择压力角及最小曲率半径
	{
	  if(angle1max<a(i))
	  {
		angle1max=a(i);
		k=i;
	  }
	 if(pmin>p(i))
	 {
		pmin=p(i);
		h=i;
	  }
	}
	for(i=ang2;i<=ang3;i=i+5)
	{
		if(angle2max<a(i))
		{
			angle2max=a(i);
			l=i;
		}
	    if(pmin>p(i))
		{
			pmin=p(i);
			h=i;
		}
	}
     for(i=0;i<72;i++)
   {
		fprintf(fp,"\n");

		{
fprintf(fp,"%12.3f\t%12.3f\t%12.3f\t%12.3f\t",theory[i][0],theory[i][1],fact[i][0],fact[i][1]);
		}
   }

	fclose(fp);

	printf("       理论坐标(x,y)                     ");
	printf("实际坐标(x,y)");printf("\n");
	for(i=0;i<72;i++)
	{
		printf("%f    ",theory[i][0]);
		printf("   ");
		printf("%f    ",theory[i][1]);
		printf("   ");
		printf("%f    ",fact[i][0]);
		printf("     ");
		printf("%f    ",fact[i][1]);
		printf("\n");
	}

	printf("基圆半径是:%f\n",rb);
	printf("推程最大压力角是:%f\n",angle1max*180/PI);
	printf("此时角度是是:%d\n",k);
	printf("回程最大压力角是:%f\n",angle2max*180/PI);
	printf("此时角度是是:%d\n",l);
	printf("最小曲率半径是:%f\n",pmin);
	printf("此时角度是:%d\n",h);
	return 0;
}

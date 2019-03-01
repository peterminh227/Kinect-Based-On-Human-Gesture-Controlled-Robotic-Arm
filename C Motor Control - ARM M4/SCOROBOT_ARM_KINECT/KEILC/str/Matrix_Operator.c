#include "stm32f4xx.h"
#include "stdio.h"
#include "stdint.h"
#include "Matrix_Operator.h"
double Temp_nul;
uint8_t i,j,k;
/*
*             Nhan 2 matran 
*             Matran x co m hang n cot
*             Matran y co n hang p cot
*             Ket qua matran z 
*      n							i=1,2...m
* Zij=Sum(xik*ykj),
*      k=1						j=1,2,...,p
*/

void Multiply_Matrix(double x[6][6],unsigned char m,unsigned char n,double y[6][6],unsigned char p,unsigned char q,double z[6][6])
{
for (i=0;i<m;i++)
	{
	for (j=0;j<q;j++)
		{
		for (k=0;k<p;k++)
			{
			Temp_nul= Temp_nul+x[i][k]*y[k][j];
			}
			z[i][j]=Temp_nul;
			Temp_nul=0;
		}
	}

}
/**
* Add 2 Matrix the same size mxn
* Xmxn=(xij) Ymxn=(yij)
* Xmxn+Ymxn=Zmxn
* with zij=xij+yij, i=1,2,...
*                   j=1,2,...,n
**/

void Add_Matrix(double x[6][6],double y[6][6],uint8_t m,uint8_t n, double z[6][6])
{
   for (i=0;i<m;i++)

			for (j=0;j<n;j++)

				z[i][j]=x[i][j]+y[i][j];
	

}
/**
* Sub 2 Matrix the same size mxn
* Xmxn=(xij) Ymxn=(yij)
* Xmxn-Ymxn=Zmxn
* with zij=xij-yij, i=1,2,...
*                   j=1,2,...,n
**/
void Sub_Matrix(double x[6][6], double y[6][6],unsigned char m, unsigned char n,  double z[6][6])
{
    for(i=0;i<m;i++)
       for(j=0;j<n;j++)
            z[i][j]=x[i][j]-y[i][j];
}
/*
*
  Nhan 1 matran voi 1 hang so
*/
void Multiply_Matrix_Vector(double x[6][6],unsigned char m, unsigned char n,double y, double z[6][6])
{
		   for(i=0;i<m;i++)
       for(j=0;j<n;j++)
            z[i][j]=x[i][j]*y;
}
/*
 chia mot ma tran mxn cho mot hang so
*/
void Divide_Matrix_Vector(double x[6][6],unsigned char m, unsigned char n,double y, double z[6][6])
{
	    for(i=0;i<m;i++)
       for(j=0;j<n;j++)
            z[i][j]=x[i][j]/y;
}

/**
  Tinh ma tran chuyen vi
**/

void Tranpose_Matrix(double x[6][6],unsigned char m, unsigned char n,  double y[6][6])
{
   for(i=0;i<n;i++)
      for(j=0;j<m;j++)
           y[i][j]=0;
  for(i=0;i<n;i++)
	{
      for(j=0;j<m;j++)
		{
           y[i][j]=x[j][i];
    }
  }
}
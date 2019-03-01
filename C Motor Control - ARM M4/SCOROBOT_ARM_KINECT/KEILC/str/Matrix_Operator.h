#ifndef _MATRIX_OPERATOR_H
#define _MATRIX_OPERATOR_H
#include "stm32f4xx.h"
#include "stdint.h"
extern double Temp_nul;
extern uint8_t i,j,k;
void Multiply_Matrix(double x[6][6],unsigned char m,unsigned char n,double y[6][6],unsigned char p,unsigned char q,double z[6][6]);

void Add_Matrix(double x[6][6],double y[6][6],uint8_t m,uint8_t n, double z[6][6]);
void Sub_Matrix(double x[6][6], double y[6][6],unsigned char m, unsigned char n,  double z[6][6]);
void Multiply_Matrix_Vector(double x[6][6],unsigned char m, unsigned char n,double y, double z[6][6]);
void Divide_Matrix_Vector(double x[6][6],unsigned char m, unsigned char n,double y, double z[6][6]);
void Tranpose_Matrix(double x[6][6],unsigned char m, unsigned char n,  double y[6][6]);
#endif
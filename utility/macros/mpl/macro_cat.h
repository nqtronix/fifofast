/*
 * macro_cat.h
 *
 * Created: 14.10.2018 11:09:54
 *  Author: Dennis
 *
 * Description:
 * concatenates up to 16 arguments without inhibition
 */


#ifndef MACRO_CAT_H_
#define MACRO_CAT_H_

#include "../mpl/macro_vfunc.h"

#define CAT(...)								VFUNC_0ARG(CAT, __VA_ARGS__)
#define CAT0()
#define CAT1(a)									a
#define CAT2(a,b)								a ## b
#define CAT3(a,b,c)								a ## b ## c
#define CAT4(a,b,c,d)							a ## b ## c ## d
#define CAT5(a,b,c,d,e)							a ## b ## c ## d ## e
#define CAT6(a,b,c,d,e,f)						a ## b ## c ## d ## e ## f
#define CAT7(a,b,c,d,e,f,g)						a ## b ## c ## d ## e ## f ## g
#define CAT8(a,b,c,d,e,f,g,h)					a ## b ## c ## d ## e ## f ## g ## h
#define CAT9(a,b,c,d,e,f,g,h,i)					a ## b ## c ## d ## e ## f ## g ## h ## i
#define CAT10(a,b,c,d,e,f,g,h,i,j)				a ## b ## c ## d ## e ## f ## g ## h ## i ## j
#define CAT11(a,b,c,d,e,f,g,h,i,j,k)			a ## b ## c ## d ## e ## f ## g ## h ## i ## j ## k
#define CAT12(a,b,c,d,e,f,g,h,i,j,k,l)			a ## b ## c ## d ## e ## f ## g ## h ## i ## j ## k ## l
#define CAT13(a,b,c,d,e,f,g,h,i,j,k,l,m)		a ## b ## c ## d ## e ## f ## g ## h ## i ## j ## k ## l ## m
#define CAT14(a,b,c,d,e,f,g,h,i,j,k,l,m,n)		a ## b ## c ## d ## e ## f ## g ## h ## i ## j ## k ## l ## m ## n
#define CAT15(a,b,c,d,e,f,g,h,i,j,k,l,m,n,o)	a ## b ## c ## d ## e ## f ## g ## h ## i ## j ## k ## l ## m ## n ## o
#define CAT16(a,b,c,d,e,f,g,h,i,j,k,l,m,n,o,p)	a ## b ## c ## d ## e ## f ## g ## h ## i ## j ## k ## l ## m ## n ## o ## p




#endif /* MACRO_CAT_H_ */
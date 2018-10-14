/*
 * macro_vfunc.h
 *
 * Created: 14.10.2018 11:05:26
 *  Author: Dennis
 *
 * Description:
 * vfunc expands to a different macro depending on the argument count. See usage example below.
 */


#ifndef MACRO_VFUNC_H_
#define MACRO_VFUNC_H_

#include "macro_narg.h"

// general definition for any function name
// VFUNC_NARG passes N arguments to each function in addition to the variable arguments
#define _VFUNC_(name,n)							name##n
#define _VFUNC(name,n)							_VFUNC_(name,n)
#define VFUNC_0ARG(func,...)					_VFUNC(func,__NARG__(__VA_ARGS__)) (__VA_ARGS__)

#define _VFUNC1(name,arg1,n)					_VFUNC_(name,n)
#define VFUNC_1ARG(func,arg1,...)				_VFUNC1(func,arg1,__NARG__(__VA_ARGS__)) (arg1,__VA_ARGS__)

#define _VFUNC2(name,arg1,arg2,n)				_VFUNC_(name,n)
#define VFUNC_2ARG(func,arg1,arg2,...)			_VFUNC2(func,arg1,arg2,__NARG__(__VA_ARGS__)) (arg1,arg2,__VA_ARGS__)

#define _VFUNC3(name,arg1,arg2,arg3,n)			_VFUNC_(name,n)
#define VFUNC_3ARG(func,arg1,arg2,arg3,...)		_VFUNC3(func,arg1,arg2,arg3,__NARG__(__VA_ARGS__)) (arg1,arg2,arg3,__VA_ARGS__)

// USAGE:
// EXAMPLE 1:
// Change all 5 'SUM' in the example below to your desired macro name
//
// #define SUM(...)			VFUNC_0ARG(SUM, __VA_ARGS__)
// #define SUM1(x)			(x)
// #define SUM2(x, y)		((x) + (y))
// #define SUM3(x, y, z)	((x) + (y) + (z))
//
// EXAMPLE 2:
// Use it for default arguments:
// 
// #define func(...)		VFUNC_0ARG(func, __VA_ARGS__)
// #define func2(a, b)		func4(a, b, NULL, NULL)
// #define func3(a, b, c)	func4(a, b, c, NULL)
// 
// // real function:
// int func4(int a, int b, void* c, void* d) { /* ... */ }



#endif /* MACRO_VFUNC_H_ */
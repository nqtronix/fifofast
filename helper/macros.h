/*
 * macros.h
 *
 * Created: 17.04.2016 10:49:34
 *  Author: Dennis aka nqtronix (github.com/nqtronix)
 *
 * Description:
 * This file contains a subset of my macro collection which is required for fifiofast.c/.h to function.
 */


#ifndef MACROS_H_
#define MACROS_H_

//////////////////////////////////////////////////////////////////////////
// Helper Macros
//////////////////////////////////////////////////////////////////////////

// SPLIT __VA_ARGS__ into separate macros based on argument count
// get number of arguments with __NARG__
#define __NARG__(...)  __NARG_I_("ignored", ##__VA_ARGS__, __RSEQ_N())
#define __NARG_I_(...) __ARG_N(__VA_ARGS__)
#define __ARG_N( \
      _1, _2, _3, _4, _5, _6, _7, _8, _9,_10, \
     _11,_12,_13,_14,_15,_16,_17,_18,_19,_20, \
     _21,_22,_23,_24,_25,_26,_27,_28,_29,_30, \
     _31,_32,_33,_34,_35,_36,_37,_38,_39,_40, \
     _41,_42,_43,_44,_45,_46,_47,_48,_49,_50, \
     _51,_52,_53,_54,_55,_56,_57,_58,_59,_60, \
     _61,_62,_63,_64,N,...) N
#define __RSEQ_N() \
     63,62,61,60,                   \
     59,58,57,56,55,54,53,52,51,50, \
     49,48,47,46,45,44,43,42,41,40, \
     39,38,37,36,35,34,33,32,31,30, \
     29,28,27,26,25,24,23,22,21,20, \
     19,18,17,16,15,14,13,12,11,10, \
     9,8,7,6,5,4,3,2,1,0

// general definition for any function name
// VFUNC_NARG passes N arguments to each function in addition to the variable arguments
// These macros can't be inserted into one with the same name, so as a workaround alternative names
 // "B" are provided
#define _VFUNC_(name,n)					name##n
#define _VFUNC(name,n)					_VFUNC_(name,n)
#define VFUNC_0ARG(func,...)			_VFUNC(func,__NARG__(__VA_ARGS__)) (__VA_ARGS__)
#define VFUNC_0ARGB(func,...)			_VFUNC(func,__NARG__(__VA_ARGS__)) (__VA_ARGS__)


//////////////////////////////////////////////////////////////////////////
// User Macros	
//////////////////////////////////////////////////////////////////////////

// concatenates up to 8 arguments without inhibition
#define CAT(...)					VFUNC_0ARG(CAT, __VA_ARGS__)
#define CAT0()
#define CAT1(a)									a
#define CAT2(a,b)								a ## b
#define CAT3(a,b,c)								a ## b ## c
#define CAT4(a,b,c,d)							a ## b ## c ## d
#define CAT5(a,b,c,d,e)							a ## b ## c ## d ## e
#define CAT6(a,b,c,d,e,f)						a ## b ## c ## d ## e ## f
#define CAT7(a,b,c,d,e,f,g)						a ## b ## c ## d ## e ## f ## g
#define CAT8(a,b,c,d,e,f,g,h)					a ## b ## c ## d ## e ## f ## g ## h


// STR(...) wraps #defined names and turns them into a string. Unlike the typical variant, this
// implementations PRESERVES commas, making it possible to "stringify" #defined C code snippets.
// note: technically the preprocessor will return 1, 3, 5, etc. individual strings. However the
//       C compiler and the #pragma message command will concat them into one string before use.
#define STR(...)					VFUNC_0ARG(STR, __VA_ARGS__)
#define STR0()
#define STR1(a)									#a
#define STR2(a,b)								#a ", " #b
#define STR3(a,b,c)								#a ", " #b ", " #c
#define STR4(a,b,c,d)							#a ", " #b ", " #c ", " #d
#define STR5(a,b,c,d,e)							#a ", " #b ", " #c ", " #d ", " #e
#define STR6(a,b,c,d,e,f)						#a ", " #b ", " #c ", " #d ", " #e ", " #f
#define STR7(a,b,c,d,e,f,g)						#a ", " #b ", " #c ", " #d ", " #e ", " #f ", " #g
// following code can be used to print messages containing #define values at compile time
// #pragma message "some description here" STR(VALUE_TO_DIPLAY)


#endif /* MACROS_H_ */
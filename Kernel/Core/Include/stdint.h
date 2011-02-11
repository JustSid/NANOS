//
//  stdint.h
//  NANOS
//
//  Created by Sidney Just
//  Copyright (c) 2010 by Sidney Just
//  Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated 
//  documentation files (the "Software"), to deal in the Software without restriction, including without limitation 
//  the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, 
//  and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
//  The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
//  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, 
//  INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR 
//  PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE 
//  FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, 
//  ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
//

#ifndef _STDINT_H_
#define _STDINT_H_

#define NULL ((void*) 0)

typedef unsigned long long int	uint64_t;
typedef unsigned long int		uint32_t;
typedef unsigned short int		uint16_t;
typedef unsigned char			uint8_t;

typedef signed long long int	int64_t;
typedef signed long int			int32_t;
typedef signed short int		int16_t;
typedef signed char				int8_t;

typedef uint32_t				uintptr_t;
typedef uint32_t				size_t;

#define __INT64_C(c)  c ## L
#define __UINT64_C(c) c ## UL

#define INT8_MIN		(-128)
#define INT16_MIN		(-32767-1)
#define INT32_MIN		(-2147483647-1)
#define INT64_MIN		(-__INT64_C(9223372036854775807)-1)

#define INT8_MAX		(127)
#define INT16_MAX		(32767)
#define INT32_MAX		(2147483647)
#define INT64_MAX		(__INT64_C(9223372036854775807))

#define UINT8_MAX		(255)
#define UINT16_MAX		(65535)
#define UINT32_MAX		(4294967295U)
#define UINT64_MAX		(__UINT64_C(18446744073709551615))


typedef __builtin_va_list	va_list;

#define va_start(v, l)		__builtin_va_start(v,l)
#define va_end(v)			__builtin_va_end(v)
#define va_arg(v, l)		__builtin_va_arg(v,l)
#define va_copy(d, s)		__builtin_va_copy(d,s)

#endif
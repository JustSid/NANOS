/*
 *  assert.h
 *  Kernel
 *
 *  Created by Sidney Just on 22.03.11.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef _ASSERT_H_
#define _ASSERT_H_

#include "console.h"
#include "panic.h"
#include "debug.h"

#define __assert(file, line, function, expression) panic("%s:%i: %s: Assertion '%s' failed\n", db_lastPathComponent(file), line, function, expression)

#ifndef NDEBUG
#	define assert(condition) ((condition) ? ((void) 0) : __assert(__FILE__, __LINE__, __func__, #condition))
#else
#	define assert(ignored) ((void) 0)
#endif

#endif

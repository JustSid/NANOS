//
//  kernelTask.c
//  NANOS
//
//  Created by Sidney Just
//  Copyright (c) 2011 by Sidney Just
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

#include <system/assert.h>
#include <system/debug.h>
#include <system/console.h>

#include <memory/memory.h>
#include <scheduler/scheduler.h>


extern void shell_main();



/*void kt_testMemoryAddress()
{
	// This test checks if the allocater returns the very same address for the same allocation
	cn_printf("   Running memory address test...");
	
	void *areaA, *areaB;
	
	areaA = mm_alloc(10);
	mm_free(areaA);
	
	areaB = mm_alloc(10);
	mm_free(areaB);
	
	
	assert(areaA == areaB);
	cn_printf("passed.\n");
}

void kt_testMemoryMerging()
{
	// This test checks if the memory merging works as intended
	cn_printf("   Running memory merging test...");
	
	#define kt_testMemoryMergingRounds 10
	
	void *areaEven[kt_testMemoryMergingRounds];
	void *areaOdd[kt_testMemoryMergingRounds];
	
	int i;
	for(i=0; i<kt_testMemoryMergingRounds; i++)
	{
		areaEven[i] = mm_alloc(10);
		areaOdd[i]  = mm_alloc(10);
	}
	
	// Free the even areas
	for(i=0; i<kt_testMemoryMergingRounds; i++)
		mm_free(areaEven[i]);
	
	// Free the odd areas
	for(i=0; i<kt_testMemoryMergingRounds; i++)
		mm_free(areaOdd[i]);
	
	mm_defrag();
	
	void *areaTest = mm_alloc((10 * kt_testMemoryMergingRounds) * 2);
	
	assert(areaTest == areaEven[0]);
	cn_printf("passed.\n");
}*/



void kernelTask()
{
	//sd_setName("Idle");
	//sd_setIdentifier("krn.nanos.kernel_task");
	
	/*if(0)
	{
		cn_printf("Running memory manager tests...\n");
		
		kt_testMemoryAddress();
		kt_testMemoryMerging();
		
		cn_printf("All tests passed\n");		
	}*/
	
	
	//sd_spawnProcess(shell_main); // Kick off the shell
	while(1) {__asm__ volatile ("hlt");}
}

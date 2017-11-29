


As Mike Abrash points out, when timing various parts of you code, it is
essential to understand how long the actual time fetch call itself
takes.  You'll have no hope of timing something that takes one millionth
of a second if your timer call takes one ten-thousandth of a second
to return the time.

The time required to return from the various timeGetTime(),
GetTickCount(), QuerryPerformanceCounter(), and special assembly
_emit 0x0F timing methods can vary by a factor of 150.  This difference
can obscure your results and reduce the accuracy of your measurements.

It often essential to use the fastest timer available, especialy when
monitoring for communication time-outs.  In one instance, replacing
GetTickCount() with a faster timer resulted in transactions being 
sped up by a factor of more than 100.

This simple test application runs a variety of timing functions and 
reports how quickly the timing functions are executed.  It generates
a "times.txt" report in the directory where the application is run.
It is also easy to modify the application code and add your own timing
functions for comparison.

To generate the report, just double-click the TimerTiming.exe and hit
retun when the DOS window prompts you to.

***********************************************************************
Note:  This code does not strictly follow Intel's guidelines for
CPU Identification and the RDTSC (Read Time-Stamp Counter) instruction.
For more information, see Developer.Intel.com

AP-485  Intel Processor Identification and the CPUID Instruction
Model Specific Registers and Functions - Performance Monitoring
***********************************************************************

-NVIDIA Developer Relations

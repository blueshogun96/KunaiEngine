/*********************************************************************NVMH2****
Path:  C:\Dev\devrel\Nv_sdk_4\CommonSrc\TimerTiming
File:  TimerTiming.cpp

Copyright (C) 1999, 2000 NVIDIA Corporation
This file is provided without support, instruction, or implied warranty of any
kind.  NVIDIA makes no guarantee of its fitness for a particular purpose and is
not liable under any circumstances for any damages or loss whatsoever arising
from the use or inability to use this file or items derived from it.

Comments:


******************************************************************************/



#include <windows.h>
#include <stdio.h>
#include <time.h>

#include <stdlib.h>
#include <mmsystem.h>					// for timeGetTime() 
#include <assert.h>

#pragma comment( lib, "winmm.lib" )		// for timeGetTime()


#define TRACE		NullFunc
#define ASSERT		assert

void DoTimingRun();
void NullFunc( char * szFormat, ... ) {};


/////////////////////////////////////////////////////////////////

int main(int argc, char* argv[])
{
	char c;

	printf("\n\n");
	printf("This application times a number of system time functions.\n");
	printf("It will create a \"times.txt\" report file in the directory\n");
	printf("   from which it is run.\n");
	printf("\n");
	printf("Various timing functions include the Pentium _emit 0x0F\n");
	printf("  however there is no Athlon specific timing code.\n");
	printf("\n\n");
	printf("Hit RETURN to run the timing session\n");

	scanf("%hc", & c );


	DoTimingRun();


	return 0;
}


//////////




struct TimingInfo
{	
	LARGE_INTEGER	m_start_clk;
	LARGE_INTEGER	m_end_clk;

	int				m_nSamples;
	LARGE_INTEGER	m_interval_sum;		// sum of all end-start, nSamples number added in

	char 			m_csName[2048];		// text desc of what timed
};


#pragma warning (push)
#pragma warning (disable : 4035)	// disable no return value warning

__forceinline  DWORD GetPentiumCounter()
{
	__asm
	{
		xor   eax,eax	          // VC won't realize that eax is modified w/out this
								  //   instruction to modify the val.
								  //   Problem shows up in release mode builds
		_emit 0x0F		          // Pentium high-freq counter to edx;eax
		_emit 0x31		          // only care about low 32 bits in eax
								  //   A 500 Mhz machine will rollover every 2 sec.
		xor   edx,edx	          // so VC gets that edx is modified
	}
}

#pragma warning (pop)



void DoTimingRun()
{
	LARGE_INTEGER	freq;
	DWORD			tmp_tick;
	LARGE_INTEGER   tmp_clk;
	
	int i,n;
	int meth;

	#define NMETHOD	4
	#define NPASSES	4

	int passes[NPASSES];
	passes[0] = 100;
	passes[1] = 500;
	passes[2] = 1000;
	passes[3] = 10000;
	
	TimingInfo	times[NMETHOD][NPASSES];

	DWORD * pdw = (DWORD*) calloc( passes[3], sizeof( DWORD ) );

	if( pdw == NULL )
	{
		// can't allocate
		MessageBeep( MB_ICONEXCLAMATION );
		Sleep( 100 );
		MessageBeep( MB_ICONEXCLAMATION );
		Sleep( 100 );
		MessageBeep( MB_ICONASTERISK );
		assert( false );
		return;
	}

	FILE * pf = fopen( "times.txt", "wt" );

	if( pf == NULL )
	{
		TRACE("can't open timing data file!\n");
		ASSERT(false);
		MessageBeep(MB_ICONEXCLAMATION);
	}

	fprintf( pf,"\n\n");
	TRACE("\n\n");


	for( i=0; i < NPASSES; i++ )
	{
		//fprintf( pf,"i: %d   passes[i]: %d\n", i, passes[i] );
		//TRACE("i: %d   passes[i]: %d\n", i, passes[i] );

		////////////////////////////////////
		// 1st method

		meth = 0;

		sprintf( &(times[meth][i].m_csName[0]),  "QueryPerfCntr..()  %d times\n", passes[i] );
		TRACE( "%s", times[meth][i].m_csName );

		QueryPerformanceCounter( &( times[meth][i].m_start_clk ) );

		for( n=0; n < passes[i]; n++ )
		{
			QueryPerformanceCounter( &tmp_clk );
		}

		QueryPerformanceCounter( &( times[meth][i].m_end_clk ) );

		times[meth][i].m_nSamples = passes[i];
		times[meth][i].m_interval_sum.HighPart = times[meth][i].m_end_clk.HighPart - times[meth][i].m_start_clk.HighPart;
		times[meth][i].m_interval_sum.LowPart  = times[meth][i].m_end_clk.LowPart  - times[meth][i].m_start_clk.LowPart;


		////////////////////////////
		//  2nd method

		meth = 1;

		sprintf( &(times[meth][i].m_csName[0] ),"GetTickCount()  %d times\n", passes[i] );
		TRACE("%s", times[meth][i].m_csName );

		QueryPerformanceCounter( &( times[meth][i].m_start_clk ) );

		for( n=0; n < passes[i]; n++ )
		{
			tmp_tick = GetTickCount();
		}

		QueryPerformanceCounter( &( times[meth][i].m_end_clk ) );

		times[meth][i].m_nSamples = passes[i];
		times[meth][i].m_interval_sum.HighPart = times[meth][i].m_end_clk.HighPart - times[meth][i].m_start_clk.HighPart;
		times[meth][i].m_interval_sum.LowPart  = times[meth][i].m_end_clk.LowPart  - times[meth][i].m_start_clk.LowPart;

		////////////////////////////
		// 3rd method - TimeGetTime()

		meth = 2;

		sprintf( &(times[meth][i].m_csName[0] ),"TimeGetTime()  %d times\n", passes[i] );
		TRACE("%s", times[meth][i].m_csName );

		QueryPerformanceCounter( &( times[meth][i].m_start_clk ) );

		for( n=0; n < passes[i]; n++ )
		{
			tmp_tick = timeGetTime();
		}

		QueryPerformanceCounter( &( times[meth][i].m_end_clk ) );

		times[meth][i].m_nSamples = passes[i];
		times[meth][i].m_interval_sum.HighPart = times[meth][i].m_end_clk.HighPart - times[meth][i].m_start_clk.HighPart;
		times[meth][i].m_interval_sum.LowPart  = times[meth][i].m_end_clk.LowPart  - times[meth][i].m_start_clk.LowPart;

		////////////////////////////
		// 4th method - assembly timer dump

		meth = 3;

		try
		{
			sprintf( &(times[meth][i].m_csName[0] ), "Pentium internal high-freq cntr()  %d times\n", passes[i] );
			TRACE("%s", times[meth][i].m_csName );

			QueryPerformanceCounter( &( times[meth][i].m_start_clk ) );

			for( n=0; n < passes[i]; n++ )
			{
				pdw[n] = GetPentiumCounter();

			}

			QueryPerformanceCounter( &( times[meth][i].m_end_clk ) );


			times[meth][i].m_nSamples = passes[i];
			times[meth][i].m_interval_sum.HighPart = times[meth][i].m_end_clk.HighPart - times[meth][i].m_start_clk.HighPart;
			times[meth][i].m_interval_sum.LowPart  = times[meth][i].m_end_clk.LowPart  - times[meth][i].m_start_clk.LowPart;
		}
		catch( char *str )
		{ 
			sprintf( &(times[meth][i].m_csName[0] ), "Pentium internal counter not available!\n");
			times[meth][i].m_nSamples = -1;
			times[meth][i].m_interval_sum.HighPart = -1;
			times[meth][i].m_interval_sum.LowPart  = 0;
		}

	}

	free( pdw );



	// write the report:

	QueryPerformanceFrequency( & freq );

	fprintf( pf, "Report file for timing the various timers.\n\n");
	fprintf( pf, "*** Key number is the avg time.\n");
	fprintf( pf, "    The smaller this number, the faster the timer.\n\n");

	fprintf( pf, "\nQueryPerformanceFrequency() freq  = %d  %u \n\n\n",  freq.HighPart, freq.LowPart );
	TRACE( "\nQueryPerformanceFrequency() freq = %d  %u \n", freq.HighPart, freq.LowPart );

	double dur;


	for(n=0; n < NMETHOD; n++ )
	{
		for(i=0; i < NPASSES; i++ )
		{
			fprintf( pf, "method %d:\n", n );
			fprintf( pf, "  %s", times[n][i].m_csName );
			fprintf( pf, "  tot:   %u %u\n", times[n][i].m_interval_sum.HighPart, times[n][i].m_interval_sum.LowPart ); 

			TRACE( "method %d:\n", n );
			TRACE( "  %s", times[n][i].m_csName );
			TRACE( "  tot:   %u %u\n", times[n][i].m_interval_sum.HighPart, times[n][i].m_interval_sum.LowPart ); 

			dur = ((double)times[n][i].m_interval_sum.LowPart) / ((double)times[n][i].m_nSamples * (double)freq.LowPart );
			
			fprintf( pf, "  avg:   %f\n", times[n][i].m_interval_sum.LowPart / (float)times[n][i].m_nSamples );
			fprintf( pf, "  avg time:\t\t\t%g sec\n", dur );

			TRACE( "  avg:   %f\n", times[n][i].m_interval_sum.LowPart / (float)times[n][i].m_nSamples );
			TRACE( "  avg time:  %g sec\n", dur );
		}

		fprintf( pf, "\n\n\n");
		TRACE("\n\n\n");
	}


	fclose(pf);
}



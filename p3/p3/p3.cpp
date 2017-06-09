// p3.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <stdlib.h>
#include <omp.h>
#include <stdio.h>
#include <math.h>
#include <time.h>

#define NUMPAD 1
#define NUMT 1
#define NUMTRIES 10






int main()
{
	struct s
	{
		float value;
		int pad[NUMPAD];
} Array[NUMT];
#ifndef _OPENMP
	fprintf(stderr, "OpenMP is not supported here -- sorry.\n");
	return 1;
#endif
	
	omp_set_num_threads(NUMT);
	fprintf(stderr, "Using %d threads\n", NUMT);
	srand(time(NULL));

	const int SomeBigNumber = 100000000;	// keep < 2B
	double maxMegaMults = 0.;
	double sumMegaMults = 0.;
	for (int t = 0; t < NUMTRIES; t++) {
		double time0 = omp_get_wtime();
#pragma omp parallel for
		for (int i = 0; i < NUMT; i++)
		{
			unsigned int seed = 0;
			for (unsigned int j = 0; j < SomeBigNumber; j++)
			{
				Array[i].value = Array[i].value + (float)rand();
			}
		}
		double time1 = omp_get_wtime();
		double megaMults = (time1 - time0);
		sumMegaMults += megaMults;
		if (megaMults > maxMegaMults)
			maxMegaMults = megaMults;
	}

	double avgMegaMults = sumMegaMults / (double)NUMTRIES;
	printf("   Peak Performance = %8.2lf MegaMults/Sec\n", maxMegaMults);
	printf("Average Performance = %8.2lf MegaMults/Sec\n", avgMegaMults);
	
	

	
	//printf("Performance = %8.2lf MegaMults/Sec\n", megaMults);
	
    return 0;
}


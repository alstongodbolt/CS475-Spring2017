#include <omp.h>
#include <stdio.h>
#include <math.h>
#include <cstdlib>

#define NUMT	        16
#define ARRAYSIZE       32768	// you decide
//#define NUMTRIES        4	// you decide
#define CHUNKSIZE		4096

float A[ARRAYSIZE];
float B[ARRAYSIZE];
float C[ARRAYSIZE];

float Ranf(float low, float high)
{
	float r = (float)rand();		// 0 - RAND_MAX

	return(low + r * (high - low) / (float)RAND_MAX);
}

int main()
{
#ifndef _OPENMP
	fprintf(stderr, "OpenMP is not supported here -- sorry.\n");
	return 1;
#endif
	float array[ARRAYSIZE];
	float product;

	omp_set_num_threads(NUMT);
	fprintf(stderr, "Using %d threads\n", NUMT);
	double maxMegaMults = 0.;
	double sumMegaMults = 0.;

	for (int i = 0; i < ARRAYSIZE; i++) {
		array[i] = Ranf(-1.f, 1.f);
	}

	double time0 = omp_get_wtime();
#pragma omp parallel for private(product), schedule(dynamic,CHUNKSIZE)
	for (int i = 0; i < ARRAYSIZE - 1; i++)
	{
		product = 1.;
		for (int j = 0; j < i; j++)
		{
			product *= array[i];
		}
	}
	double time1 = omp_get_wtime();

	long int numMuled = (long int)ARRAYSIZE * (long int)(ARRAYSIZE + 1) / 2;  // count of how many multiplications were done:
	fprintf(stderr, "Threads = %2d; ChunkSize = %5d; Scheduling=static ; MegaMults/sec = %10.2lf\n", NUMT, CHUNKSIZE, (double)numMuled / (time1 - time0) / 1000000.);

	



	return 0;
}


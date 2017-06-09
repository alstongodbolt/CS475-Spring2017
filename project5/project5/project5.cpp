// project5.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <omp.h>
#include <stdio.h>
#include <math.h>
//#include "simd.p5.h"

#define ARRAYSIZE 32000000
#define len 32000000
#define NUMTRIES 10

float a[ARRAYSIZE];
float b[ARRAYSIZE];
float c[ARRAYSIZE];

void nonSIMDMult(float *a, float *b, float *c) {
	for (int i = 0; i < ARRAYSIZE; i++) {
		c[i] = a[i] * b[i];
	}
}

float nonSIMDReduction(float *a, float *b) {
	//float sum[4] = { 0., 0., 0., 0. };
	float sum1 = 0.;
	for (int i = 0; i < len; i++) {
		sum1 += a[i] * b[i];
	}
	//return sum[0] + sum[1] + sum[2] + sum[3];
	return sum1;
}


int main()
{
	//double maxMegaMults = 0.;
	//double sumMegaMults = 0.;

	/*for (int t = 0; t < NUMTRIES; t++) 
	{
		double time0 = omp_get_wtime();
		
		nonSIMDMult(a, b, c);

		double time1 = omp_get_wtime();
		double megaMults = (double)ARRAYSIZE / (time1 - time0) / 1000000.;
		sumMegaMults += megaMults;
		if (megaMults > maxMegaMults) {
			maxMegaMults = megaMults;
		}
	}
	double avgMegaMults = sumMegaMults / (double)NUMTRIES;
	printf("Peak Performance = %8.2lf MegaMults/Sec\n", maxMegaMults);
	printf("Avg Performance = %8.2lf MegaMults/Sec\n", avgMegaMults);*/

	/*double maxMegaMults = 0.;
	double sumMegaMults = 0.;

	for (int t = 0; t < NUMTRIES; t++)
	{
		double time0 = omp_get_wtime();

		SimdMul(a, b, c, len);

		double time1 = omp_get_wtime();
		double megaMults = (double)ARRAYSIZE / (time1 - time0) / 1000000.;
		sumMegaMults += megaMults;
		if (megaMults > maxMegaMults) {
			maxMegaMults = megaMults;
		}
	}
	double avgMegaMults = sumMegaMults / (double)NUMTRIES;
	printf("Peak Performance = %8.2lf MegaMults/Sec\n", maxMegaMults);
	printf("Avg Performance = %8.2lf MegaMults/Sec\n", avgMegaMults);*/

	double maxMegaMults = 0.;
	double sumMegaMults = 0.;

	for (int t = 0; t < NUMTRIES; t++)
	{
		double time0 = omp_get_wtime();

		nonSIMDReduction(a, b);

		double time1 = omp_get_wtime();
		double megaMults = (double)ARRAYSIZE / (time1 - time0) / 1000000.;
		sumMegaMults += megaMults;
		if (megaMults > maxMegaMults) {
			maxMegaMults = megaMults;
		}
	}
	double avgMegaMults = sumMegaMults / (double)NUMTRIES;
	printf("Peak Performance = %8.2lf MegaMults/Sec\n", maxMegaMults);
	printf("Avg Performance = %8.2lf MegaMults/Sec\n", avgMegaMults);

    return 0;
}


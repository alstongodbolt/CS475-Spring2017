// project7.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <omp.h>
#include <stdio.h>
#include <fstream>




#define NUMT 8
#define NUMTRIES 10

int main()
{
	FILE *fp = fopen("signal.txt", "r");
	if (fp == NULL)
	{
		fprintf(stderr, "Cannot open file 'signal.txt'\n");
		exit(1);
	}
	int Size;
	fscanf(fp, "%d", &Size);
	float *Array = new float[2 * Size];
	float *Sums = new float[1 * Size];
	for (int i = 0; i < Size; i++)
	{
		fscanf(fp, "%f", &Array[i]);
		Array[i + Size] = Array[i];		// duplicate the array
	}
	fclose(fp);

#ifndef _OPENMP
	fprintf(stderr, "OpenMP is not supported here -- sorry.\n");
	return 1;
#endif
	FILE *op = fopen("file.txt", "w");
	if (op == NULL)
	{
		printf("Error opening file\n");
		exit(1);
	}
	omp_set_num_threads(NUMT);
	fprintf(stderr, "Using %d threads\n", NUMT);

	double maxMegaMults = 0.;
	double sumMegaMults = 0.;
	
	for (int t = 0; t < NUMTRIES; t++)
	{
		double time0 = omp_get_wtime();

#pragma omp parallel for default(none)
		for (int shift = 0; shift < Size; shift++)
		{
			float sum = 0.;
			for (int i = 0; i < Size; i++)
			{
				sum += Array[i] * Array[i + shift];
				//sum += Array[i];
			}
			Sums[shift] = sum;	// note the "fix #2" from false sharing if you are using OpenMP
			
			//printf("%8.10f\n", Sums[shift]);
			fprintf(op, "%8.10f\n", Sums[shift]);
		}

		double time1 = omp_get_wtime();
		double megaMults = ((double)Size * Size) / (time1 - time0) / 1000000.;
		sumMegaMults += megaMults;
		if (megaMults > maxMegaMults)
			maxMegaMults = megaMults;
	}
	fclose(op);
	double avgMegaMults = sumMegaMults / (double)NUMTRIES;
	printf("   Peak Performance = %8.2lf MegaMults/Sec\n", maxMegaMults);
	printf("Average Performance = %8.2lf MegaMults/Sec\n", avgMegaMults);
		
	

    return 0;
}


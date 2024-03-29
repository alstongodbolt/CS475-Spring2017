#include <omp.h>
#include <stdio.h>
#define _USE_MATH_DEFINES
#include <math.h>


#define NUMT	      1

#define NUMNODES      1000	// you decide
#define NUMTRIES        100	// you decide

#define XMIN	 0.
#define XMAX	 3.
#define YMIN	 0.
#define YMAX	 3.

#define TOPZ00  0.
#define TOPZ10  1.
#define TOPZ20  0.
#define TOPZ30  0.

#define TOPZ01  1.
#define TOPZ11  6.
#define TOPZ21  1.
#define TOPZ31  0.

#define TOPZ02  0.
#define TOPZ12  1.
#define TOPZ22  0.
#define TOPZ32  4.

#define TOPZ03  3.
#define TOPZ13  2.
#define TOPZ23  3.
#define TOPZ33  3.

#define BOTZ00  0.
#define BOTZ10  -3.
#define BOTZ20  0.
#define BOTZ30  0.

#define BOTZ01  -2.
#define BOTZ11  10.
#define BOTZ21  -2.
#define BOTZ31  0.

#define BOTZ02  0.
#define BOTZ12  -5.
#define BOTZ22  0.
#define BOTZ32  -6.

#define BOTZ03  -3.
#define BOTZ13   2.
#define BOTZ23  -8.
#define BOTZ33  -3.



double Height(int iu, int iv)	// iu,iv = 0 .. NUMNODES-1
{
	double u = (double)iu / (double)(NUMNODES - 1);
	double v = (double)iv / (double)(NUMNODES - 1);

	// the basis functions:

	double bu0 = (1. - u) * (1. - u) * (1. - u);
	double bu1 = 3. * u * (1. - u) * (1. - u);
	double bu2 = 3. * u * u * (1. - u);
	double bu3 = u * u * u;

	double bv0 = (1. - v) * (1. - v) * (1. - v);
	double bv1 = 3. * v * (1. - v) * (1. - v);
	double bv2 = 3. * v * v * (1. - v);
	double bv3 = v * v * v;

	// finally, we get to compute something:


	double top = bu0 * (bv0*TOPZ00 + bv1*TOPZ01 + bv2*TOPZ02 + bv3*TOPZ03)
		+ bu1 * (bv0*TOPZ10 + bv1*TOPZ11 + bv2*TOPZ12 + bv3*TOPZ13)
		+ bu2 * (bv0*TOPZ20 + bv1*TOPZ21 + bv2*TOPZ22 + bv3*TOPZ23)
		+ bu3 * (bv0*TOPZ30 + bv1*TOPZ31 + bv2*TOPZ32 + bv3*TOPZ33);

	double bot = bu0 * (bv0*BOTZ00 + bv1*BOTZ01 + bv2*BOTZ02 + bv3*BOTZ03)
		+ bu1 * (bv0*BOTZ10 + bv1*BOTZ11 + bv2*BOTZ12 + bv3*BOTZ13)
		+ bu2 * (bv0*BOTZ20 + bv1*BOTZ21 + bv2*BOTZ22 + bv3*BOTZ23)
		+ bu3 * (bv0*BOTZ30 + bv1*BOTZ31 + bv2*BOTZ32 + bv3*BOTZ33);

	return top - bot;	// if the bottom surface sticks out above the top surface
						// then that contribution to the overall volume is negative
}

double Height(int, int);
int main()
{
#ifndef _OPENMP
	fprintf(stderr, "OpenMP is not supported here -- sorry.\n");
	return 1;
#endif
	
	const double A = 0;
	const double B = M_PI;
	double dx = (B - A) / double(NUMNODES - 1);
	double volume = 0;
	double fullTileArea = (((XMAX - XMIN) / (double)(NUMNODES - 1))  *  ((YMAX - YMIN) / (double)(NUMNODES - 1)));
	omp_set_num_threads(NUMT);
	fprintf(stderr, "Using %d threads\n", NUMT);
	double time0 = omp_get_wtime();
#pragma omp parallel for default(none),shared(fullTileArea),reduction(+:volume)
	for (int i = 0; i < NUMNODES*NUMNODES; i++)
	{
		 
		int iu = i % NUMNODES;
		int iv = i / NUMNODES;

				// the area of a single full-sized tile:
		double x = A + fullTileArea * (double)i;
		double f = Height(iu, iv);
		volume += f;
		// sum up the weighted heights into the variable "volume"
		// using an OpenMP for loop and a reduction:
		
		//printf("weighted %f\n", fullTileArea);
	}
	double time1 = omp_get_wtime();
	double timed = time1 - time0;
	printf("time %f\n", timed);
	volume *= fullTileArea;
	printf("volume %f\n", volume);
	double output = volume*.6;
	//double output = volume *60 * NUMT;
	printf("output %f\n", output);
	
	return 0;
}





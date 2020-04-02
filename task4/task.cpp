#include <cilk/cilk.h>
#include <stdlib.h>
#include <iostream>
#include <math.h>
#include <chrono>
#include "tbb/task_scheduler_init.h"
#include "tbb/parallel_for.h"
#include "tbb/blocked_range.h"
#include "tbb/scalable_allocator.h"
#include "tbb/parallel_reduce.h"
using namespace tbb;
using namespace std;
using namespace std::chrono;

# define M_PI           3.14159265358979323846  /* pi, #include <math.h> */
double a = 0.0;
double b = 1.0;
double refer = 2.094395;

double func(double x)
{
	return 4 / (sqrt(4 - x * x));
}

int main()
{
	int q[5] = { 100, 1000, 10000, 100000, 1000000 };
	double h[5];
	for (int i = 0; i < 5; i++)
		h[i] = (double)(b - a) / (double)q[i];

	cout << " Value " << "|" << "  Error  " << "|" << "Time" << endl;

	for (int j = 0; j < 5; j++)
	{
		high_resolution_clock::time_point t1 = high_resolution_clock::now();
		double sum;
		sum = parallel_reduce
		(
			blocked_range<int>(0, q[j]),
			0.f,
			[&](blocked_range<int> range, double integral) -> double
			{
				for (int i = range.begin(); i < range.end(); i++)
				{
					integral = integral + func((double)(a + (double)i * h[j])) * h[j];
				}
				return integral;
			},
			std::plus<double>()
		);
		double integral = sum;
		high_resolution_clock::time_point t2 = high_resolution_clock::now();
		duration<double> duration = (t2 - t1);

		cout << integral << "|" << refer - integral << "|" << duration.count() << " seconds" << endl;
	}

	return 0;
}
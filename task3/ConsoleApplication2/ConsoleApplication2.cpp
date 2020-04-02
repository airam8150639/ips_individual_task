#include <cilk/cilk.h>
#include <stdlib.h>
#include <iostream>
#include <math.h>
#include <chrono>
#include <cilk/reducer_opadd.h>
using namespace std;
using namespace std::chrono;

# define M_PI           3.14159265358979323846  /* pi, #include <math.h> */
double refer = 2.094395;

double func(double x)
{
	return 4 / (sqrt(4 - x * x));
}

int main()
{
	double double_integral;
	int a = 0;
	int b = 1;	
	int q[5] = { 100, 1000, 10000, 100000, 1000000 };
	double h[5];
	for (int i = 0; i < 5; i++)
		h[i] = (double)(b - a) / (double)q[i];

	cout << " Value " << "|" << "  Error  " << "|" << "Time" << endl;
	
	for (int j = 0; j < 5; j++)
	{
		high_resolution_clock::time_point t1 = high_resolution_clock::now();
		cilk::reducer_opadd<double> integral(0);
		cilk_for (int i = 0; i < q[j]; i++)
		{
			integral = integral + func((double)(a + i * h[j] + h[j]/2))*h[j];
		}
		double_integral = integral.get_value();
		high_resolution_clock::time_point t2 = high_resolution_clock::now();
		duration<double> duration = (t2 - t1);

		cout << double_integral << "|" << abs(refer - double_integral) << "|" << duration.count() << " seconds" << endl;
	}

	return 0;
}
#include <iostream>
#include <iomanip>
#include <locale>
#include <chrono>
#include <thread>
#include <mutex>
#include <cmath>

# define M_PI           3.14159265358979323846  /* pi, #include <math.h> */
using namespace std;
using namespace std::chrono;
double refer = 2.094395;
//double refer = 0.5;
double tmr = 0;
double value;
mutex gmutex;

double f(double x)
{
	return 4 / (sqrt(4 - x * x));
	//return  x;
}

void compute1(int n) {
	int a = 0;
	int b = 1;
	double h = (double)(b - a) / (double)n;
	double sum=0;
	high_resolution_clock::time_point t1 = high_resolution_clock::now();
	for (int i = 0; i < n; i++) {
		sum = sum + (4 / sqrt(4 - pow((h * i + h / 2), 2))) * h;
	}
	high_resolution_clock::time_point t2 = high_resolution_clock::now();
	cout << setprecision(25) << sum - refer << endl;
	duration<double> duration = (t2 - t1);
	cout << "Duration is: " << setprecision(5) << duration.count() << " seconds" << endl;
}

void compute2(double a, double b, int n) {
	double h = (double)(b - a) / (double)n;
	double sum=0;
	high_resolution_clock::time_point t1 = high_resolution_clock::now();	
	for (int i = 0; i < n; i++) {
		//sum = sum + (4 / sqrt(4 - pow((h * i + h / 2), 2))) * h;
		sum = sum + f(a+h*i+h/2)*h;
	}
	//cout << "sum "<<sum<<endl;
	high_resolution_clock::time_point t2 = high_resolution_clock::now();
	gmutex.lock();
	value += sum;
	//cout << "value "<<value << endl;
	duration<double> duration = (t2 - t1);
	tmr += duration.count();
	gmutex.unlock();
}

int main() {
	setlocale(LC_ALL, "Rus");
	//#pragma loop(no_vector)
	//#pragma loop(hint_parallel(4))
	for (int i = 100; i < 10000001; i *= 10) {
		value = 0;
		tmr = 0;
		cout << "[" << i << "]" << endl;
		cout << "Без потоков: ";
		compute1(i);
		cout << "С потоками: ";
		thread t1(compute2, 0, 0.2, i);
		thread t2(compute2, 0.2, 0.4, i);
		thread t3(compute2, 0.4, 0.6, i);
		thread t4(compute2, 0.6, 0.8, i);
		thread t5(compute2, 0.8, 1.0, i);
		t1.join();
		t2.join();
		t3.join();
		t4.join();
		t5.join();
		cout  << value - refer << endl;
		cout << "Duration is:" << tmr << " seconds" << endl;
	}
	return 0;
}

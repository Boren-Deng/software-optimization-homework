#include "Executor.h"
#include "Logger.h"

#include<string>
#include<sstream>
#include<iomanip>

using namespace std;

Executor::Executor(unsigned long N)
	:N(N)
{
	h = HALF_PI / 2 / N;
}

void Executor::execute()
{
	double result = 0.0;
	Logger log;
	stringstream ss;
	
	ss << setprecision(16);
	log(Logger::start);
	result=commonScheme();
	log(Logger::end, "common scheme");
	ss << result;
	log("common scheme result" + ss.str());
	ss.str("");
	ss.clear();

	ss << setprecision(16);
	log(Logger::start);
	result = loopUnrollScheme();
	log(Logger::end, "loop unroll scheme");
	ss << result;
	log("loop unroll scheme result: " + ss.str());
}

double Executor::commonScheme()
{
	double result = 0.0;
	for (size_t k = 0; k < N; k++)
	{
		double kh = k * h,kh_2=k*h*k*h;
		result += (1.0 - (1.0 / FACTORIAL_3 + (1.0 / FACTORIAL_5 - kh_2 / FACTORIAL_7) * kh_2) * kh_2) * kh * h;
	}
	return result;
}

double Executor::loopUnrollScheme()
{
	double result = 0.0;
	unsigned long n=N-N%4;
	for (size_t k = 0; k < n; k+=4)
	{
		double kh = k * h, kh_2 = k * h * k * h,
			k1h = (k + 1) * h, k1h_2 = (k + 1) * h * (k + 1) * h,
			k2h = (k + 2) * h, k2h_2 = (k + 2) * h * (k + 2) * h,
			k3h = (k + 3) * h, k3h_2 = (k + 3) * h * (k + 3) * h;
		result += (1.0 - (1.0 / FACTORIAL_3 + (1.0 / FACTORIAL_5 - kh_2 / FACTORIAL_7) * kh_2) * kh_2) * kh * h+
			(1.0 - (1.0 / FACTORIAL_3 + (1.0 / FACTORIAL_5 - k1h_2 / FACTORIAL_7) * k1h_2) * k1h_2) * k1h * h+
			(1.0 - (1.0 / FACTORIAL_3 + (1.0 / FACTORIAL_5 - k2h_2 / FACTORIAL_7) * k2h_2) * k2h_2) * k2h * h+
			(1.0 - (1.0 / FACTORIAL_3 + (1.0 / FACTORIAL_5 - k3h_2 / FACTORIAL_7) * k3h_2) * k3h_2) * k3h * h;
	}
	for (size_t k = n; k < N; k++)
	{
		double kh = k * h, kh_2 = k * h * k * h;
		result += (1 - (1 / FACTORIAL_3 + (1 / FACTORIAL_5 - kh_2 / FACTORIAL_7) * kh_2) * kh_2) * kh * h;
	}
	return result;
}


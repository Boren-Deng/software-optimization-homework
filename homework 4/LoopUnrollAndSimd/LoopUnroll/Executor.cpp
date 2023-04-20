#include "Executor.h"
#include "Logger.h"

#include <string>
#include <sstream>
#include <iomanip>
#include <immintrin.h>
#include <emmintrin.h>

using namespace std;

Executor::Executor(unsigned long N)
	:N(N)
{
	h = HALF_PI  / N;
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
	log("common scheme result: " + ss.str());
	ss.str("");
	ss.clear();

	ss << setprecision(16);
	log(Logger::start);
	result = loopUnrollScheme();
	log(Logger::end, "loop unroll scheme");
	ss << result;
	log("loop unroll scheme result: " + ss.str());
	ss.str("");
	ss.clear();

	ss << setprecision(16);
	log(Logger::start);
	result = loopUnroolAndSimdScheme();
	log(Logger::end, "loop unroll and SIMD scheme");
	ss << result;
	log("loop unroll and SIMD scheme result: " + ss.str());
}

#pragma optimize("", off)
double Executor::commonScheme()
{
	double result = 0.0;
	for (size_t k = 0; k < N; k++)
	{
		double kh = k * h,kh_2=kh*kh;
		result += (1.0 + ( FACTORIAL_3 + ( FACTORIAL_5 + kh_2 * FACTORIAL_7) * kh_2) * kh_2) * kh * h;
	}
	return result;
}
#pragma optimize("", on)

double Executor::loopUnrollScheme()
{
	double result = 0.0;
	unsigned long n=N-N%4;
	for (size_t k = 0; k < n; k+=4)
	{
		double kh = k * h, kh_2 = kh*kh,
			k1h = (k + 1) * h, k1h_2 = k1h*k1h,
			k2h = (k + 2) * h, k2h_2 = k2h*k2h,
			k3h = (k + 3) * h, k3h_2 = k3h*k3h;
		result += (1.0 + ( FACTORIAL_3 + ( FACTORIAL_5 + kh_2 * FACTORIAL_7) * kh_2) * kh_2) * kh * h+
			(1.0 + ( FACTORIAL_3 + ( FACTORIAL_5 + k1h_2 * FACTORIAL_7) * k1h_2) * k1h_2) * k1h * h+
			(1.0 + ( FACTORIAL_3 + ( FACTORIAL_5 + k2h_2 * FACTORIAL_7) * k2h_2) * k2h_2) * k2h * h+
			(1.0 + ( FACTORIAL_3 + ( FACTORIAL_5 + k3h_2 * FACTORIAL_7) * k3h_2) * k3h_2) * k3h * h;
	}
	for (size_t k = n; k < N; k++)
	{
		double kh = k * h, kh_2 = kh*kh;
		result += (1.0 + ( FACTORIAL_3 + ( FACTORIAL_5 + kh_2 * FACTORIAL_7) * kh_2) * kh_2) * kh * h;
	}
	return result;
}

double Executor::loopUnroolAndSimdScheme()
{
	double result = 0.0;
	unsigned long n = N - N % 4;
	double kh_arr[4], result_arr[4];
	__m256d resultReg=_mm256_set1_pd(0.0);
	for (size_t k = 0; k < n; k += 4)
	{
		kh_arr[0] = k * h,
			kh_arr[1] = (k + 1) * h,
			kh_arr[2] = (k + 2) * h,
			kh_arr[3] = (k + 3) * h;

		__m256d h_ = _mm256_set1_pd(h);
		__m256d kh = _mm256_load_pd(kh_arr);
		__m256d kh_2 = _mm256_mul_pd(kh, kh);
		__m256d fac_1 = _mm256_set1_pd(1.0);
		__m256d fac_3 = _mm256_set1_pd(FACTORIAL_3);
		__m256d fac_5 = _mm256_set1_pd(FACTORIAL_5);
		__m256d fac_7 = _mm256_set1_pd(FACTORIAL_7);

		resultReg = _mm256_add_pd(resultReg, _mm256_mul_pd(_mm256_mul_pd(_mm256_fmadd_pd(kh_2, _mm256_fmadd_pd(kh_2, _mm256_fmadd_pd(kh_2, fac_7, fac_5), fac_3), fac_1), kh), h_));
	}
	_mm256_store_pd(result_arr, resultReg);
	result += result_arr[0] + result_arr[1] + result_arr[2] + result_arr[3];

	for (size_t k = n; k < N; k++)
	{
		double kh = k * h, kh_2 = kh * kh;
		result += (1.0 + (FACTORIAL_3 + (FACTORIAL_5 + kh_2 * FACTORIAL_7) * kh_2) * kh_2) * kh * h;
	}
	return result;
}


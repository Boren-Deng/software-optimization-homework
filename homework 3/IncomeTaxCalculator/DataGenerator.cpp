#include "DataGenerator.h"

#include<random>

using namespace std;

DataGenerator::DataGenerator(size_t N)
	:N(N)
{
	salary.reserve(N);

	mt19937 engine(SEED);
	uniform_real_distribution<> salaryDistribution(0.0, 150000.0);

	for (size_t i = 0; i < N; i++)
	{
		salary.emplace_back(move(salaryDistribution(engine)));
	}
}


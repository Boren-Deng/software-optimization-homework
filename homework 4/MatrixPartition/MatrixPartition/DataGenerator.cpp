#include "DataGenerator.h"
#include <random>

using namespace std;

DataGenerator::DataGenerator()
{
	mt19937 engine(SEED);
	uniform_real_distribution<float> distribution(-1000.0f, 1000.0f);

	for (size_t i = 0; i < 512; i++)
	{
		for (size_t j=0;j<512;j++)
		{
			matrix_512.first[i][j] = distribution(engine);
			matrix_512.second[i][j] = distribution(engine);
		}
	}
	for (size_t i = 0; i < 1024; i++)
	{
		for (size_t j = 0; j < 1024; j++)
		{
			matrix_1024.first[i][j] = distribution(engine);
			matrix_1024.second[i][j] = distribution(engine);
		}
	}
	for (size_t i = 0; i < 2048; i++)
	{
		for (size_t j = 0; j < 2048; j++)
		{
			matrix_2048.first[i][j] = distribution(engine);
			matrix_2048.second[i][j] = distribution(engine);
		}
	}
	for (size_t i = 0; i < 4096; i++)
	{
		for (size_t j = 0; j < 4096; j++)
		{
			matrix_4096.first[i][j] = distribution(engine);
			matrix_4096.second[i][j] = distribution(engine);
		}
	}
}

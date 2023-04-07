#pragma once
#ifndef DATAGENERATOR_H
#define DATAGENERATOR_H

#include<vector>

class DataGenerator
{
public:
	DataGenerator(size_t N);
	~DataGenerator() = default;
	const std::vector<float>& getSalary() { return salary; }
private:
	size_t N;
	constexpr static unsigned int SEED = 2023;
	std::vector<float>salary;
};

#endif // !DATAGENERATOR_H


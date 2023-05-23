#pragma once
#ifndef DATAGENERATOR_H
#define DATAGENERATOR_H

#include <vector>
class DataGenerator
{
public:
	DataGenerator();
	~DataGenerator() = default;

private:
	constexpr static unsigned long SEED = 2023;

	using matrixType = std::vector<std::vector<float>>;

	std::pair <matrixType,matrixType>matrix_512=std::make_pair(matrixType(512, std::vector<float>(512, 0.0f)),
		matrixType(512, std::vector<float>(512, 0.0f)));
	std::pair <matrixType, matrixType>matrix_1024 = std::make_pair(matrixType(1024, std::vector<float>(1024, 0.0f)),
		matrixType(1024, std::vector<float>(1024, 0.0f)));
	std::pair <matrixType, matrixType>matrix_2048 = std::make_pair(matrixType(2048, std::vector<float>(2048, 0.0f)),
		matrixType(2048, std::vector<float>(2048, 0.0f)));
	std::pair <matrixType, matrixType>matrix_4096 = std::make_pair(matrixType(4096, std::vector<float>(4096, 0.0f)),
		matrixType(4096, std::vector<float>(4096, 0.0f)));
public:
	const std::pair<matrixType,matrixType>& getMatrix512() const { return matrix_512; }
	const std::pair<matrixType,matrixType>& getMatrix1024() const { return matrix_1024; }
	const std::pair<matrixType,matrixType>& getMatrix2048() const { return matrix_2048; }
	const std::pair<matrixType, matrixType>& getMatrix4096() const { return matrix_4096; }

};
#endif

#pragma once
#ifndef EXECUTOR_H
#define EXECUTOR_H

#include <vector>
#include <cstddef>
#include "DataGenerator.h"

class Executor
{
public:
	explicit Executor(const DataGenerator&dg);
	~Executor() = default;
	void execute();
	void resetResults();

private:
	struct PartitionInfo
	{
		size_t aStartRow, aEndRow,
			bStartCol, bEndCol,
			aColNum,
			aStartCol, bStartRow;
	};
private:
	std::vector<std::vector<float>>resultMatrix_512,resultMatrix_1024, resultMatrix_2048, resultMatrix_4096;
	const DataGenerator& dg;

	void commonScheme();
	void matrixPartitionScheme(size_t sideLen);
	void calCheckSum() const;

	void matrixMulti(const std::vector<std::vector<float>>& inputMatrixA, const std::vector<std::vector<float>>& inputMatrixB,
	                 std::vector<std::vector<float>>& resultMatrix, const PartitionInfo& info);
};
#endif
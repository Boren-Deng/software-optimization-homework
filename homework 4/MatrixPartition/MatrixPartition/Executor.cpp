#include "Executor.h"
#include "Logger.h"
using namespace std;

Executor::Executor(const DataGenerator& dg)
	: resultMatrix_512(512, vector<float>(512, 0.0f)), resultMatrix_1024(1024, vector<float>(1024, 0.0f)),
	  resultMatrix_2048(2048, vector<float>(2048, 0.0f)), resultMatrix_4096(4096, vector<float>(4096, 0.0f)),
	  dg(dg)
{
}

void Executor::execute()
{
	commonScheme();
	resetResults();

	matrixPartitionScheme(8);
	resetResults();

	matrixPartitionScheme(16);
	resetResults();

	matrixPartitionScheme(32);
	resetResults();

	matrixPartitionScheme(64);
	resetResults();

	matrixPartitionScheme(128);
	resetResults();
}

void Executor::resetResults()
{
	for (auto& eachRow : resultMatrix_512)
		fill(eachRow.begin(), eachRow.end(), 0.0f);
	for (auto& eachRow : resultMatrix_1024)
		fill(eachRow.begin(), eachRow.end(), 0.0f);
	for (auto& eachRow : resultMatrix_2048)
		fill(eachRow.begin(), eachRow.end(), 0.0f);
	for (auto& eachRow : resultMatrix_4096)
		fill(eachRow.begin(), eachRow.end(), 0.0f);
}

void Executor::commonScheme()
{
	Logger l;
	PartitionInfo info{};

	//matrix 512*512 multiplication using common scheme
	info = {0, 511, 0, 511, 512, 0, 0};
	l(Logger::start);
	matrixMulti(dg.getMatrix512().first, dg.getMatrix512().second, resultMatrix_512, info);
	l(Logger::end, "common scheme matrix 512*512 multi");

	//matrix 1024*1024 multiplication using common scheme
	info = { 0, 1023, 0, 1023, 1024, 0, 0 };
	l(Logger::start);
	matrixMulti(dg.getMatrix1024().first, dg.getMatrix1024().second, resultMatrix_1024, info);
	l(Logger::end, "common scheme matrix 1024*1024 multi");

	//matrix 2048*2048 multiplication using common scheme
	info = { 0, 2047, 0, 2047, 2048, 0, 0 };
	l(Logger::start);
	matrixMulti(dg.getMatrix2048().first, dg.getMatrix2048().second, resultMatrix_2048, info);
	l(Logger::end, "common scheme matrix 2048*2048 multi");

	//matrix 4096*4096 multiplication using common scheme
	info = { 0, 4095, 0, 4095, 4096, 0, 0 };
	l(Logger::start);
	matrixMulti(dg.getMatrix4096().first, dg.getMatrix4096().second, resultMatrix_4096, info);
	l(Logger::end, "common scheme matrix 4096*4096 multi");
	calCheckSum();
}

void Executor::matrixPartitionScheme(size_t sideLen)
{
	Logger l;
	PartitionInfo info{ 0,0,0,0,sideLen,0,0 };
	size_t partitionRow, partitionCol, kLength;

	//matrix 512*512 multiplication using sideLen*sideLen block scheme
	partitionRow = 512/sideLen;
	partitionCol = 512/sideLen;
	kLength = 512/sideLen;
	l(Logger::start);
	for (size_t i = 0; i < partitionRow; i++)
	{
		for (size_t j = 0; j < partitionCol; j++)
		{
			for (size_t k = 0; k < kLength; k++)
			{
				info.aStartRow = i * sideLen;
				info.aEndRow = i * sideLen + sideLen-1;
				info.bStartCol = j * sideLen;
				info.bEndCol = j * sideLen + sideLen-1;
				info.aStartCol = k * sideLen;
				info.bStartRow = k * sideLen;
				matrixMulti(dg.getMatrix512().first, dg.getMatrix512().second,
					resultMatrix_512, info);
			}
		}
	}
	l(Logger::end, to_string(sideLen)+"*"+to_string(sideLen)+" partition scheme matrix 512 * 512 multi");

	//matrix 1024*1024 multiplication using sideLen*sideLen block scheme
	partitionRow = 1024 / sideLen;
	partitionCol = 1024 / sideLen;
	kLength = 1024 / sideLen;
	l(Logger::start);
	for (size_t i = 0; i < partitionRow; i++)
	{
		for (size_t j = 0; j < partitionCol; j++)
		{
			for (size_t k = 0; k < kLength; k++)
			{
				info.aStartRow = i * sideLen;
				info.aEndRow = i * sideLen + sideLen - 1;
				info.bStartCol = j * sideLen;
				info.bEndCol = j * sideLen + sideLen - 1;
				info.aStartCol = k * sideLen;
				info.bStartRow = k * sideLen;
				matrixMulti(dg.getMatrix1024().first, dg.getMatrix1024().second,
					resultMatrix_1024, info);
			}
		}
	}
	l(Logger::end, to_string(sideLen) + "*" + to_string(sideLen) + " partition scheme matrix 1024*1024 multi");

	//matrix 2048*2048 multiplication using sideLen*sideLen block scheme
	partitionRow = 2048 / sideLen;
	partitionCol = 2048 / sideLen;
	kLength = 2048 / sideLen;
	l(Logger::start);
	for (size_t i = 0; i < partitionRow; i++)
	{
		for (size_t j = 0; j < partitionCol; j++)
		{
			for (size_t k = 0; k < kLength; k++)
			{
				info.aStartRow = i * sideLen;
				info.aEndRow = i * sideLen + sideLen - 1;
				info.bStartCol = j * sideLen;
				info.bEndCol = j * sideLen + sideLen - 1;
				info.aStartCol = k * sideLen;
				info.bStartRow = k * sideLen;
				matrixMulti(dg.getMatrix2048().first, dg.getMatrix2048().second,
					resultMatrix_2048, info);
			}
		}
	}
	l(Logger::end, to_string(sideLen) + "*" + to_string(sideLen) + " partition scheme matrix 2048*2048 multi");

	//matrix 4096*4096 multiplication using sideLen*sideLen block scheme
	partitionRow = 4096 / sideLen;
	partitionCol = 4096 / sideLen;
	kLength = 4096 / sideLen;
	l(Logger::start);
	for (size_t i = 0; i < partitionRow; i++)
	{
		for (size_t j = 0; j < partitionCol; j++)
		{
			for (size_t k = 0; k < kLength; k++)
			{
				info.aStartRow = i * sideLen;
				info.aEndRow = i * sideLen + sideLen - 1;
				info.bStartCol = j * sideLen;
				info.bEndCol = j * sideLen + sideLen - 1;
				info.aStartCol = k * sideLen;
				info.bStartRow = k * sideLen;
				matrixMulti(dg.getMatrix4096().first, dg.getMatrix4096().second,
					resultMatrix_4096, info);
			}
		}
	}
	l(Logger::end, to_string(sideLen) + "*" + to_string(sideLen) + " partition scheme matrix 4096*4096 multi");
	calCheckSum();
}

void Executor::calCheckSum() const
{
	Logger l;
	float result = 0.0f;
	for (const auto& eachRow : resultMatrix_512)
	{
		for (const auto& element : eachRow)
		{
			result += element;
		}
	}
	l("check sum of result matrix 512*512:" + to_string(result));

	result = 0.0f;
	for (const auto& eachRow : resultMatrix_1024)
	{
		for (const auto& element : eachRow)
		{
			result += element;
		}
	}
	l("check sum of result matrix 1024*1024:" + to_string(result));

	result = 0.0f;
	for (const auto& eachRow : resultMatrix_2048)
	{
		for (const auto& element : eachRow)
		{
			result += element;
		}
	}
	l("check sum of result matrix 2048*2048:" + to_string(result));

	result = 0.0f;
	for (const auto& eachRow : resultMatrix_4096)
	{
		for (const auto& element : eachRow)
		{
			result += element;
		}
	}
	l("check sum of result matrix 4096*4096:" + to_string(result));
}

void Executor::matrixMulti(const std::vector<std::vector<float>>& inputMatrixA,
                           const std::vector<std::vector<float>>& inputMatrixB,
                           std::vector<std::vector<float>>& resultMatrix, const PartitionInfo& info)
{
	for (size_t i = info.aStartRow; i <= info.aEndRow; i++)
	{
		for (size_t j = info.bStartCol; j <= info.bEndCol; j++)
		{
			for (size_t k = 0; k < info.aColNum; k++)
			{
				resultMatrix[i][j] += inputMatrixA[i][info.aStartCol + k] * inputMatrixB[info.bStartRow + k][j];
			}
		}
	}
}

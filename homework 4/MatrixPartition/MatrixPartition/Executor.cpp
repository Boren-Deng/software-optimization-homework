﻿#include "Executor.h"
#include "Logger.h"
#include <emmintrin.h>
#include <thread>
#ifndef __linux__
#include "windows.h"
#else
#include "unistd.h"
#include "sys/sysinfo.h"
#endif

#define ENABLE_SIMD
#define MULTI_THREAD
using namespace std;

Executor::Executor(const DataGenerator& dg)
	: resultMatrix_512(512, vector<float>(512, 0.0f)), resultMatrix_1024(1024, vector<float>(1024, 0.0f)),
	  resultMatrix_2048(2048, vector<float>(2048, 0.0f)), resultMatrix_4096(4096, vector<float>(4096, 0.0f)),
	  dg(dg)
{
}

void Executor::execute()
{
	/*commonScheme();
	resetResults();*/

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

#ifdef MULTI_THREAD
	vector<thread>workThreads;
	int cpuNum, taskNum,taskBalanceNum;
#ifndef __linux__
	SYSTEM_INFO sysInfo;
	GetSystemInfo(&sysInfo);
	cpuNum = sysInfo.dwNumberOfProcessors;
#else
	cpuNum = get_nprocs();
#endif
	workThreads.reserve(cpuNum);
#endif

	//matrix 512*512 multiplication using sideLen*sideLen block scheme
	partitionRow = 512/sideLen;
	partitionCol = 512/sideLen;
	kLength = 512/sideLen;
	l(Logger::start);
#ifndef MULTI_THREAD
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
#else
	taskNum = min(cpuNum, static_cast<int>(partitionRow));
	taskBalanceNum = partitionRow % taskNum;
	for(int thd=0;thd<taskNum;thd++)
	{
		int threadRowPartition = partitionRow / taskNum;
		if (taskBalanceNum > 0)
		{
			threadRowPartition++;
			taskBalanceNum--;
		}
		workThreads.emplace_back([=](size_t beginRowPartition, size_t endRowPartition)mutable 
			{
				for (size_t i = beginRowPartition; i < endRowPartition; i++)
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
							matrixMulti(dg.getMatrix512().first, dg.getMatrix512().second,
								resultMatrix_512, info);
						}
					}
				}
			},thd*threadRowPartition,(thd+1)*threadRowPartition);
	}
	for (auto& eachThread : workThreads)
		eachThread.join();
#endif
	l(Logger::end, to_string(sideLen)+"*"+to_string(sideLen)+" partition scheme matrix 512 * 512 multi");
#ifdef MULTI_THREAD
	workThreads.clear();
#endif

	//matrix 1024*1024 multiplication using sideLen*sideLen block scheme
	partitionRow = 1024 / sideLen;
	partitionCol = 1024 / sideLen;
	kLength = 1024 / sideLen;
	l(Logger::start);
#ifndef MULTI_THREAD
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
#else
	taskNum = min(cpuNum, static_cast<int>(partitionRow));
	taskBalanceNum = partitionRow % taskNum;
	for (int thd = 0; thd < taskNum; thd++)
	{
		int threadRowPartition = partitionRow / taskNum;
		if (taskBalanceNum > 0)
		{
			threadRowPartition++;
			taskBalanceNum--;
		}
		workThreads.emplace_back([=](size_t beginRowPartition, size_t endRowPartition)mutable
			{
				for (size_t i = beginRowPartition; i < endRowPartition; i++)
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
			}, thd * threadRowPartition, (thd + 1) * threadRowPartition);
	}
	for (auto& eachThread : workThreads)
		eachThread.join();
#endif
	l(Logger::end, to_string(sideLen) + "*" + to_string(sideLen) + " partition scheme matrix 1024*1024 multi");
#ifdef MULTI_THREAD
	workThreads.clear();
#endif

	//matrix 2048*2048 multiplication using sideLen*sideLen block scheme
	partitionRow = 2048 / sideLen;
	partitionCol = 2048 / sideLen;
	kLength = 2048 / sideLen;
	l(Logger::start);
#ifndef MULTI_THREAD
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
#else
	taskNum = min(cpuNum, static_cast<int>(partitionRow));
	taskBalanceNum = partitionRow % taskNum;
	for (int thd = 0; thd < taskNum; thd++)
	{
		int threadRowPartition = partitionRow / taskNum;
		if (taskBalanceNum > 0)
		{
			threadRowPartition++;
			taskBalanceNum--;
		}
		workThreads.emplace_back([=](size_t beginRowPartition, size_t endRowPartition)mutable
			{
				for (size_t i = beginRowPartition; i < endRowPartition; i++)
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
			}, thd * threadRowPartition, (thd + 1) * threadRowPartition);
	}
	for (auto& eachThread : workThreads)
		eachThread.join();
#endif
	l(Logger::end, to_string(sideLen) + "*" + to_string(sideLen) + " partition scheme matrix 2048*2048 multi");
#ifdef MULTI_THREAD
	workThreads.clear();
#endif

	//matrix 4096*4096 multiplication using sideLen*sideLen block scheme
	partitionRow = 4096 / sideLen;
	partitionCol = 4096 / sideLen;
	kLength = 4096 / sideLen;
	l(Logger::start);
#ifndef MULTI_THREAD
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
#else
	taskNum = min(cpuNum, static_cast<int>(partitionRow));
	taskBalanceNum = partitionRow % taskNum;
	for (int thd = 0; thd < taskNum; thd++)
	{
		int threadRowPartition = partitionRow / taskNum;
		if (taskBalanceNum > 0)
		{
			threadRowPartition++;
			taskBalanceNum--;
		}
		workThreads.emplace_back([=](size_t beginRowPartition, size_t endRowPartition)mutable
			{
				for (size_t i = beginRowPartition; i < endRowPartition; i++)
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
			}, thd * threadRowPartition, (thd + 1) * threadRowPartition);
	}
	for (auto& eachThread : workThreads)
		eachThread.join();
#endif
	l(Logger::end, to_string(sideLen) + "*" + to_string(sideLen) + " partition scheme matrix 4096*4096 multi");
#ifdef MULTI_THREAD
	workThreads.clear();
#endif
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
#ifdef ENABLE_SIMD
	__m128 currentRow, currentCol;
#ifdef _MSC_VER
	__declspec(align(16)) float row_arr[4], col_arr[4], result_arr[4];
#else
	alignas(16) float row_arr[4], col_arr[4], result_arr[4];
#endif
	for (size_t i = info.aStartRow; i <= info.aEndRow; i++)
	{
		for (size_t j = info.bStartCol; j <= info.bEndCol; j++)
		{
			__m128 tempResult = _mm_set_ps1(0.0f);
			for (size_t k = 0; k < info.aColNum; k+=4)
			{
				row_arr[0] = inputMatrixA[i][info.aStartCol + k + 0];
				row_arr[1] = inputMatrixA[i][info.aStartCol + k + 1];
				row_arr[2] = inputMatrixA[i][info.aStartCol + k + 2];
				row_arr[3] = inputMatrixA[i][info.aStartCol + k + 3];
				col_arr[0] = inputMatrixB[info.bStartRow + k + 0][j];
				col_arr[1] = inputMatrixB[info.bStartRow + k + 1][j];
				col_arr[2] = inputMatrixB[info.bStartRow + k + 2][j];
				col_arr[3] = inputMatrixB[info.bStartRow + k + 3][j];
				currentRow = _mm_load_ps(row_arr);
				currentCol = _mm_load_ps(col_arr);
				tempResult = _mm_add_ps(tempResult, _mm_mul_ps(currentRow, currentCol));
			}
			_mm_store_ps(result_arr, tempResult);
			resultMatrix[i][j] += result_arr[0] + result_arr[1] + result_arr[2] + result_arr[3];
		}
	}

#else
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
#endif // ENABLE_SIMD
}

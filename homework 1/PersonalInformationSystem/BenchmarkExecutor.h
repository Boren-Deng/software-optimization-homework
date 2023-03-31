#pragma once
#ifndef BENCHMARKEXECUTOR_H
#define BENCHMARKEXECUTOR_H
#include "Logger.h"
#include "DataGenerator.h"
#include "defines.h"

class BenchmarkExecutor
{
public:
	BenchmarkExecutor(DataGenerator& dg);
	~BenchmarkExecutor() = default;
	void execute();
private:
	void scheme1();
	void scheme2();
	void scheme3();
	void scheme4();

private:
	DataGenerator& dataGenerator;
	Logger logger;
};
#endif // !BENCHMARKEXECUTOR_H

#include"BenchmarkExecutor.h"
#include "defines.h"
#include "Logger.h"
#include "ManagementSystem.hpp"
#include "DataGenerator.h"

#include<string>

using namespace std;

BenchmarkExecutor::BenchmarkExecutor(DataGenerator& dg)
	:dataGenerator(dg),logger()
{
}

void BenchmarkExecutor::execute()
{
	scheme1();
	scheme2();
	scheme3();
	scheme4();
}

void BenchmarkExecutor::scheme1()
{
	ManagementSystem<string, person>managementSystem;

	logger(Logger::start);
	for (auto& each : dataGenerator.getSmallBatch())
		managementSystem.person_insert(each);
	logger(Logger::end, "scheme 1 small batch insert");

	logger(Logger::start);
	for (const auto& each : dataGenerator.getQuery())
		managementSystem.person_search(each);
	logger(Logger::end, "scheme 1 small batch search");
	managementSystem.clearData();

	logger(Logger::start);
	for (auto& each : dataGenerator.getMediumBatch())
		managementSystem.person_insert(each);
	logger(Logger::end, "scheme 1 medium batch insert");

	logger(Logger::start);
	for (const auto& each : dataGenerator.getQuery())
		managementSystem.person_search(each);
	logger(Logger::end, "scheme 1 medium batch search");
	managementSystem.clearData();

	logger(Logger::start);
	for (auto& each : dataGenerator.getLargeBatch())
		managementSystem.person_insert(each);
	logger(Logger::end, "scheme 1 large batch insert");

	logger(Logger::start);
	for (const auto& each : dataGenerator.getQuery())
		managementSystem.person_search(each);
	logger(Logger::end, "scheme 1 large batch search");
	managementSystem.clearData();
}

void BenchmarkExecutor::scheme2()
{
	ManagementSystem<string, int>managementSystem;

	logger(Logger::start);
	for (auto& each : dataGenerator.getSmallBatchScheme2())
		managementSystem.person_insert(each);
	logger(Logger::end, "scheme 2 small batch insert");

	logger(Logger::start);
	for (const auto& each : dataGenerator.getQuery())
		managementSystem.person_search(each);
	logger(Logger::end, "scheme 2 small batch search");
	managementSystem.clearData();

	logger(Logger::start);
	for (auto& each : dataGenerator.getMediumBatchScheme2())
		managementSystem.person_insert(each);
	logger(Logger::end, "scheme 2 medium batch insert");

	logger(Logger::start);
	for (const auto& each : dataGenerator.getQuery())
		managementSystem.person_search(each);
	logger(Logger::end, "scheme 2 medium batch search");
	managementSystem.clearData();

	logger(Logger::start);
	for (auto& each : dataGenerator.getLargeBatchScheme2())
		managementSystem.person_insert(each);
	logger(Logger::end, "scheme 2 large batch insert");

	logger(Logger::start);
	for (const auto& each : dataGenerator.getQuery())
		managementSystem.person_search(each);
	logger(Logger::end, "scheme 2 large batch search");
	managementSystem.clearData();
}

void BenchmarkExecutor::scheme3()
{
	ManagementSystem<long long, int>managementSystem;

	logger(Logger::start);
	for (auto& each : dataGenerator.getSmallBatchScheme3())
		managementSystem.person_insert(each);
	logger(Logger::end, "scheme 3 small batch insert");

	logger(Logger::start);
	for (const auto& each : dataGenerator.getQueryForScheme3())
		managementSystem.person_search(each);
	logger(Logger::end, "scheme 3 small batch search");
	managementSystem.clearData();

	logger(Logger::start);
	for (auto& each : dataGenerator.getMediumBatchScheme3())
		managementSystem.person_insert(each);
	logger(Logger::end, "scheme 3 medium batch insert");

	logger(Logger::start);
	for (const auto& each : dataGenerator.getQueryForScheme3())
		managementSystem.person_search(each);
	logger(Logger::end, "scheme 3 medium batch search");
	managementSystem.clearData();

	logger(Logger::start);
	for (auto& each : dataGenerator.getLargeBatchScheme3())
		managementSystem.person_insert(each);
	logger(Logger::end, "scheme 3 large batch insert");

	logger(Logger::start);
	for (const auto& each : dataGenerator.getQueryForScheme3())
		managementSystem.person_search(each);
	logger(Logger::end, "scheme 3 large batch search");
	managementSystem.clearData();
}

void BenchmarkExecutor::scheme4()
{
	ManagementSystem<int, ManagementSystem<unsigned int,int>>managementSystem;

	logger(Logger::start);
	for (auto& each : dataGenerator.getSmallBatchScheme4())
		managementSystem[each.first.first][each.first.second]=each.second.second;
	logger(Logger::end, "scheme 4 small batch insert");

	logger(Logger::start);
	for (const auto& each : dataGenerator.getQueryForScheme4())
	{
		auto res = managementSystem.person_search(each.first);
		if (res != nullptr)
			res->person_search(each.second);
	}
	logger(Logger::end, "scheme 4 small batch search");
	managementSystem.clearData();

	logger(Logger::start);
	for (auto& each : dataGenerator.getMediumBatchScheme4())
		managementSystem[each.first.first][each.first.second] = each.second.second;
	logger(Logger::end, "scheme 4 medium batch insert");

	logger(Logger::start);
	for (const auto& each : dataGenerator.getQueryForScheme4())
	{
		auto res = managementSystem.person_search(each.first);
		if (res != nullptr)
			res->person_search(each.second);
	}
	logger(Logger::end, "scheme 4 medium batch search");
	managementSystem.clearData();

	logger(Logger::start);
	for (auto& each : dataGenerator.getLargeBatchScheme4())
		managementSystem[each.first.first][each.first.second] = each.second.second;
	logger(Logger::end, "scheme 4 large batch insert");

	logger(Logger::start);
	for (const auto& each : dataGenerator.getQueryForScheme4())
	{
		auto res = managementSystem.person_search(each.first);
		if (res != nullptr)
			res->person_search(each.second);
	}
	logger(Logger::end, "scheme 4 large batch search");
	managementSystem.clearData();
}

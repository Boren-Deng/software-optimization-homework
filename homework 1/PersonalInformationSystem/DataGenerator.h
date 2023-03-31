#pragma once
#ifndef DATAGENERATOR_H
#define DATAGENERATOR_H

#include<vector>
#include<array>
#include<random>
#include<string>

struct person;

class DataGenerator
{
public:
	DataGenerator(int seed);
	~DataGenerator() = default;
	std::vector<std::pair<std::string, person>>& getSmallBatch() { return smallBatch; }
	std::vector<std::pair<std::string, person>>& getMediumBatch() { return mediumBatch; }
	std::vector<std::pair<std::string, person>>& getLargeBatch() { return largeBatch; }

	std::vector<std::string>& getQuery() { return query; }//query for scheme 1 and 2

	std::vector<std::pair<std::string, int>>& getSmallBatchScheme2() { return smallBatchScheme2; }
	std::vector<std::pair<std::string, int>>& getMediumBatchScheme2() { return mediumBatchScheme2; }
	std::vector<std::pair<std::string, int>>& getLargeBatchScheme2() { return largeBatchScheme2; }

	std::vector<std::pair<long long, int>>& getSmallBatchScheme3() { return smallBatchScheme3; }
	std::vector<std::pair<long long, int>>& getMediumBatchScheme3() { return mediumBatchScheme3; }
	std::vector<std::pair<long long, int>>& getLargeBatchScheme3() { return largeBatchScheme3; }
	std::vector<long long>& getQueryForScheme3() { return queryForScheme3; }

	std::vector<std::pair<std::pair<int, unsigned int>, std::pair<unsigned int, int>>>& getSmallBatchScheme4() { return smallBatchScheme4; }
	std::vector<std::pair<std::pair<int, unsigned int>, std::pair<unsigned int, int>>>& getMediumBatchScheme4() { return mediumBatchScheme4; }
	std::vector<std::pair<std::pair<int, unsigned int>, std::pair<unsigned int, int>>>& getLargeBatchScheme4() { return largeBatchScheme4; }
	std::vector<std::pair<int, unsigned int>>& getQueryForScheme4() { return queryForScheme4; }

	const static std::array<int, 1000>regionNumber;
private:
	void fillCharArray(char* target, int offset, int count, int dataToFill);//将整形填入字符数组
	bool isLeapYear(int year);
	void fillEntry(std::vector<std::pair<std::string, person>>& target);
	void generateQuery();
	void scheme2DataConvert();
	void scheme3DataConvert();//call after function:scheme2DataConvert()
	void scheme4DataConvert();//call after function:scheme3DataConvert()
	int dayOfYear(int monthAndDay,bool isLeap);
	unsigned int compressId(long long id);


private:
	//raw data
	std::vector<std::pair<std::string, person>>smallBatch = std::vector<std::pair<std::string, person>>(100000);//10e5
	std::vector<std::pair<std::string, person>>mediumBatch = std::vector<std::pair<std::string, person>>(1000000);//10e6
	std::vector<std::pair<std::string, person>>largeBatch = std::vector<std::pair<std::string, person>>(10000000);//10e7
	std::vector<std::string>query = std::vector<std::string>(1000000);//query for scheme 1 and 2

	//scheme 2 data
	std::vector<std::pair<std::string, int>>smallBatchScheme2 = std::vector<std::pair<std::string, int>>(100000);
	std::vector<std::pair<std::string, int>>mediumBatchScheme2 = std::vector<std::pair<std::string, int>>(1000000);
	std::vector<std::pair<std::string, int>>largeBatchScheme2 = std::vector<std::pair<std::string, int>>(10000000);

	//scheme 3 data
	std::vector<std::pair<long long, int>>smallBatchScheme3 = std::vector<std::pair<long long, int>>(100000);
	std::vector<std::pair<long long, int>>mediumBatchScheme3 = std::vector<std::pair<long long, int>>(1000000);
	std::vector<std::pair<long long, int>>largeBatchScheme3 = std::vector<std::pair<long long, int>>(10000000);
	std::vector<long long>queryForScheme3 = std::vector<long long>(1000000);

	//scheme 4 data
	std::vector<std::pair<std::pair<int,unsigned int>,std::pair<unsigned int,int>>>smallBatchScheme4 = std::vector<std::pair<std::pair<int, unsigned int>, std::pair<unsigned int, int>>>(100000);
	std::vector<std::pair<std::pair<int, unsigned int>, std::pair<unsigned int, int>>>mediumBatchScheme4 = std::vector<std::pair<std::pair<int, unsigned int>, std::pair<unsigned int, int>>>(1000000);
	std::vector<std::pair<std::pair<int, unsigned int>, std::pair<unsigned int, int>>>largeBatchScheme4 = std::vector<std::pair<std::pair<int, unsigned int>, std::pair<unsigned int, int>>>(10000000);
	std::vector<std::pair<int,unsigned int>>queryForScheme4 = std::vector<std::pair<int, unsigned int>>(1000000);

	const static std::array<int, 13>daysOfMonth;

	std::mt19937 engine;
	std::uniform_int_distribution<> regionDistribution= std::uniform_int_distribution<>(0, 999);
	std::uniform_int_distribution<> yearDistribution= std::uniform_int_distribution<>(1900, 2023);
	std::uniform_int_distribution<> monthDistribution= std::uniform_int_distribution<>(1, 12);
	std::uniform_int_distribution<> lastFourDigitDistribution= std::uniform_int_distribution<>(0, 9999);
	std::uniform_int_distribution<> digitGenerate= std::uniform_int_distribution<>('0', '9');
	std::uniform_int_distribution<> upperGenerate= std::uniform_int_distribution<>('A', 'Z');
	std::uniform_int_distribution<> lowerGenerate= std::uniform_int_distribution<>('a', 'z');
	std::uniform_int_distribution<> addressGenerate= std::uniform_int_distribution<>(32, 125);
	std::uniform_int_distribution<> phoneNumHeaderGenerate= std::uniform_int_distribution<>(131, 189);
	std::uniform_int_distribution<> phoneNumRestGenerate= std::uniform_int_distribution<>(11111111, 99999999);
};
#endif // !DATAGENERATOR_H

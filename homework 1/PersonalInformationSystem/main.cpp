#include "DataGenerator.h"
#include "BenchmarkExecutor.h"

int main()
{
	DataGenerator *dataGenerator=new DataGenerator(2023);
	BenchmarkExecutor be(*dataGenerator);
	be.execute();
	delete dataGenerator;

	return 0;
}
#include "DataGenerator.h"
#include "Executor.h"

int main()
{
	DataGenerator dg(1e8);
	Executor e(dg.getSalary());
	e.execute();
	return 0;
}
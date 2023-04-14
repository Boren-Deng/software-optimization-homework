#include "DataGenerator.h"
#include "Executor.h"

int main()
{
	DataGenerator dg;
	Executor e(dg);
	e.execute();
	return 0;
}
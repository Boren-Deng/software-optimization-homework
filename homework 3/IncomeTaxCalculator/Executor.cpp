#include "Executor.h"
#include "Logger.h"

#include<algorithm>

using namespace std;

Executor::Executor(const vector<float>& data)
	:salary(data),salaryToTaxCal(301)
{
	fill(salaryToTaxCal.begin(), salaryToTaxCal.begin() + 7, &calNothing);
    fill(salaryToTaxCal.begin() + 7, salaryToTaxCal.begin() + 10, &cal0_1500);
    fill(salaryToTaxCal.begin() + 10, salaryToTaxCal.begin() + 13, &cal1500_3000);
    fill(salaryToTaxCal.begin() + 13, salaryToTaxCal.begin() + 25, &cal3000_9000);
    fill(salaryToTaxCal.begin() + 25, salaryToTaxCal.begin() + 78, &cal9000_35500);
    fill(salaryToTaxCal.begin() + 78, salaryToTaxCal.begin() + 117, &cal35500_55000);
    fill(salaryToTaxCal.begin() + 117, salaryToTaxCal.begin() + 167, &cal55000_80000);
    fill(salaryToTaxCal.begin() + 167, salaryToTaxCal.end(), &calLargeThan80000);
}


void Executor::execute()
{
	Logger l;

	l(Logger::start);
	commonScheme();
	l(Logger::end, "common scheme");

	l(Logger::start);
	branchEliminateScheme();
	l(Logger::end, "branch elimination scheme");

}

#pragma optimize("", off) 
void Executor::commonScheme()
{
	for (auto each : salary)
	{
		float taxableSalary = each - 3500.0f;
		if (less(taxableSalary, 0.0f))
			continue;
		else if (greaterEqual(taxableSalary, 0.0f) && lessEqual(taxableSalary, 1500.0f))
			cal0_1500(taxableSalary);
		else if (greater(taxableSalary, 1500.0f) && lessEqual(taxableSalary, 3000.0f))
			 cal1500_3000(taxableSalary);
		else if (greater(taxableSalary, 3000.0f) && lessEqual(taxableSalary, 9000.0f))
			 cal3000_9000(taxableSalary);
		else if (greater(taxableSalary, 9000.0f) && lessEqual(taxableSalary, 35500.0f))
			 cal9000_35500(taxableSalary);
		else if (greater(taxableSalary, 35500.0f) && lessEqual(taxableSalary, 55000.0f))
			 cal35500_55000(taxableSalary);
		else if (greater(taxableSalary, 55000.0f) && lessEqual(taxableSalary, 80000.0f))
			 cal55000_80000(taxableSalary);
		else if (greater(taxableSalary, 80000.0f))
			 calLargeThan80000(taxableSalary);
	}
}
#pragma optimize("", on) 

void Executor::branchEliminateScheme()
{
	for (auto each : salary)
	{
		salaryToTaxCal[size_t(each*0.002f)](each-3500.0f);
	}
}

float Executor::calNothing(float s)
{
	return 0.0f;
}

float Executor::cal0_1500(float s)
{
	return s * 0.03f;
}

float Executor::cal1500_3000(float s)
{
	return s * 0.1f - 105.0f;
}

float Executor::cal3000_9000(float s)
{
	return s * 0.2f - 555.0f;
}

float Executor::cal9000_35500(float s)
{
	return s * 0.25f - 1005.0f;
}

float Executor::cal35500_55000(float s)
{
	return s * 0.3f - 2755.0f;
}

float Executor::cal55000_80000(float s)
{
	return s * 0.35f - 5505.0f;
}

float Executor::calLargeThan80000(float s)
{
	return s * 0.45f - 13505.0f;
}

inline bool Executor::greater(float a, float b)const
{
	return (a > b + EPSINON) ? true : false;
}

inline bool Executor::greaterEqual(float a, float b)const
{
	return (a > b - EPSINON) ? true : false;
}

inline bool Executor::less(float a, float b)const
{
	return (a < b - EPSINON) ? true: false;
}

inline bool Executor::lessEqual(float a, float b)const
{
	return (a < b + EPSINON) ? true : false;
}

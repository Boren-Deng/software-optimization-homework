#pragma once
#ifndef EXECUTOR_H
#define EXECUTOR_H

#include<vector>
#include<functional>

class Executor
{
public:
	explicit Executor(const std::vector<float>&data);
	~Executor() = default;
	void execute();
private:
	const std::vector<float>& salary;
	std::vector<auto(*)(float)->float>salaryToTaxCal;//工资区间到税运算函数的映射
	constexpr static float EPSINON = 1e-8f;
	void commonScheme();
	void branchEliminateScheme();

	static float calNothing(float s);//便于原始工资为3500以下的区间映射
	static float cal0_1500(float s);
	static float cal1500_3000(float s);
	static float cal3000_9000(float s);
	static float cal9000_35500(float s);
	static float cal35500_55000(float s);
	static float cal55000_80000(float s);
	static float calLGreaterThan80000(float s);

	inline bool greater(float a, float b)const;
	inline bool greaterEqual(float a, float b)const;
	inline bool less(float a, float b)const;
	inline bool lessEqual(float a, float b)const;
};

#endif // !EXECUTOR_H



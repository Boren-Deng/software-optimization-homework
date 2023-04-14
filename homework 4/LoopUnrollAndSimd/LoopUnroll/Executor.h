#pragma once
#ifndef EXECUTOR_H
#define EXECUTOR_H

class Executor
{
public:
	explicit Executor(unsigned long N);
	~Executor() = default;
	void execute();
	
private:
	double commonScheme();
	double loopUnrollScheme();
	double loopUnroolAndSimdScheme();

	unsigned long N;
	constexpr static double HALF_PI= 1.5707963267948966192313216916;
	constexpr static double FACTORIAL_3 = -1.0/6, FACTORIAL_5 = 1.0/120, FACTORIAL_7 = -1.0/5040;
	double h;
};

#endif // !EXECUTOR_H

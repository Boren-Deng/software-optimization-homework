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

	unsigned long N;
	constexpr static double HALF_PI=1.57;
	constexpr static unsigned long FACTORIAL_3 = 6, FACTORIAL_5 = 120, FACTORIAL_7 = 5040;
	double h;
};

#endif // !EXECUTOR_H

#pragma once
#ifndef LOGGER_H

#include<string>

class Logger
{
public:
	Logger()=default;
	~Logger()=default ;

	enum logOption {
		start, end
	};

	void operator()(logOption option,const std::string& item="");
private:
	long long startTime = 0, endTime = 0;
	const static std::string LOG_PATH;
};
#endif // !LOGGER_H

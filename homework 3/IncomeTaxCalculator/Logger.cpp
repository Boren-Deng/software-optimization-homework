#include "Logger.h"


#include<chrono>
#include<fstream>
using namespace std;

const string Logger::LOG_PATH = "BranchEliminationConsumption.log";


void Logger::operator()(logOption option, const string& item)
{
	switch (option)
	{
	case Logger::start:
		startTime = chrono::time_point_cast<chrono::milliseconds>(chrono::system_clock::now()).time_since_epoch().count();
		break;
	case Logger::end:
	{
		endTime = chrono::time_point_cast<chrono::milliseconds>(chrono::system_clock::now()).time_since_epoch().count();
		fstream fs(LOG_PATH, ios::app);
		fs << item + " consumption:" << endTime - startTime << "ms\n";
		fs.close();
	}
		break;
	default:
		break;
	}
}

void Logger::operator()(const std::string& item)
{
	fstream fs(LOG_PATH, ios::app);
	fs << item << endl;
	fs.close();
}


#include "Logger.h"

#include<chrono>
#include<fstream>
using namespace std;

const string Logger::LOG_PATH = "mapConsumption.log";


void Logger::operator()( logOption option, const string& item)
{
	if (option == start)
		startTime = chrono::time_point_cast<chrono::milliseconds>(chrono::system_clock::now()).time_since_epoch().count();
	else
	{
		endTime = chrono::time_point_cast<chrono::milliseconds>(chrono::system_clock::now()).time_since_epoch().count();
		fstream fs(LOG_PATH, ios::app);
		fs << item + " consumption:" << endTime - startTime << "ms\n";
		fs.close();
	}
}

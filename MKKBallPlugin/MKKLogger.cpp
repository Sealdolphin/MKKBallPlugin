#include "MKKLogger.h"
#include <ctime>

/*
Statuikus változók
*/
bool MKKLogger::fileBusy = false;
int MKKLogger::lastID = BASIC_INFO;
std::string MKKLogger::logFilePath = "C:\\MKKLog\\MKKLog.txt";	//Default path

/*
A Naplóbejegyzés szintjének átírása a naplóba.
*/
std::string LevelToString(LogLevel level) {
	switch (level) {
	default:
	case LL_ERROR:
		return "ERROR";
	case WARNING:
		return "WARNING";
	case INFO:
		return "INFO";
	case UPDATE:
		return "UPDATE";
	}
}

/*
Új napló fájl létrehozása
*/
int MKKLogger::generateLogFile(std::string filePath, int intervals)
{
	
	interval = intervals;
	logFileStream.open(filePath, std::fstream::out | std::fstream::app);
	int open = logFileStream.is_open();
	if (open > 0) {
		logFilePath = filePath;
	} else {
		logFileStream.open(logFilePath, std::fstream::out | std::fstream::app);
	}
	
	logFileStream.close();
	return open;
	
}



/*
Új naplóbejegyzés írása
*/
void MKKLogger::createLog(LogLevel level, std::string message, int groupID)
{

	while (fileBusy) {
		//Do nothing
	}

	fileBusy = true;
	std::time_t t = time(0);
	std::tm now;
	localtime_s(&now, &t);



	if((groupID != lastID) && (abs(lastLog - t) < interval)) return;

	logFileStream.open(logFilePath, std::fstream::out | std::fstream::app);
	logFileStream << " [" << now.tm_hour << ":" << now.tm_min << ":" << now.tm_sec << "] " << "[" << LevelToString(level).c_str() << "] " << message.c_str() << std::endl;
	logFileStream.close();

	lastLog = t;
	lastID = groupID;
	fileBusy = false;
}

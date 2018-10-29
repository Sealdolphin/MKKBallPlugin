#include "MKKLogger.h"
#include <ctime>

/*
Statuikus változók
*/
int MKKLogger::repeatID = 0;
bool MKKLogger::repeatSecurity = false;
std::string MKKLogger::logFilePath = "MKKLog.txt";

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
void MKKLogger::generateLogFile(std::string filePath, int intervalms)
{
	interval = intervalms;
	logFilePath = filePath;
	logFileStream.open(logFilePath, std::fstream::out | std::fstream::app);
	logFileStream.close();
}



/*
Új naplóbejegyzés írása
*/
void MKKLogger::createLog(LogLevel level, std::string message)
{

	std::time_t t = time(0);
	std::tm now;
	localtime_s(&now, &t);

	if (repeatSecurity) repeatID++;

	if ((repeatID == lastID) && abs(lastLog - t) < interval) return;

	logFileStream.open(logFilePath, std::fstream::out | std::fstream::app);
	logFileStream << "[" << now.tm_hour << ":" << now.tm_min << ":" << now.tm_sec << "] " << "[" << LevelToString(level).c_str() << "] " << message.c_str() << std::endl;
	logFileStream.close();
	lastLog = t;
	lastID = repeatID;
}

void MKKLogger::setRepeatSecurity(bool security)
{
	repeatSecurity = security;
	if (repeatID > 1000) repeatID = 0;
}

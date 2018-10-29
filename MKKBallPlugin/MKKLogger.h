#pragma once
#include <iostream>
#include <fstream>

enum LogLevel {
	WARNING,
	INFO,
	UPDATE,
	LL_ERROR
};


class MKKLogger
{
public:
	void generateLogFile(std::string filePath, int intervalms = 1);
	void createLog(LogLevel level, std::string message);
	void setRepeatSecurity(bool security = true);

private:
	
	static int repeatID;
	static bool repeatSecurity;

	static std::string logFilePath;

	std::fstream logFileStream;
	int interval;
	time_t lastLog;
	int lastID;

};
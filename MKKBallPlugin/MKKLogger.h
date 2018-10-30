#pragma once
#include <iostream>
#include <fstream>

enum LogLevel {
	WARNING,
	INFO,
	UPDATE,
	LL_ERROR
};

enum G_ID {
	BASIC_INFO,
	TRACK_INFO

};

class MKKLogger
{
public:
	void generateLogFile(std::string filePath, int intervals = 1);
	void createLog(LogLevel level, std::string message, int groupID = BASIC_INFO);

private:
	
	static std::string logFilePath;

	std::fstream logFileStream;
	int interval;
	time_t lastLog;
	static int lastID;

};
#pragma once
#include <iostream>
#include <fstream>

/*
Enumeráció a különbözõ naplóbejegyzések szintjeihez
WARNING: figyelmen kívül hagyható (veszélyes) hiba
INFO: alapvetõ információ
UPDATE: státuszváltozás, metainformáció
LL_ERROR: végzetes hiba
*/
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

/*
A Naplózó osztály felépítése
Ez az osztály képes naplózni fáljba a pluginnal kapcsolatos alapvetõ információkat
Jelenleg naplózza:
	A plugin belsõ mûködését, és a státuszok változását
	A lejátszott zeneszámokat egy külön fájlba
*/
class MKKLogger
{
public:
	int generateLogFile(std::string filePath, int intervals = 1);
	void createLog(LogLevel level, std::string message, int groupID = BASIC_INFO);

private:
	
	static std::string logFilePath;
	static bool fileBusy;

	std::fstream logFileStream;
	int interval;
	time_t lastLog;
	static int lastID;

};
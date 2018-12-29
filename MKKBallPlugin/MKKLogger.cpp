#include "MKKLogger.h"
#include <ctime>
/*
Ez az MKK pluginhoz írt naplózó osztály.
*/

/*
Statuikus változók beállítása
*/
bool MKKLogger::fileBusy = false;								//Fálj írása folyamatban van-e? (szinkronizálás)
int MKKLogger::lastID = BASIC_INFO;								//Utolsó bejegyzés típusa
std::string MKKLogger::logFilePath = "C:\\MKKLog\\MKKLog.txt";	//Alapméretezett elérési útvonal (ha a beállítás nem működne)

/*
A Naplóbejegyzés szintjének átírása a naplóba.
Ez a függvény a megfelelő szintből egy hasonló szöveget készít
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
A fájlt az alapvető elérési útvonalon (filePath) hozzuk létre előszőr
Ha ez az útvonal nem létezik, vagy nem elérhető, akkor az alapméretezett útvonalat használjuk (static)
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
Szükséges a bejegyzés szintje / típusa, valamint az üzenet
*/
void MKKLogger::createLog(LogLevel level, std::string message, int groupID)
{

	while (fileBusy) {
		//A fájlt egy másik utasítás írja, ezért várakozunk
	}

	std::time_t t = time(0);	//Időbélyeg készítése (aktuális pillanat)
	std::tm now;				//Dátum osztály deklarálása
	localtime_s(&now, &t);		//Az időbélyeg másolása az dátum osztályba

	/*
	Csak akkor írunk, ha az üzenet különböző információt hordoz, mint az előző
	és letelt a beállított időintervallum (alapméretezett: 1mp)
	*/
	if((groupID != lastID) && (abs(lastLog - t) < interval)) return;

	//Fájl megnyitása
	fileBusy = true;
	logFileStream.open(logFilePath, std::fstream::out | std::fstream::app);
	logFileStream << " [" << now.tm_hour << ":" << now.tm_min << ":" << now.tm_sec << "] " << "[" << LevelToString(level).c_str() << "] " << message.c_str() << std::endl;
	logFileStream.close();	//Memória felszabadítása

	//Előző bejegyzés felülírása a mostanival
	lastLog = t;
	lastID = groupID;
	//Erőforrás felszabadítása
	fileBusy = false;
}

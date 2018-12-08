#ifndef MYPLUGIN8_H
#define MYPLUGIN8_H

// we include stdio.h only to use the sprintf() function
// we define _CRT_SECURE_NO_WARNINGS for the warnings of the sprintf() function
#define _CRT_SECURE_NO_WARNINGS

#include "vdjDsp8.h"
#include "MKKLogger.h"
#include "json.hpp"

#include "timer.h"

using json = nlohmann::json;


///Ez a plugint megvalósító osztály
class MKKBallMaker : public IVdjPluginDsp8
{
public:

	HRESULT VDJ_API OnLoad();
	HRESULT VDJ_API OnGetPluginInfo(TVdjPluginInfo8 *infos);
	ULONG VDJ_API Release();
	HRESULT VDJ_API OnStart();
	HRESULT VDJ_API OnStop();
	HRESULT VDJ_API OnProcessSamples(float *buffer, int nb);

	HRESULT VDJ_API OnParameter(int id);
	HRESULT VDJ_API OnGetParameterString(int id, char *outParam, int outParamSize);

private:

	//A naplózó egység
	MKKLogger logger;

	//Az időzítő (overflow ellen)
	Timer timer;

	//Gombok
	char ip_address[64];
	char con_port[64];
	char message_of_the_dj[64];
	char connection_status[64];
	bool ladies_choice;
	bool is_connected;

	//Gomb Státuszok
	int btnIPStatus;
	int btnPortStatus;
	int conSwitch_status;

	//Enum a gombok detektálásához
protected:
	typedef enum _ID_Interface {
		SWITCH_CONNECT,
		BTN_SETADDR,
		BTN_SETPORT,
		BTN_SETMSG,
		BTN_LADIES,
		LABEL

	} ID_Interface;

};


///Ez egy zenét megvalósító osztály
class MKKTrack {

public:
	MKKTrack(MKKBallMaker *parent);

	std::string getTitle() { return s_title; }
	std::string getArtist() { return s_artist; }
	std::string getGenre() { return s_genre; }
	std::string getNextGenre() { return s_next_genre; }

	std::string createJSON();

private:
	static const int max_length = 64;
	json TrackData;

	double time_rem_min;
	double time_rem_sec;
	std::string s_title;
	std::string s_artist;
	std::string s_genre;
	std::string s_next_genre;

};

#endif
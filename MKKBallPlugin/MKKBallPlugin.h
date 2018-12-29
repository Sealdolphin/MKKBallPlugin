#ifndef MYPLUGIN8_H
#define MYPLUGIN8_H
/*
Ez az MKK Báli vetítőrendszerhez írt plugin VirtualDJ 8-hoz
A plugin segítségével helyi hálózaton elküldhetők a számok metaadatai
A plugin elküldi a szervernek:
	Az éppen most játszott szám előadóját (artist)
	Az éppen most játszott szám címét (title)
	Az éppen most játszott szám stílusát / táncát (genre)
	Az éppen most játszott számból hátralévő időt (sec_left és min_left)
	A következő szám stílusát / táncát (next_song)
A metaadatok maximális hossza 64 karakter

Verziószám: 2.1
*/
#define PLUGIN_VER "2.1"	//A plugin verziószáma

// we include stdio.h only to use the sprintf() function
// we define _CRT_SECURE_NO_WARNINGS for the warnings of the sprintf() function
#define _CRT_SECURE_NO_WARNINGS

#include "vdjDsp8.h"
#include "MKKLogger.h"
#include "json.hpp"

#include "timer.h"

using json = nlohmann::json;


/*
Ez a plugint megvalósító osztály
Az osztály megvalósítja a VirtualDJ-hez szükséges interface-t
*/
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

	MKKLogger logger;				//A naplózó egység
	Timer timer;					//Az időzítő (overflow ellen kell, mert a nagyobb terhelést nehezen viseli a szerver)

	//Gombok
	char ip_address[64];			//beállított IP cím
	char con_port[64];				//beállított Port
	char message_of_the_dj[64];		//Dj üzenete (nem használt)
	char connection_status[64];		//Kapcsolat a szerverrel (felirat)
	bool ladies_choice;				//Hölgyválasz állapota
	bool is_connected;				//Kapcsolat állapota

	//Gomb Státuszok
	int btnIPStatus;				//IP beállítás gomb lenyomva
	int btnPortStatus;				//Port beállítás gomb lenyomva
	int conSwitch_status;			//Kapcsolat váltás gomb lenyomva

	//Enum a gombok típusaihoz
protected:
	typedef enum _ID_Interface {
		SWITCH_CONNECT,				//Csatlakozás / Szétkapcsolás (kapcsoló)
		BTN_SETADDR,				//IP Cím beállítása
		BTN_SETPORT,				//Port beállítása
		BTN_SETMSG,					//DJ üzenete beállítás
		BTN_LADIES,					//Hölgyválasz (kapcsoló)
		LABEL						//Felirat

	} ID_Interface;

};


/*
A zene adatait magában foglaló gyüjtőosztály
Tartalmazza a zene azon metaadatait, amit tovább küldünk a szervernek
*/
class MKKTrack {

public:
	//Konstruktor
	MKKTrack(MKKBallMaker *parent);

	//Getterek
	std::string getTitle() { return s_title; }
	std::string getArtist() { return s_artist; }
	std::string getGenre() { return s_genre; }
	std::string getNextGenre() { return s_next_genre; }

	//JSON-ba alakítás
	std::string createJSON();

private:
	static const int max_length = 64;	//Szöveg maximum hossza (metaadatokra vonatkozik)
	json TrackData;						//A JSON struktúra

	//Metaadatok
	double time_rem_min;				//Hátralévő idő (percek)
	double time_rem_sec;				//Hátralévő idő (másodpercek)
	std::string s_title;				//Szám címe
	std::string s_artist;				//Előadó
	std::string s_genre;				//Szám stílusa (milyen tánc)
	std::string s_next_genre;			//Következő szám stílusa (milyen tánc)

};

#endif
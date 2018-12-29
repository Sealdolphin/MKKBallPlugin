#include "MKKBallPlugin.h"
#include "MKKSocket.h"

#include "timer.h"

#include <comdef.h>

#if (defined(VDJ_WIN))
#ifndef SAFE_RELEASE
#define SAFE_RELEASE(p) { if (p) { (p)->Release(); (p)=NULL; } }
#endif
#endif

Network network;			//Kapcsolat a szerverrel
int Network::net_id = 0;	//A kapcsolat egyedi azonosítója

/*
A plugin betöltésekor lefutó kód
A VirtualDJ elindulása után betölti az összes plugint
Ekkor inicializáljuk a változókat, és készítjük el a szükséges gombokat,
valamint ekkor töltjük be a beállításokat
Itt hozzuk létre a naplófájlt is

Ez a kód a VirtualDJ megnyitása során egyszer fut le!
*/
//-----------------------------------------------------------------------------
HRESULT VDJ_API MKKBallMaker::OnLoad()
{
	timer = Timer(1);									//Időzítő létrehozása (1 mp késleltetéssel)
	char folder[64];									//Elérési útvonal
	GetStringInfo("get_vdj_folder", folder, 64);		//A VirtualDJ munkamappájának lekérése (working directory)
	strcat(folder, "\\Plugins\\MKKData\\MKKLog.txt");	//A naplófájl elérési útvonalának elkészítése

	std::string workingDir = folder;					//A naplófájl elérési útvonala

	int opened = logger.generateLogFile(folder);		//A naplófájl létrehozása az adott mappában, majd a függvény naplózása
	logger.createLog(UPDATE, "OnLoad: A Plugin betöltése megtörtént.");

	network.init();										//Hálózati kapcsolat elindítása
	logger.createLog(UPDATE, "OnLoad: A hálózati kapcsolat sikeresen felállt");

	//Paraméterek betöltése (gombok)
	DeclareParameterSwitch(&conSwitch_status, SWITCH_CONNECT, "Vetítés", "C", false);
	DeclareParameterButton(&btnIPStatus, BTN_SETADDR, "IP beáll", "IP");
	DeclareParameterButton(&btnPortStatus, BTN_SETPORT, "Port beáll", "P");
	//Gombokhoz tartozó feliratok
	DeclareParameterString(connection_status, LABEL, "Kapcsolat", "CS", 32);
	DeclareParameterString(ip_address, LABEL, "IP cím", "IP", 32);
	DeclareParameterString(con_port, LABEL, "Port", "P", 16);
	//DeclareParameterButton(&is_connected, BTN_SETMSG, "A DJ üzenete", "M");

	//Paraméterek inicializálása
	is_connected = false;								//Kapcsolat a szerverrel (Offline)
	conSwitch_status = 0;								//A Kapcsolat gomb állása (kikapcsolva)
	strcpy(connection_status, "Offline");				//Kapcsolat felirata
	strcpy(ip_address, "localhost");					//A vetítő szerver IP címe
	strcpy(con_port, "5503");							//A kommunikációs port

	return S_OK;
}
//-----------------------------------------------------------------------------
HRESULT VDJ_API OnDeviceInit() {
	return S_OK;
}
//-----------------------------------------------------------------------------
HRESULT VDJ_API OnDeviceClose() {
	return S_OK;
}
/*
A VirtualDJ-be épített Plugin metaadatai
Ez jelenik meg, amikor a felhasználó lekéri a plugin információit
*/
//-----------------------------------------------------------------------------
HRESULT VDJ_API MKKBallMaker::OnGetPluginInfo(TVdjPluginInfo8 *infos)
{
	infos->PluginName = "MKK Vetítő rendszer";								//A plugin neve
	infos->Author = "Mihalovits Márk";										//A plugin készítője
	infos->Description = "VirtualDJ aktuális számairól küld információt";	//A plugin leírása
	infos->Version = PLUGIN_VER;											//A plugin verziószáma
	infos->Flags = 0x00;													//Jelzőbitek (a különböző pluginok feldolgozási sorrendjéhez)
	infos->Bitmap = NULL;													//A pluginhoz tartozó kép bitmapja
	
	return S_OK;
}
/*
A plugin feloldásakor lefutó kód.
A VirtualDJ a (normális) kikapcsoláskor feloldja a pluginoknak lefoglalt memóriát
Itt állítjuk le a szerver kapcsolatot, és szabadítjuk fel az általunk pluszba lefoglalt memóriát

Ez a kódrészlet a VirtualDJ rendes bezárása során egyszer fut le!
*/
//---------------------------------------------------------------------------
ULONG VDJ_API MKKBallMaker::Release()
{
	//Kikapcsolás naplózása
	logger.createLog(UPDATE, "OnRelease: A Plugin feloldása megtörtént.");
	//Szerver kapcsolat leállítása
	network.shutdown_server();
	//Nem tudom mi ez, de kell NE TÖRÖLD KI
	delete this;				
	return 0;
}
//---------------------------------------------------------------------------
HRESULT VDJ_API MKKBallMaker::OnStart()
{
	return S_OK;
}
//---------------------------------------------------------------------------
HRESULT VDJ_API MKKBallMaker::OnStop()
{
	return S_OK;
}
/*
Egyes audiofájlok hangmintavételezésekor lefutó kód

FIGYELEM!
Ez a kód meghatározott időegységenként mintát vételez a lejátszott audióból másodpercenként többször is!
A kódsorozat MINDEN egyes mintavételezésnél lefut!
*/
//---------------------------------------------------------------------------
HRESULT VDJ_API MKKBallMaker::OnProcessSamples(float * buffer, int nb)
{
	//Időzítő lekérdezése, csak másodpercenkénti futás érdekében
	if (timer.is_alarmed()) {
		MKKTrack nowPlaying = MKKTrack(this);				//Most játszott zeneszám lekérése
		logger.createLog(INFO, "Sample processed");			//Mintavételezés naplózása
		
		if (is_connected == TRUE) {
			network.send_message(nowPlaying.createJSON());	//Metaadatok küldése, ha a kapcsolat fenáll
		}
		timer.start();										//Időzítő újraindítása
	}
	return S_OK;
}
/*
A pluginhoz tartozó saját paraméterek és beállítások kalibrálása
A kód minden alkalommal lefut, ha a felhasználó a VirtualDJ-ben a plugin egy paraméterével interakcióba lép (gombnyomás)
*/
//---------------------------------------------------------------------------
HRESULT VDJ_API MKKBallMaker::OnParameter(int id) {

	HRESULT hr;				//Eredményjelző

	//Ellenőrizzük a hívott paraméter adatait
	switch (id) {

		//Kapcsolat beállítása, vagy leállítása gomb
	case SWITCH_CONNECT:

		//Kapcsolat leállítása
		if (is_connected == true) {
			network.shutdown_server();
			network.init();
			is_connected = false;
			strcpy(connection_status,"Offline");
		}
		//Kapcsolódás
		else 
		{
			switch (network.resolve(ip_address, con_port)) {

			default:
			case ERR_NO_DOMAIN:
				strcpy(connection_status, "Nincs Cím");
				is_connected = false;
				break;

			case ERR_GETADDRINFO_FAIL:
				strcpy(connection_status, "Rossz Cím");
				is_connected = false;
				break;

			case ERR_NOERR:
				network.connect_server() == ERR_NOERR ? strcpy(connection_status, "Online") : strcpy(connection_status, "Hibás");
				is_connected = true;
				break;
			}
		}

		//IP cím beállítása gomb
	case BTN_SETADDR:
		DLGPROC dp;

		hr = DialogBox(NULL, "Add meg az IP címet!", NULL, dp);
		break;

		//Port beállítása gomb
	case BTN_SETPORT:

		break;

		//DJ üzenete gomb
	case BTN_SETMSG:
		//TODO
		break;

		//Hölgyválasz ki-be kapcsolása
	case BTN_LADIES:
		
		break;
	default:
		break;
	}

	//Refresh strings
	logger.createLog(INFO, "HR = " + hr);

	return S_OK;
}
//---------------------------------------------------------------------------
HRESULT VDJ_API MKKBallMaker::OnGetParameterString(int id, char *outParam, int outParamSize) {
	switch (id)
	{
	default:
	case SWITCH_CONNECT:
		sprintf(outParam, "Yeah!");
		break;
	}

	return S_OK;

}



/*
Zene osztály létrehozása
*/
MKKTrack::MKKTrack(MKKBallMaker *parent)
{
	//Memória egységek lefoglalása
	char* title = new char[max_length];
	char* artist = new char[max_length];
	char* genre = new char[max_length];
	char* next_genre = new char[max_length];

	//VDj Script segítségével a metaadatok lekérése és átírása a lefoglalt memóriába
	parent->GetStringInfo("deck active get_loaded_song 'title'", title, max_length);
	parent->GetStringInfo("deck active get_loaded_song 'artist'", artist, max_length);
	parent->GetStringInfo("deck active get_loaded_song 'genre'", genre, max_length);
	parent->GetStringInfo("get_automix_song 'genre'", next_genre, max_length);
	parent->GetInfo("get_time_min 'remain'", &time_rem_min);
	parent->GetInfo("get_time_sec 'remain'", &time_rem_sec);

	//A karaktertömbök átírása a stringekbe
	s_title = title;
	s_artist = artist;
	s_genre = genre;
	s_next_genre = next_genre;

	//A felesleges karaktertömbök felszabadítása
	free(title);
	free(artist);
	free(genre);
	free(next_genre);

}

/*
A zene metaadatait egy JSON objektumba írja
*/
std::string MKKTrack::createJSON()
{
	TrackData["title"] = s_title;
	TrackData["artist"] = s_artist;
	TrackData["genre"] = s_genre;
	TrackData["next_song"] = s_next_genre;
	TrackData["min_left"] = time_rem_min;
	TrackData["sec_left"] = time_rem_sec;

	return TrackData.dump();
}
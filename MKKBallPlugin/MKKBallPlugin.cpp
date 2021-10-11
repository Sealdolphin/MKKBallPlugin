#include "MKKBallPlugin.h"
#include "MKKSocket.h"

#include "timer.h"

#include <comdef.h>
#include <commdlg.h>

#if (defined(VDJ_WIN))
#ifndef SAFE_RELEASE
#define SAFE_RELEASE(p) { if (p) { (p)->Release(); (p)=NULL; } }
#endif
#endif

#define WM_INITDIALOG                   0x0110

Network network;			//Kapcsolat a szerverrel
int Network::net_id = 0;	//A kapcsolat egyedi azonosítója
char szItemName[80];		//Dialógus bemeneti szövege

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
	SendCommand("deck right masterdeck");					//2. számú deck beállítása masternek

	timer = Timer(1);									//Időzítő létrehozása (1 mp késleltetéssel)
	char folder[128];									//Elérési útvonal
	GetStringInfo("get_vdj_folder", folder, 128);		//A VirtualDJ munkamappájának lekérése (working directory)
	strcat(folder, "\\Plugins64\\MKKData\\MKKLog.txt");	//A naplófájl elérési útvonalának elkészítése

	std::string workingDir = folder;					//A naplófájl elérési útvonala

	int opened = logger.generateLogFile(folder);		//A naplófájl létrehozása az adott mappában, majd a függvény naplózása
	logger.createLog(LogLevel::UPDATE, "OnLoad: A Plugin betöltése megtörtént.");

	network.init();										//Hálózati kapcsolat elindítása
	logger.createLog(LogLevel::UPDATE, "OnLoad: A hálózati kapcsolat sikeresen felállt");

	//Paraméterek betöltése (gombok)
	DeclareParameterSwitch(&conSwitch_status, SWITCH_CONNECT, "Vetítés", "C", false);
	DeclareParameterButton(&btnIPStatus, BTN_SETADDR, "IP beáll", "IP");
	DeclareParameterButton(&btnPortStatus, BTN_SETPORT, "Port beáll", "P");
	DeclareParameterSwitch(&ladiesSwitch_status, SWITCH_LADIES, "Hölgyválasz", "H", false);
	//Gombokhoz tartozó feliratok
	DeclareParameterString(connection_status, LABEL, "Kapcsolat", "CS", 32);
	DeclareParameterString(ip_address, LABEL, "IP cím", "IP", 32);
	DeclareParameterString(con_port, LABEL, "Port", "P", 16);

	DeclareParameterSlider(&headphone_volume, SLIDER_HEADPHONE, "Süketség", "Füles", 0.5f);
	char headphoneCommand[32] = "";
	sprintf(headphoneCommand, "headphone_volume %f", headphone_volume);
	SendCommand(headphoneCommand);

	//Paraméterek inicializálása
	is_connected = false;								//Kapcsolat a szerverrel (Offline)
	ladies_choice = false;								//Hölgyválasz
	conSwitch_status = 0;								//A Kapcsolat gomb állása (kikapcsolva)
	ladiesSwitch_status = 0;							//A Hölgyválasz gomb állása (kikapcsolva)
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
	logger.createLog(LogLevel::UPDATE, "OnRelease: A Plugin feloldása megtörtént.");
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
	//METAADATOK KÜLDÉSE
	//Időzítő lekérdezése, csak másodpercenkénti futás érdekében
	if (timer.is_alarmed()) {
		try
		{
			MKKTrack nowPlaying = MKKTrack(this);				//Most játszott zeneszám lekérése
				//logger.createLog(INFO, "Sample processed");			//Mintavételezés naplózása

			if (is_connected == TRUE) {
				logger.createLog(LogLevel::INFO, nowPlaying.createJSON());					//Mintavételezés naplózása
				int result = network.send_message(nowPlaying.createJSON());			//Metaadatok küldése, ha a kapcsolat fenáll

				if (result == ERR_SEND_FAIL) {
					strcpy(connection_status, "Megszakadt");
					logger.createLog(LogLevel::ERR, "Kapcsolat megszakadt...");
				}

				if (result == ERR_NULLMSG) {
					logger.createLog(LogLevel::WARNING, "Üres üzenet küldése");
				}
			}
		}
		catch (const std::exception& exception)
		{
			std::string message = "Exception occured during send!: ";
			message.append(exception.what());
			logger.createLog(LogLevel::ERR, message);
			strcpy(connection_status, "Nem várt hiba");
			is_connected = false;
		}
	}
	timer.start();										//Időzítő újraindítása
	return S_OK;
}
/*
A pluginhoz tartozó saját paraméterek és beállítások kalibrálása
A kód minden alkalommal lefut, ha a felhasználó a VirtualDJ-ben a plugin egy paraméterével interakcióba lép (gombnyomás)
*/
//---------------------------------------------------------------------------
HRESULT VDJ_API MKKBallMaker::OnParameter(int id) {

	HRESULT hr;				//Eredményjelző
	char headphoneCommand[32] = "";
	char loggerBuffer[1024] = "";

	//Ellenőrizzük a hívott paraméter adatait
	switch (id) {
	default:
		break;
		//Kapcsolat beállítása, vagy leállítása gomb
	case SWITCH_CONNECT:

		//Kapcsolat leállítása
		if (is_connected == true) {
			logger.createLog(LogLevel::INFO, "Kapcsolat bontása...");
			network.shutdown_server();
			network.init();
			is_connected = false;
			strcpy(connection_status,"Offline");
		}
		//Kapcsolódás
		else 
		{
			sprintf(loggerBuffer, "Kapcsolódás: %s:%s", ip_address, con_port);
			logger.createLog(LogLevel::INFO, loggerBuffer);
			logger.createLog(LogLevel::INFO, "Cím feloldása...");
			switch (network.resolve(ip_address, con_port)) {

			default:
			case ERR_NO_DOMAIN:
				strcpy(connection_status, "Nincs Cím");
				logger.createLog(LogLevel::ERR, "Domain nem található");
				is_connected = false;
				break;

			case ERR_GETADDRINFO_FAIL:
				strcpy(connection_status, "Rossz Cím");
				logger.createLog(LogLevel::ERR, "Cím feloldása nem sikerült");
				is_connected = false;
				break;

			case ERR_NOERR:
				int connect_status = network.connect_server();
				switch (connect_status)
				{
				default:
				case ERR_CONNECT_FAIL:
					strcpy(connection_status, "Sikertelen");
					logger.createLog(LogLevel::INFO, "Sikertelen csatlakozás");
					break;
				case ERR_INVALID_SOCK:
					strcpy(connection_status, "Hibás");
					logger.createLog(LogLevel::INFO, "Kapcsolat megszakadt");
					break;
				case ERR_NOERR:
					strcpy(connection_status, "Online");
					logger.createLog(LogLevel::INFO, "Sikeres csatlakozás");
					if (network.recv_message() == ERR_NOERR) {
						logger.createLog(LogLevel::INFO, network.get_server_input());
					}
					break;
				}
				is_connected = true;
				break;
			}
		}
		break;
		//Hölgyválasz ki-be kapcsolása
	case SWITCH_LADIES:
		ladies_choice = !ladies_choice;
		sprintf(loggerBuffer, "Hölválasz státusz = %d", ladies_choice);
		logger.createLog(LogLevel::INFO, loggerBuffer);
		break;

	case SLIDER_HEADPHONE:
		sprintf(headphoneCommand, "headphone_volume %f", headphone_volume);
		hr = SendCommand(headphoneCommand);
		//logger.createLog(INFO, headphoneCommand);
		break;
	case BTN_SETPORT:
		if (DialogBox(hInstance,
			MAKEINTRESOURCE(WM_INITDIALOG),
			pluginWindow,
			(DLGPROC) MKKBallMaker::DeleteItemProc) == IDOK) {
			// Complete the command; szItemName contains the dialog answer
		}
		else
		{
			// Cancel the command. 
		}
		break;
	}

	return S_OK;
}
//---------------------------------------------------------------------------
HRESULT VDJ_API MKKBallMaker::OnGetParameterString(int id, char *outParam, int outParamSize) {

	switch (id)
	{
	case SLIDER_HEADPHONE:
		sprintf(outParam, "%.0f%%", headphone_volume * 100);
		break;
	}
	return S_OK;
}
//---------------------------------------------------------------------------
HRESULT VDJ_API MKKBallMaker::OnGetUserInterface(TVdjPluginInterface8* pluginInterface) {
	
	/*
	pluginInterface->hWnd = CreateWindow(
		"PluginWindow",
		"My MKK PLugin",
		WS_VISIBLE | WS_CHILD,
		0, 0, 100, 500,
		NULL,
		NULL,
		hInstance,
		NULL
	);
	pluginWindow = pluginInterface->hWnd;
	*/
	
	return S_OK;
}


BOOL CALLBACK MKKBallMaker::DeleteItemProc(HWND hwndDlg,
	UINT message,
	WPARAM wParam,
	LPARAM lParam)
{
	/*
	switch (message)
	{
	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case IDOK:
			if (!GetDlgItemText(hwndDlg, 0, szItemName, 80))
				*szItemName = 0;

			// Fall through. 

		case IDCANCEL:
			EndDialog(hwndDlg, wParam);
			return TRUE;
		}
	}
	*/
	return FALSE;
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
	parent->GetStringInfo("deck right get_loaded_song 'title'", title, max_length);
	parent->GetStringInfo("deck right get_loaded_song 'artist'", artist, max_length);
	parent->GetStringInfo("deck right get_loaded_song 'genre'", genre, max_length);
	parent->GetStringInfo("get_automix_song 'genre'", next_genre, max_length);
	parent->GetInfo("get_time_min 'remain'", &time_rem_min);
	parent->GetInfo("get_time_sec 'remain'", &time_rem_sec);

	//A karaktertömbök átírása a stringekbe
	s_title = title;
	s_artist = artist;
	s_genre = genre;
	s_next_genre = next_genre;
	ladies = parent->GetLadies();

	//A felesleges karaktertömbök felszabadítása
	delete[](title);
	delete[](artist);
	delete[](genre);
	delete[](next_genre);

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
	TrackData["ladies"] = ladies;

	std::string jsonData = TrackData.dump();
	jsonData.append("\n");
	return jsonData;
}
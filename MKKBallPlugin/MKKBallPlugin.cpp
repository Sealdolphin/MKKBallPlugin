#include "MKKBallPlugin.h"
#include "MKKSocket.h"

#include "timer.h"

#include <comdef.h>

#if (defined(VDJ_WIN))
#ifndef SAFE_RELEASE
#define SAFE_RELEASE(p) { if (p) { (p)->Release(); (p)=NULL; } }
#endif
#endif

Network network;
int Network::net_id = 0;

//-----------------------------------------------------------------------------
HRESULT VDJ_API MKKBallMaker::OnLoad()
{
	// ADD YOUR CODE HERE WHEN THE PLUGIN IS CALLED
	//Felkészülünk a plugin betöltésére.
	//Naplófájl létrehozása
	timer = Timer(1);
	char folder[64];
	GetStringInfo("get_vdj_folder", folder, 64);

	strcat(folder, "\\Plugins\\MKKData\\MKKLog.txt");

	std::string workingDir = folder;

	int opened = logger.generateLogFile(folder);
	logger.createLog(UPDATE, "OnLoad: A Plugin betöltése megtörtént.");

	network.init();

	//Paraméterek inicializálása
	is_connected = false;
	strcpy(connection_status, "Offline");
	strcpy(ip_address, "localhost");
	strcpy(con_port, "5503");

	//Paraméterek betöltése (gombok)
	DeclareParameterSwitch(&conSwitch_status, SWITCH_CONNECT, "Vetítés", "C", false);
	DeclareParameterButton(&btnIPStatus, BTN_SETADDR, "IP address", "IP");
	DeclareParameterButton(&btnPortStatus, BTN_SETPORT, "Port", "P");

	DeclareParameterString(connection_status, LABEL, "Kapcsolat", "CS", 32);
	DeclareParameterString(ip_address, LABEL, "IP cím", "IP", 32);
	DeclareParameterString(con_port, LABEL, "Port", "P", 16);
	//DeclareParameterButton(&is_connected, BTN_SETMSG, "A DJ üzenete", "M");

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
//-----------------------------------------------------------------------------
HRESULT VDJ_API MKKBallMaker::OnGetPluginInfo(TVdjPluginInfo8 *infos)
{
	infos->PluginName = "MKK Vetítő rendszer";
	infos->Author = "Mihalovits Márk";
	infos->Description = "VirtualDJ aktuális számairól küld információt";
	infos->Version = "2.0";
	infos->Flags = 0x00; // VDJFLAG_PROCESSLAST if you want to ensure that all other effects are processed first
	infos->Bitmap = NULL;
	

	return S_OK;
}
//---------------------------------------------------------------------------
ULONG VDJ_API MKKBallMaker::Release()
{
	// ADD YOUR CODE HERE WHEN THE PLUGIN IS RELEASED
	logger.createLog(UPDATE, "OnRelease: A Plugin feloldása megtörtént.");

	network.shutdown_server();

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
//---------------------------------------------------------------------------
HRESULT VDJ_API MKKBallMaker::OnProcessSamples(float * buffer, int nb)
{
	if (timer.is_alarmed()) {

		MKKTrack nowPlaying = MKKTrack(this);

		logger.createLog(INFO, "Loaded song!");

		if (is_connected == TRUE) {

			network.send_message(nowPlaying.createJSON());
		}
		timer.start();
	}
	return S_OK;
}
//---------------------------------------------------------------------------
HRESULT VDJ_API MKKBallMaker::OnParameter(int id) {
	//Gombok működése

	HRESULT hr;

	switch (id) {

	case SWITCH_CONNECT:

		if (is_connected == true) {
			network.shutdown_server();
			network.init();
			is_connected = false;
			strcpy(connection_status,"Offline");
		}
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

	case BTN_SETADDR:
		DLGPROC dp;

		hr = DialogBox(NULL, "Add meg az IP címet!", NULL, dp);
		break;

	case BTN_SETPORT:

		break;


	case BTN_SETMSG:
		//DJ üzenet
		break;

	case BTN_LADIES:
		//Hölgyválasz
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




MKKTrack::MKKTrack(MKKBallMaker *parent)
{
	char* title = new char[max_length];
	char* artist = new char[max_length];
	char* genre = new char[max_length];
	char* next_genre = new char[max_length];

	parent->GetStringInfo("deck active get_loaded_song 'title'", title, max_length);
	parent->GetStringInfo("deck active get_loaded_song 'artist'", artist, max_length);
	parent->GetStringInfo("deck active get_loaded_song 'genre'", genre, max_length);
	parent->GetStringInfo("get_automix_song 'genre'", next_genre, max_length);
	parent->GetInfo("get_time_min 'remain'", &time_rem_min);
	parent->GetInfo("get_time_sec 'remain'", &time_rem_sec);

	s_title = title;
	s_artist = artist;
	s_genre = genre;
	s_next_genre = next_genre;

	free(title);
	free(artist);
	free(genre);
	free(next_genre);

}

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
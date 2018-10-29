#include "MKKBallPlugin.h"
#include "MKKSocket.h"

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
	logger.generateLogFile("C:\\Users\\Márk\\Documents\\VirtualDJ\\Plugins\\VideoEffect\\MKKLog.txt");
	logger.createLog(UPDATE, "OnLoad: A Plugin betöltése megtörtént.");

	logger.createLog(UPDATE, "Establishing server connection...");
	network.init();
	network.resolve("localhost","5503");
	int connection = network.connect_server();
	

	VideoWidth = 0;
	VideoHeight = 0;

#if (defined(VDJ_WIN))

	D3DDevice = NULL;
	D3DTexture = NULL;
	D3DSurface = NULL;

#elif (defined(VDJ_MAC))

	glContext = 0;
	GLTexture = 0;

#endif

	return S_OK;
}
//-----------------------------------------------------------------------------
HRESULT VDJ_API MKKBallMaker::OnGetPluginInfo(TVdjPluginInfo8 *infos)
{
	infos->PluginName = "MKKTrackInfoSender";
	infos->Author = "Mihalovits Márk";
	infos->Description = "VirtualDJ aktuális számairól küld információt";
	infos->Version = "1.5";
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
HRESULT VDJ_API MKKBallMaker::OnDeviceInit()
{
	// ADD YOUR CODE HERE
	logger.createLog(UPDATE, "OnDeviceInit: A Plugint kezelő ablak felépült.");
	HRESULT hr;

#if (defined(VDJ_WIN))

	hr = GetDevice(VdjVideoEngineDirectX9, (void**)&D3DDevice);
	if (hr != S_OK || D3DDevice == NULL) return S_FALSE;

#elif (defined(VDJ_MAC))

	glContext = CGLGetCurrentContext();

#endif

	// Size of the Video Window
	VideoWidth = width;
	VideoHeight = height;

	return S_OK;
}
//---------------------------------------------------------------------------
HRESULT VDJ_API MKKBallMaker::OnDeviceClose()
{
	// ADD YOUR CODE HERE
	logger.createLog(UPDATE, "OnDeviceClose: A Plugint kezelő ablak bezárult.");
	return S_OK;
}
//---------------------------------------------------------------------------
HRESULT VDJ_API MKKBallMaker::OnStart()
{
	// ADD YOUR CODE HERE
	logger.setRepeatSecurity();
	logger.createLog(UPDATE, "OnStart: A Plugin elindult.");

	MKKTrack nowPlaying = MKKTrack(this);

	logger.createLog(INFO, "Loaded song!");
	logger.createLog(INFO, "Artist: " + nowPlaying.getArtist());
	logger.createLog(INFO, "Title: " + nowPlaying.getTitle());
	logger.createLog(INFO, "Genre: " + nowPlaying.getGenre());
	logger.createLog(INFO, "Next Song: " + nowPlaying.getNextGenre());

	logger.setRepeatSecurity(false);

	network.send_message(nowPlaying.createJSON());


	return S_OK;
}
//---------------------------------------------------------------------------
HRESULT VDJ_API MKKBallMaker::OnStop()
{
	// ADD YOUR CODE HERE
	logger.createLog(UPDATE, "OnStop: A Plugin leállt.");
	return S_OK;
}
//---------------------------------------------------------------------------
HRESULT VDJ_API MKKBallMaker::OnDraw()
{
	// ADD YOUR CODE HERE
	HRESULT hr;
	TVertex *vertices;

	if (VideoWidth != width || VideoHeight != height)
	{
		hr = OnVideoResize(width, height);
	}

#if (defined(VDJ_WIN))

	hr = GetTexture(VdjVideoEngineDirectX9, (void **)&D3DTexture, &vertices);
	hr = D3DTexture->GetSurfaceLevel(0, &D3DSurface);

	SAFE_RELEASE(D3DTexture);
	SAFE_RELEASE(D3DSurface);

#elif (defined(VDJ_MAC))

	hr = GetTexture(VdjVideoEngineOpenGL, (void **)&GLTexture, &vertices);

	// glEnable(GL_TEXTURE_RECTANGLE_EXT);
	// glBindTexture(GL_TEXTURE_RECTANGLE_EXT, GLTexture);
	// glBegin(GL_TRIANGLE_STRIP);

	//for(int j=0;j<4;j++)
	//{
	//glColorD3D(vertices[j].color);
	//glTexCoord2f(vertices[j].tu, vertices[j].tv);
	//glVertex3f(vertices[j].position.x, vertices[j].position.y, vertices[j].position.z);
	//}

	//glEnd();
#endif


	hr = DrawDeck();
	return S_OK;
}
//---------------------------------------------------------------------------
HRESULT MKKBallMaker::OnVideoResize(int VidWidth, int VidHeight)
{
	// OnDeviceClose();
	// OnDeviceInit();
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

	return TrackData.dump();
}

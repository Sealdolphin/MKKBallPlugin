#ifndef MYPLUGIN8_H
#define MYPLUGIN8_H

#include "vdjVideo8.h"
#include "MKKLogger.h"
#include "json.hpp"

using json = nlohmann::json;

#if (defined(VDJ_WIN))
#define DIRECT3D_VERSION 0x9000
#include <d3dx9.h>
#pragma comment(lib, "d3dx9.lib")
#elif (defined(VDJ_MAC))
#include <OpenGL/OpenGL.h> // you have to import OpenGL.framework in the XCode project
typedef unsigned long D3DCOLOR; // To use the same approach as Microsoft Direct3D
#define D3DCOLOR_RGBA(r,g,b,a) ((D3DCOLOR)(((((a)&0xFF)<<24)|(((r)&0xFF)<<16)|((g)&0xFF)<<8)|((b)&0xFF))) 
#define glColorD3D(d3dcolor) glColor4ub((d3dcolor>>16)&255, (d3dcolor>>8)&255, d3dcolor&255, (d3dcolor>>24)&255 )
#endif

// Some colors
const D3DCOLOR white = D3DCOLOR_RGBA(255, 255, 255, 255);
const D3DCOLOR black = D3DCOLOR_RGBA(0, 0, 0, 255);
const D3DCOLOR red = D3DCOLOR_RGBA(255, 0, 0, 255);
const D3DCOLOR green = D3DCOLOR_RGBA(0, 255, 0, 255);
const D3DCOLOR blue = D3DCOLOR_RGBA(0, 0, 255, 255);
const D3DCOLOR translucide_black = D3DCOLOR_RGBA(0, 0, 0, 120);

///Ez a plugint megvalósító osztály
class MKKBallMaker : public IVdjPluginVideoFx8
{
public:
	HRESULT VDJ_API OnLoad();
	HRESULT VDJ_API OnGetPluginInfo(TVdjPluginInfo8 *infos);
	ULONG VDJ_API Release();
	HRESULT VDJ_API OnDeviceInit();
	HRESULT VDJ_API OnDeviceClose();
	HRESULT VDJ_API OnStart();
	HRESULT VDJ_API OnStop();
	HRESULT VDJ_API OnDraw();

private:
	HRESULT OnVideoResize(int VidWidth, int VidHeight);
	int VideoWidth;
	int VideoHeight;

	MKKLogger logger;
	std::string ip_address;
	std::string port;

#if (defined(VDJ_WIN))

	IDirect3DDevice9* D3DDevice;
	IDirect3DTexture9* D3DTexture;
	IDirect3DSurface9* D3DSurface;

#elif (defined(VDJ_MAC))

	CGLContextObj glContext;
	GLuint GLTexture;

#endif
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
	static const int max_length = 50;
	json TrackData;

	std::string s_title;
	std::string s_artist;
	std::string s_genre;
	std::string s_next_genre;

};

#endif
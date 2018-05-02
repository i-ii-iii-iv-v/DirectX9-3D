#pragma once

#include <windows.h>
#include <tchar.h>
#include <stdio.h>
#include <d3d9.h>
#include <d3dx9.h>
#include "d3dx9tex.h"
#include <ctime>
#include "Render.h"
#include "FPS.h"
#include "Ray.h"
class Game
{
public:
	Game(HWND* hwnd);
	~Game();

	int GameInit();
	int GameLoop();
	int GameShutdown();

	int InitDirect3DDevice(HWND hWndTarget, int Width, int Height, BOOL bWindowed, D3DFORMAT FullScreenFormat, LPDIRECT3D9 pD3D, LPDIRECT3DDEVICE9* ppDevice);
	
	void SetError(char*, ...);
	bool checkStatus();
private:
	HWND g_hWndMain;

	LPDIRECT3D9 g_pD3D;//COM object
	LPDIRECT3DDEVICE9 g_pDevice;//graphics device

	gameA::Render * pRender;
	gameA::FPS fps;
	Camera * camera;
};


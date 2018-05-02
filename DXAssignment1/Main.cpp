#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include "Game.h"
#include "Window.h"

#include <windows.h>
#include <d3d9.h>
#include <d3dx9.h>

#include <string>
#include <sstream>


int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR pstrCmdLine, int iCmdShow) {
	MSG msg;

	gameA::Window wndMain(&hInstance);
	wndMain.createWindow();

	//start game after main window has been created
	Game game(wndMain.getHWnd());
	
	//show window
	wndMain.show(iCmdShow);

	if (game.checkStatus() == false) 
	{
		game.SetError("Initialization Failed");
		game.GameShutdown();
		return E_FAIL;
	}

	while (TRUE) 
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) 
		{
			if (msg.message == WM_QUIT)
				break;
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		else 
		{
			game.GameLoop();
		}
	}

	game.GameShutdown();

	return 0;
}
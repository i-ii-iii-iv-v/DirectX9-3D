#define D3DX_DEFAULT ((UINT)-1)
#include "Game.h"
#include <string>
#include <cstdlib>
#include "Camera.h"
#include "Light.h"
#include "Ray.h"

Game::Game(HWND* hwnd)
{
	g_hWndMain = *hwnd;

	//get Create direct3d object -> direct3d device
	GameInit();

	//initiate renderer
	pRender = new gameA::Render(g_pDevice);

}

Game::~Game()
{

}


int Game::GameInit()
{
	HRESULT r = 0;//return values

	g_pD3D = Direct3DCreate9(D3D_SDK_VERSION);//COM object
	if (g_pD3D == NULL) {
		SetError("Could not create IDirect3D9 object");
		return E_FAIL;
	}

	r = InitDirect3DDevice(g_hWndMain, 640, 480, false, D3DFMT_X8R8G8B8, g_pD3D, &g_pDevice);
	if (FAILED(r)) {//FAILED is a macro that returns false if return value is a failure - safer than using value itself
		SetError("Initialization of the device failed");
		return E_FAIL;
	}

	fps.start();
	camera = new Camera(Camera::AIRCRAFT);
	return S_OK;
}

int Game::GameLoop()
{
	fps.countFrames();
	//Render();
	pRender->render(&fps, camera);

	if ((GetKeyState(VK_LBUTTON) & 0x100) != 0)
	{
		POINT p;
		GetCursorPos(&p);
		ScreenToClient(g_hWndMain, &p);

		OutputDebugString((std::to_string(p.x) + ", " + std::to_string(p.y)+"\n").c_str());
		gameA::Ray ray(g_pDevice);
		ray.calcPickingRay(p.x, p.y);
		D3DXMATRIX view;
		g_pDevice->GetTransform(D3DTS_VIEW, &view);

		D3DXMATRIX viewInverse;
		D3DXMatrixInverse(&viewInverse, 0, &view);
		
		ray.TransformRay(&viewInverse);
		for (int i = 0; i < pRender->meshCount; i++)
		{
			if (ray.RaySphereIntTest(pRender->getMeshArray()[i]->getBoundingSphere()))
			{
				std::string str = "hit : " + std::to_string(i) + "\n";
				OutputDebugString(str.c_str());
				pRender->chooseObject(i);
			}
		}
	}

	if (GetAsyncKeyState(VK_ESCAPE))
		PostQuitMessage(0);
	if (GetAsyncKeyState('1'))
	{
		OutputDebugString("1 selected\n");
		pRender->chooseObject(-1);
	}

	if(GetAsyncKeyState('2'))
	{
		//OutputDebugString("1 selected\n");
		//pRender->chooseObject(1);
	}

	if(GetAsyncKeyState('X')) //lock to X axis
	{
		OutputDebugString("o selected\n");
		pRender->lockAxis('x');
	}
	if (GetAsyncKeyState('Y')) //Lock to Y Axis
	{
		pRender->lockAxis('y');
	}

	if (GetAsyncKeyState('Z')) //Lock to Z Axis
	{
		pRender->lockAxis('z');
	}
	if (GetAsyncKeyState('P')) //positive move
	{
		pRender->translateModel(0.3);
	}

	if (GetAsyncKeyState('O')) //negative
	{
		pRender->translateModel(-0.3);
	}

	if (GetAsyncKeyState('I'))
	{
		pRender->rotateModel(0.05);
	}
	if (GetAsyncKeyState('U'))
	{
		pRender->rotateModel(-0.05);
	}
	if (::GetAsyncKeyState('W') & 0x8000f)
		camera->walk(0.1);

	if (::GetAsyncKeyState('S') & 0x8000f)
		camera->walk(-0.1);

	if (::GetAsyncKeyState('A') & 0x8000f)
		camera->strafe(0.1);

	if (::GetAsyncKeyState('D') & 0x8000f)
		camera->strafe(-0.1);

	if (::GetAsyncKeyState('R') & 0x8000f)
		camera->fly(0.1);	

	if (::GetAsyncKeyState('F') & 0x8000f)
		camera->fly(-0.1);

	if (::GetAsyncKeyState(VK_UP) & 0x8000f)
		camera->pitch(0.02);

	if (::GetAsyncKeyState(VK_DOWN) & 0x8000f)
		camera->pitch(-0.02);

	if (::GetAsyncKeyState(VK_LEFT) & 0x8000f)
		camera->yaw(0.02);

	if (::GetAsyncKeyState(VK_RIGHT) & 0x8000f)
		camera->yaw(-0.02);

	if (::GetAsyncKeyState('N') & 0x8000f)
		camera->roll(0.1);

	if (::GetAsyncKeyState('M') & 0x8000f)
		camera->roll(-0.1);

	if (::GetAsyncKeyState('5'))
		pRender->setLight(gameA::Light::LightType::AMBIENT);

	if (::GetAsyncKeyState('6'))
		pRender->setLight(gameA::Light::LightType::DIRECTIONAL);

	if (::GetAsyncKeyState('7'))
		pRender->setLight(gameA::Light::LightType::POINT);
	if (::GetAsyncKeyState('8'))
		pRender->setSnowOn();
	if (::GetAsyncKeyState('9'))
		pRender->setSnowOff();
	return S_OK;
}

int Game::GameShutdown()
{
	delete(pRender);
	//release resources. First display adapter because COM object created it, then COM object
	if (g_pDevice)
		g_pDevice->Release();

	if (g_pD3D)
		g_pD3D->Release();

	return S_OK;
}


void Game::SetError(char* szFormat, ...)
{
	char szBuffer[1024];
	va_list pArgList;

	va_start(pArgList, szFormat);

	_vsntprintf_s(szBuffer, sizeof(szBuffer) / sizeof(char), szFormat, pArgList);

	va_end(pArgList);

	OutputDebugString(szBuffer);
	OutputDebugString("\n");
	printf("error!!");
}

int Game::InitDirect3DDevice(HWND hWndTarget, int Width, int Height, BOOL bWindowed, D3DFORMAT FullScreenFormat, LPDIRECT3D9 pD3D, LPDIRECT3DDEVICE9* ppDevice) {
	D3DPRESENT_PARAMETERS d3dpp;//rendering info
	D3DDISPLAYMODE d3ddm;//current display mode info
	HRESULT r = 0;

	if (*ppDevice)
		(*ppDevice)->Release();

	ZeroMemory(&d3dpp, sizeof(D3DPRESENT_PARAMETERS));
	r = pD3D->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &d3ddm);
	if (FAILED(r)) {
		SetError("Could not get display adapter information");
		return E_FAIL;
	}

	d3dpp.BackBufferWidth = Width;
	d3dpp.BackBufferHeight = Height;
	d3dpp.BackBufferFormat = bWindowed ? d3ddm.Format : FullScreenFormat;
	d3dpp.BackBufferCount = 1;
	d3dpp.MultiSampleType = D3DMULTISAMPLE_NONE;
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;//D3DSWAPEFFECT_COPY;
	d3dpp.hDeviceWindow = hWndTarget;
	d3dpp.Windowed = bWindowed;
	d3dpp.EnableAutoDepthStencil = TRUE;
	d3dpp.AutoDepthStencilFormat = D3DFMT_D16;
	d3dpp.FullScreen_RefreshRateInHz = 0;//default refresh rate
	d3dpp.PresentationInterval = bWindowed ? 0 : D3DPRESENT_INTERVAL_IMMEDIATE;
	d3dpp.Flags = D3DPRESENTFLAG_LOCKABLE_BACKBUFFER;

	r = pD3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWndTarget, D3DCREATE_HARDWARE_VERTEXPROCESSING, &d3dpp, ppDevice);
	if (FAILED(r)) {
		SetError("Could not create the render device");
		return E_FAIL;
	}

	return S_OK;
}

bool Game::checkStatus()
{
	if (g_pD3D == NULL)
	{
		return false;
	}
	if (g_pDevice == NULL)
	{
		return false;
	}
	return true;
}

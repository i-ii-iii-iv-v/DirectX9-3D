#include "Render.h"
#include "MySurface.h"
#include <string>
#include "Object.h"
#include <cstdlib>
#include <ctime>

namespace gameA
{
	Render::~Render()
	{
		if (bitmapSurface)
			bitmapSurface->Release();
		if (iFont)
			iFont->Release();
	}
	Render::Render(LPDIRECT3DDEVICE9 device)
	{
		HRESULT r = 0;
		pGraphicsDevice = device;

		light = new Light();

		pGraphicsDevice->SetRenderState(D3DRS_AMBIENT, D3DCOLOR_XRGB(100, 100, 100));

		D3DSURFACE_DESC pDesc;

		pGraphicsDevice->Clear(0, 0, D3DCLEAR_TARGET, D3DCOLOR_XRGB(0, 0, 25), 1.0f, 0);
		pGraphicsDevice->GetBackBuffer(0, 0, D3DBACKBUFFER_TYPE_MONO, &pBackSurface);
		pBackSurface->GetDesc(&pDesc);

		//set up font
		fontRect.top = 0;
		fontRect.bottom = 400;
		fontRect.right = 400;
		fontRect.left = 0;

		
		
		r = D3DXCreateFont(pGraphicsDevice, 50, 0, FW_NORMAL, 1, false, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS,
			ANTIALIASED_QUALITY, DEFAULT_PITCH | FF_DONTCARE, "Arial", &iFont);

		r = bitmapSurface->LoadBitmapToSurface("black.bmp", (LPDIRECT3DSURFACE9*)&bitmapSurface, pGraphicsDevice, pDesc.Width, pDesc.Height);

		r = D3DXLoadSurfaceFromSurface(pBackSurface, NULL, NULL, (LPDIRECT3DSURFACE9)bitmapSurface, NULL, NULL, D3DX_FILTER_TRIANGLE, 0);
		if (FAILED(r)) {
			OutputDebugString("could not load surface to surface\n");
		}

		meshArray[0] = new Object("tiger.x", pGraphicsDevice);
		meshArray[1] = new Object("tiger.x", pGraphicsDevice);
		meshArray[2] = new Object("Monkey.x", pGraphicsDevice);
		meshArray[3] = new Object("bigship1.x", pGraphicsDevice);
		meshCount = 4;
		pBackSurface->Release();
		pBackSurface = nullptr;
	
		srand((unsigned int)time(0));
		BoundingBox boundingBox;
		boundingBox._min = D3DXVECTOR3(-30.0f, -30.0f, -30.0f);
		boundingBox._max = D3DXVECTOR3(30.0f, 30.0f, 30.0f);
		Sno = new Snow(&boundingBox, 5000);
		Sno->init(pGraphicsDevice, "snowflake.dds");
	}

	void Render::chooseObject(int index)
	{
		static int choice = 0;

		for (int i = 0; i < meshCount; i++)
		{
			meshArray[i]->setTransform(false);
		}

		if (index == -1)
		{
			choice++;
			if (choice > 3)
			{
				choice = 0;
			}
			meshArray[choice]->setTransform(true);
			return;
		}
		
		meshArray[index]->setTransform(true);
	}

	int Render::render(FPS * fps, Camera * cam)
	{
		HRESULT r = 0;
		LPDIRECT3DSURFACE9 pBackSurface = nullptr;
		if (pGraphicsDevice == nullptr)
		{
			OutputDebugString("No device to render");
			OutputDebugString("\n");
			return 1;
		}

		Sno->update(0.1);
		//clear the render target with colour black, ignore stencil buffer
		pGraphicsDevice->Clear(0, 0, D3DCLEAR_TARGET| D3DCLEAR_ZBUFFER, 0x00000000, 1.0f, 0);
		pGraphicsDevice->GetBackBuffer(0, 0, D3DBACKBUFFER_TYPE_MONO, &pBackSurface);


		//render method:
		//D3DXLoadSurfaceFromSurface(pBackSurface, NULL, NULL, pSurface, NULL, NULL, D3DX_FILTER_TRIANGLE, 0);
		pGraphicsDevice->UpdateSurface(bitmapSurface, NULL, pBackSurface, NULL);

		pBackSurface->Release();
		pBackSurface = 0;

		D3DXMATRIX V;
		cam->getViewMatrix(&V);
		pGraphicsDevice->SetTransform(D3DTS_VIEW, &V);

		pGraphicsDevice->BeginScene();
		//scene paint goes here

		for (int i = 0; i < meshCount; i++)
		{
			meshArray[i]->translateObject(moveSum.x, moveSum.y, moveSum.z);
			meshArray[i]->rotateObject(axis, angle);
			meshArray[i]->drawSubset(pGraphicsDevice);

		}
		

		static int i;
		i++;
		iFont->DrawTextA(NULL, std::to_string((fps->getFrameRate())).c_str(), -1, &fontRect, DT_LEFT | DT_NOCLIP, 0xFFFFFF00);
		if(snowSystem)
			Sno->render();
		pGraphicsDevice->EndScene();

		pGraphicsDevice->Present(NULL, NULL, NULL, NULL);//swaps buffer
		resetSums();
		return 0;
	}
	void Render::translateModel(float value)
	{
		if (axis == 'x')
			moveSum.x += value;

		if (axis == 'y')
			moveSum.y += value;

		if (axis == 'z')
			moveSum.z += value;
	}
	void Render::resetSums()
	{
		moveSum.x = 0;
		moveSum.y = 0;
		moveSum.z = 0;
		angle = 0;
	}
	void Render::lockAxis(char c)
	{
		axis = c;
	}
	void Render::rotateModel(float value)
	{
		angle = value;

	}
	void Render::setLight(Light::LightType type)
	{
		light->setLightType(type);
		light->emit(pGraphicsDevice);
	}
	Object ** Render::getMeshArray()
	{
		return meshArray;
	}
	Object * Render::getMesh1()
	{
		return mesh1;
	}
	void Render::setSnowOff()
	{
		snowSystem = false;
	}
	void Render::setSnowOn()
	{
		snowSystem = true;
	}
}
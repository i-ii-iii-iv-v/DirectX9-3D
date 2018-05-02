#pragma once
#include <d3d9.h>
#include <d3dx9.h>
#include "MySurface.h"
#include "Object.h"
#include "FPS.h"
#include "Camera.h"
#include "Light.h"
#include "pSystem.h"
#define CUSTOMFVF (D3DFVF_XYZRHW|D3DFVF_DIFFUSE)
namespace gameA
{
	class Render
	{

	typedef struct moveStruct
	{
		float x, y, z;
	} moveStruct;

	typedef struct rotStruct
	{
		float x, y, z;
	}rotStruct;

	public:
		Render::Render() = delete;
		Render::Render(LPDIRECT3DDEVICE9 g_pDevice);
		Render::~Render();

		void chooseObject(int index);
		int render(FPS* fps, Camera* cam);
		void translateModel(float value);
		void resetSums();
		void lockAxis(char c);
		void rotateModel(float value);
		void setLight(Light::LightType type);
		Object** getMeshArray();
		Object * getMesh1();
		int meshCount = 0;
		
		void setSnowOff();
		void setSnowOn();
	private:
		LPDIRECT3DDEVICE9 pGraphicsDevice; //graphics card

		LPDIRECT3DSURFACE9 pBackSurface;
		MySurface * bitmapSurface;
		LPDIRECT3DVERTEXBUFFER9 v_buffer;
		ID3DXFont * iFont;
		RECT fontRect;

		Object * mesh1;
		Object * mesh2;
		Object* meshArray[20];
		
		Light *light;
		char axis;
		float angle;

		PSystem	* Sno = 0;
		bool snowSystem = false;
		
		rotStruct rotSum
		{
			0,0,0
		};
		moveStruct moveSum
		{
			0,0,0
		};
	};
}

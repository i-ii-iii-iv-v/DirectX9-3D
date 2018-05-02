#pragma once
#include <windows.h>
#include <tchar.h>
#include <stdio.h>
#include <d3d9.h>
#include <d3dx9.h>
#include "BoundingSphere.h"

namespace gameA
{
	class Ray
	{
	private:
		D3DXVECTOR3 origin;
		D3DXVECTOR3 direction;
		LPDIRECT3DDEVICE9 pGraphicsDevice;

	public:
		Ray(LPDIRECT3DDEVICE9 device);
		void calcPickingRay(int x, int y);
		void TransformRay(D3DXMATRIX* T);
		bool RaySphereIntTest(BoundingSphere* sphere);
	};
}
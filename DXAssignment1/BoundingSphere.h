#pragma once
#include <windows.h>
#include <tchar.h>
#include <stdio.h>
#include <d3d9.h>
#include <d3dx9.h>

namespace gameA
{
	class BoundingSphere
	{
	private:
		D3DXVECTOR3 center;
		float       radius;
		
		ID3DXMesh* Sphere;
	public:
		BoundingSphere(LPDIRECT3DDEVICE9 pGraphicsDevice, ID3DXMesh * mesh);
		float getRadius();
		D3DXVECTOR3 getCenter();	
		void setSphere(LPDIRECT3DDEVICE9 pGraphicsDevice, D3DXVECTOR3 pCenter, ID3DXMesh * mesh);
	};
}
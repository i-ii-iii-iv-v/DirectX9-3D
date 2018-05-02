#pragma once
#include <d3d9.h>
#include <d3dx9.h>
#include <Vector>
#include "BoundingSphere.h"

namespace gameA
{
	class Object
	{
	public:
		Object::Object(const char * fileName, LPDIRECT3DDEVICE9 pDevice);
		Object::~Object();
		int getNumMaterials();
		void drawSubset(LPDIRECT3DDEVICE9 pDevice);
		std::vector<D3DMATERIAL9>& getMtrlVector();
		std::vector<IDirect3DTexture9*>& getTextureVector();
		void translateObject(float x, float y, float z);
		void rotateObject(char axis, float angle);
		void setTransform(bool);
		BoundingSphere* getBoundingSphere();

	private:
		LPDIRECT3DDEVICE9 pGraphicsDevice;
		ID3DXBuffer * adjBuffer;
		ID3DXBuffer * mtrlBuffer;
		DWORD numMtrls;
		ID3DXMesh * mesh;
		BoundingSphere* bSphere = nullptr;
		D3DXMATRIX worldMatrix, transMatrix, rotMatrixX, rotMatrixY, rotMatrixZ;
		float worldX;
		float worldY;
		float worldZ;

		float angx;
		float angy; 
		float angz;

		std::vector<D3DMATERIAL9> mtrlVector;
		std::vector<IDirect3DTexture9*> TextureVector;

		bool transform;
	};
}

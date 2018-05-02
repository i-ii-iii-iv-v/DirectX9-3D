#include "Object.h"
#include <String>
namespace gameA
{
	Object::Object(const char * fileName, LPDIRECT3DDEVICE9 pDevice)
		:worldX(0), worldY(0), worldZ(0), angx(0), angy(0), angz(0), transform(true)
	{
		pGraphicsDevice = pDevice;
		D3DXMatrixIdentity(&worldMatrix);
		HRESULT hr = 0;
		hr = D3DXLoadMeshFromX(fileName, D3DXMESH_MANAGED, pDevice, &adjBuffer, &mtrlBuffer, 0, &numMtrls, &mesh);

		if (FAILED(hr))
		{
			OutputDebugString("Error Loading mesh\n");
			return;
		}

		if (mtrlBuffer != 0 && numMtrls != 0)
		{
			D3DXMATERIAL * mtrls = (D3DXMATERIAL*)mtrlBuffer->GetBufferPointer();

			for (int i = 0; i < numMtrls; i++)
			{
				mtrls[i].MatD3D.Ambient = mtrls[i].MatD3D.Diffuse;
				mtrlVector.push_back(mtrls[i].MatD3D);

				if (mtrls[i].pTextureFilename != 0)
				{
					// yes, load the texture for the ith subset
					IDirect3DTexture9* tex = 0;
					D3DXCreateTextureFromFile(
						pDevice,
						mtrls[i].pTextureFilename,
						&tex);

					// save the loaded texture
					TextureVector.push_back(tex);
				}

				else
				{
					TextureVector.push_back(0);
				}
			}
		}

		hr = mesh->OptimizeInplace(D3DXMESHOPT_ATTRSORT | D3DXMESHOPT_COMPACT | D3DXMESHOPT_VERTEXCACHE, (DWORD*)adjBuffer->GetBufferPointer(), 0, 0, 0);

		mtrlBuffer->Release();
		adjBuffer->Release();

		if (FAILED(hr))
		{
			OutputDebugString("Optimizing failed\n");
			return;
		}

		hr = pDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
		hr = pDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
		hr  = pDevice->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_POINT);

		D3DXVECTOR3 pos(4.0f, 4.0f, -13.0f);
		D3DXVECTOR3 target(0.0f, 0.0f, 0.0f);
		D3DXVECTOR3 up(0.0f, 1.0f, 0.0f);

		D3DXMATRIX V;
		D3DXMatrixLookAtLH(
			&V,
			&pos,
			&target,
			&up);

		pDevice->SetTransform(D3DTS_VIEW, &V);


		D3DDEVICE_CREATION_PARAMETERS DCP;
		RECT rect;
		hr = pDevice->GetCreationParameters(&DCP);
		int width = 0;
		int height = 0;
		GetWindowRect(DCP.hFocusWindow, &rect);
		width = rect.right - rect.left;
		height = rect.bottom - rect.top;
		std::string msg = "width: "+ std::to_string(width) + " height: " + std::to_string(height);
		OutputDebugString(msg.c_str());

		D3DXMATRIX proj;
		D3DXMatrixPerspectiveFovLH(
			&proj,
			D3DX_PI * 0.5f, // 90 - degree
			(float)width / (float)height,
			1.0f,
			1000.0f);
		pDevice->SetTransform(D3DTS_PROJECTION, &proj);
		
		bSphere = new BoundingSphere(pDevice, mesh);
		bSphere->setSphere(pDevice, pos, mesh);
	}

	Object::~Object()
	{
		delete bSphere;
	}

	int Object::getNumMaterials()
	{
		return numMtrls;
	}

	void Object::drawSubset(LPDIRECT3DDEVICE9 pDevice)
	{
		worldMatrix = (rotMatrixX*rotMatrixY*rotMatrixZ)*transMatrix;
		pDevice->SetTransform(D3DTS_WORLD, &worldMatrix);
		for (int i = 0; i < mtrlVector.size(); i++)
		{
			HRESULT hr;
			hr = pDevice->SetMaterial(&mtrlVector[i]);
			if (FAILED(hr))
			{
				OutputDebugString("mtrls set failed\n");
				return;
			}

			hr = pDevice->SetTexture(0, TextureVector[i]);
			if (FAILED(hr))
			{
				OutputDebugString("texture set failed\n");
				return;
			}
			hr = mesh->DrawSubset(i);
			if (FAILED(hr))
			{
				OutputDebugString("sraw subset failed\n");
				return;
			}
		}
	}

	std::vector<D3DMATERIAL9>& Object::getMtrlVector()
	{
		return mtrlVector;
	}

	std::vector<IDirect3DTexture9*>& Object::getTextureVector()
	{
		return TextureVector;
	}

	void Object::translateObject(float x, float y, float z)
	{
		if (transform == false)
		{
			return;
		}
		worldX += x;
		worldY += y;
		worldZ += z;
		D3DXMatrixTranslation(&transMatrix, worldX, worldY, worldZ);
		D3DXVECTOR3 center(worldX, worldY, worldZ);
		bSphere->setSphere(pGraphicsDevice, center, mesh);
	}

	void Object::rotateObject(char axis, float angle)
	{
		if (transform == false)
		{
			return;
		}
		if (axis == 'x')
		{
			angx += angle;

		}
		if (axis == 'y')
		{
			angy += angle;

		}
		else if (axis == 'z')
		{
			angz += angle;

		}
		D3DXMatrixRotationX(&rotMatrixX, angx);
		D3DXMatrixRotationY(&rotMatrixY, angy);
		D3DXMatrixRotationZ(&rotMatrixZ, angz);
	}

	void Object::setTransform(bool onOff)
	{
		transform = onOff;
	}

	BoundingSphere * Object::getBoundingSphere()
	{
		return bSphere;
	}

	
}
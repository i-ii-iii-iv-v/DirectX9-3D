#include "Light.h"

gameA::Light::Light()
{
}

void gameA::Light::setLightType(LightType type)
{
	_lightType = type;
}

void gameA::Light::emit(LPDIRECT3DDEVICE9 pDevice)
{
	pDevice->SetRenderState(D3DRS_AMBIENT, D3DCOLOR_XRGB(0, 0, 0));
	pDevice->LightEnable(1, FALSE);
	pDevice->LightEnable(0, FALSE);
	if (_lightType == AMBIENT)
	{
		pDevice->SetRenderState(D3DRS_AMBIENT, D3DCOLOR_XRGB(150, 150, 150));
		return;
	}

	if (_lightType == POINT)
	{
		
		D3DLIGHT9 light;
		ZeroMemory(&light, sizeof(D3DLIGHT9));
		light.Type = D3DLIGHT_POINT;
		light.Diffuse.r = 1.0f;
		light.Diffuse.g = 1.0f;
		light.Diffuse.b = 1.0f;
		light.Position = D3DVECTOR{0,0,20};

		light.Attenuation0 = 0.1f;
		light.Range = 200.0f;
		pDevice->SetLight(1, &light);
		pDevice->LightEnable(1, TRUE);
		pDevice->SetRenderState(D3DRS_LIGHTING, true);
		return;
	}

	if (_lightType == DIRECTIONAL)
	{
		D3DLIGHT9 light;
		ZeroMemory(&light, sizeof(D3DLIGHT9));
		light.Type = D3DLIGHT_DIRECTIONAL;
		light.Diffuse.r = 1.0f;
		light.Diffuse.g = 1.0f;
		light.Diffuse.b = 1.0f;
		light.Diffuse.a = 1.0f;
		light.Range = 1000.0f;

		// Create a direction for our light - it must be normalized  
		D3DXVECTOR3 vecDir;
		vecDir = D3DXVECTOR3(0.0f, -0.3f, 0.5);
		D3DXVec3Normalize((D3DXVECTOR3*)&light.Direction, &vecDir);

		// Tell the device about the light and turn it on
		pDevice->SetLight(0, &light);
		pDevice->LightEnable(0, TRUE);

		pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
		return;
	}
	if (_lightType == SPOT)
	{

	}
}

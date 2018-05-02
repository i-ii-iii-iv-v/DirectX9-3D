#pragma once
#include <d3d9.h>
#include <d3dx9.h>
namespace gameA
{
	class Light
	{
	public:
		enum LightType { AMBIENT, POINT,DIRECTIONAL, SPOT };
		Light::Light();
		void setLightType(LightType type);
		void emit(LPDIRECT3DDEVICE9 pDevice);
	private:
		LightType _lightType;
	};
}
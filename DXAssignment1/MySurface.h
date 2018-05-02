#pragma once
#include <d3d9.h>
#include <d3dx9.h>
#include "d3dx9tex.h"

namespace gameA
{
	class MySurface: public IDirect3DSurface9
	{
	public:
		MySurface();
		virtual ~MySurface();
		static int MySurface::LoadBitmapToSurface(char* PathName, LPDIRECT3DSURFACE9* ppSurface, LPDIRECT3DDEVICE9 D3DDevice, UINT w, UINT h);
	private:

	};
}

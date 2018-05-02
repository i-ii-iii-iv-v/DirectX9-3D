#include "MySurface.h"

namespace gameA
{
	MySurface::MySurface()
		:IDirect3DSurface9()
	{
		
	}

	MySurface::~MySurface()
	{

	}

	int MySurface::LoadBitmapToSurface(char* PathName, LPDIRECT3DSURFACE9* ppSurface, LPDIRECT3DDEVICE9 D3DDevice, UINT w, UINT h)
	{
		HRESULT r;
		HBITMAP hBitmap;
		BITMAP Bitmap;

		hBitmap = (HBITMAP)LoadImage(NULL, PathName, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
		if (hBitmap == NULL) {
			OutputDebugString("Unable to load bitmap\n");
			return E_FAIL;
		}

		GetObject(hBitmap, sizeof(BITMAP), &Bitmap);
		DeleteObject(hBitmap);//we only needed it for the header info to create a D3D surface

							  //create surface for bitmap

		r = D3DDevice->CreateOffscreenPlainSurface(w, h, D3DFMT_X8R8G8B8, D3DPOOL_SYSTEMMEM, ppSurface, NULL);


		if (FAILED(r)) {
			OutputDebugString("Unable to create surface for bitmap load\n");
			return E_FAIL;
		}
		//load bitmap onto surface
		r = D3DXLoadSurfaceFromFile(*ppSurface, NULL, NULL, PathName, NULL, D3DX_DEFAULT, 0, NULL);
		if (FAILED(r)) {
			OutputDebugString("Unable to laod file to surface\n");
			return E_FAIL;
		}

		return S_OK;
	}
}
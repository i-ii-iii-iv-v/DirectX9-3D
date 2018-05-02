#include "Ray.h"

gameA::Ray::Ray(LPDIRECT3DDEVICE9 device)
{
	pGraphicsDevice = device;
}

void gameA::Ray::calcPickingRay(int x, int y)
{
	float px = 0.0f;
	float py = 0.0f;

	D3DVIEWPORT9 vp;
	pGraphicsDevice->GetViewport(&vp);

	D3DXMATRIX proj;
	pGraphicsDevice->GetTransform(D3DTS_PROJECTION, &proj);

	px = (((2.0f*x) / vp.Width) - 1.0f) / proj(0, 0);
	py = (((-2.0f*y) / vp.Height) + 1.0f) / proj(1, 1);

	
	origin = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	direction = D3DXVECTOR3(px, py, 1.0f);
}

void gameA::Ray::TransformRay(D3DXMATRIX * T)
{
	// transform the ray's origin, w = 1.
	D3DXVec3TransformCoord(&origin, &origin, T);

	// transform the ray's direction, w = 0.
	D3DXVec3TransformNormal(&direction,	&direction, T);

	// normalize the direction
	D3DXVec3Normalize(&direction, &direction);
}

bool gameA::Ray::RaySphereIntTest(BoundingSphere * sphere)
{
	D3DXVECTOR3 v = origin - sphere->getCenter();

	float b = 2.0f * D3DXVec3Dot(&direction, &v);
	float c = D3DXVec3Dot(&v, &v) - (sphere->getRadius() * sphere->getRadius());

	// find the discriminant
	float discriminant = (b * b) - (4.0f * c);

	// test for imaginary number
	if (discriminant < 0.0f)
		return false;

	discriminant = sqrtf(discriminant);

	float s0 = (-b + discriminant) / 2.0f;
	float s1 = (-b - discriminant) / 2.0f;

	// if a solution is >= 0, then we intersected the sphere
	if (s0 >= 0.0f || s1 >= 0.0f)
		return true;

	return false;
}

#include "BoundingSphere.h"

gameA::BoundingSphere::BoundingSphere(LPDIRECT3DDEVICE9 pGraphicsDevice, ID3DXMesh * mesh)
{
	radius = 0;
	BYTE* v = 0;
	mesh->LockVertexBuffer(0, (void**)&v);
	D3DXComputeBoundingSphere((D3DXVECTOR3*)v,mesh->GetNumVertices(),D3DXGetFVFVertexSize(mesh->GetFVF()),&center,&radius);
	mesh->UnlockVertexBuffer();
	D3DXCreateSphere(pGraphicsDevice, radius, 20, 20, &Sphere, 0);
}

float gameA::BoundingSphere::getRadius()
{
	return radius;
}

D3DXVECTOR3 gameA::BoundingSphere::getCenter()
{
	return center;
}

void gameA::BoundingSphere::setSphere(LPDIRECT3DDEVICE9 pGraphicsDevice, D3DXVECTOR3 pCenter, ID3DXMesh * mesh)
{
	center = pCenter;
}

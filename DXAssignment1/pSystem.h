#pragma once
#include <d3d9.h>
#include <d3dx9.h>
#include "d3dx9tex.h"
#include <list>
#include <string>
#include <limits>

namespace gameA
{
	struct BoundingBox
	{
		BoundingBox();

		bool isPointInside(D3DXVECTOR3& p);

		D3DXVECTOR3 _min;
		D3DXVECTOR3 _max;
	};
	struct Particle
	{
		D3DXVECTOR3 _position;
		D3DCOLOR    _color;
		static const DWORD FVF;
	};

	struct Attribute
	{
		Attribute()
		{
			_lifeTime = 0.0f;
			_age = 0.0f;
			_isAlive = true;
		}

		D3DXVECTOR3 _position;
		D3DXVECTOR3 _velocity;
		D3DXVECTOR3 _acceleration;
		float       _lifeTime;     // how long the particle lives for before dying  
		float       _age;          // current age of the particle  
		D3DXCOLOR   _color;        // current color of the particle   
		D3DXCOLOR   _colorFade;    // how the color fades with respect to time
		bool        _isAlive;
	};


	class PSystem
	{
	public:
		PSystem();
		virtual ~PSystem();

		virtual bool init(IDirect3DDevice9* device, char* texFileName);
		virtual void reset();

		// sometimes we don't want to free the memory of a dead particle,
		// but rather respawn it instead.
		virtual void resetParticle(Attribute* attribute) = 0;
		virtual void addParticle();

		virtual void update(float timeDelta) = 0;

		virtual void preRender();
		virtual void render();
		virtual void postRender();

		bool isEmpty();
		bool isDead();
		float getRandomFloat(float lowBound, float highBound);
		DWORD FtoDw(float f);
	protected:
		virtual void removeDeadParticles();

	protected:
		IDirect3DDevice9*       _device;
		D3DXVECTOR3             _origin;
		BoundingBox        _boundingBox;
		float                   _emitRate;   // rate new particles are added to system
		float                   _size;       // size of particles
		IDirect3DTexture9*      _tex;
		IDirect3DVertexBuffer9* _vb;
		std::list<Attribute>    _particles;
		int                     _maxParticles; // max allowed particles system can have

											   //
											   // Following three data elements used for rendering the p-system efficiently
											   //

		DWORD _vbSize;      // size of vb
		DWORD _vbOffset;    // offset in vb to lock   
		DWORD _vbBatchSize; // number of vertices to lock starting at _vbOffset
	};

	class Snow : public PSystem
	{
	public:
		Snow(BoundingBox* boundingBox, int numParticles);
		void resetParticle(Attribute* attribute);
		void update(float timeDelta);
	};
}
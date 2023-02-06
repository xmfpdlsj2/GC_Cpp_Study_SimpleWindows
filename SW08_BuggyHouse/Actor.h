#pragma once
#include "D2DFramework.h"

class Actor
{
protected:
	D2DFramework* mpFrameWork;
	ID2D1Bitmap* mpBitmap;

	float mX, mY, mOpacity;
	float mWidth, mHeight;

public: 
	Actor() = delete;
	Actor(D2DFramework* pFramework, std::wstring filename);
	Actor(D2DFramework* pFramework, std::wstring filename, float x, float y, float scale = 1.0f, float opacity = 1.0f );
	virtual ~Actor();

protected:
	void Draw(float x, float y, float opacity = 1.0f);

public:
	virtual void Draw();
};


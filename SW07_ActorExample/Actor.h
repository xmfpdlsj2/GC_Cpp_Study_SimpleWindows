#pragma once

#include "D2DFramework.h"

class Actor
{
protected:
	D2DFramework* mpFrameWork;
	Microsoft::WRL::ComPtr<ID2D1Bitmap> mspBitmap;

	float mX, mY, mOpacity;

public: 
	Actor() = delete;
	Actor(D2DFramework* pFramework, std::wstring filename);
	Actor(D2DFramework* pFramework, std::wstring filename, float x, float y, float opacity = 1.0f);
	virtual ~Actor();

protected:
	HRESULT LoadWICImage(LPCWSTR filename);
	void Draw(float x, float y, float opacity = 1.0f);

public:
	virtual void Draw();
};


#include "Actor.h"

Actor::Actor(D2DFramework* pFramework, std::wstring filename) :
	Actor(pFramework, filename, 0.0f, 0.0f, 1.0f, 1.0f)
{
}

Actor::Actor(D2DFramework* pFramework, std::wstring filename, float x, float y, float scale, float opacity) :
	mpFrameWork{ pFramework }, mX{ x }, mY{ y }, mOpacity{ opacity }
{
	mpBitmap = BitmapManager::Instance().LoadBitmaps(filename);

	auto size = mpBitmap->GetPixelSize();
	mWidth = size.width * scale;
	mHeight = size.height * scale;
}

Actor::~Actor()
{
}

void Actor::Draw(float x, float y, float opacity)
{
	auto pRenderTarget = mpFrameWork->GetRenderTarget();

	D2D_RECT_F rect{ x, y, x + mWidth, y + mHeight};

	pRenderTarget->DrawBitmap(mpBitmap, rect, opacity);
}

void Actor::Draw()
{
	Draw(mX, mY, mOpacity);
}

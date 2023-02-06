#include "Actor.h"

Actor::Actor(D2DFramework* pFramework, std::wstring filename) :
	Actor(pFramework, filename, 0.0f, 0.0f, 1.0f, 1.0f)
{
}

Actor::Actor(D2DFramework* pFramework, std::wstring filename, float x, float y, float opacity, float scale) :
	mpFrameWork{ pFramework }, mX{ x }, mY{ y }, mOpacity{ opacity }, mScale{ scale }
{
	mpBitmap = BitmapManager::Instance().LoadBitmaps(filename);
}

Actor::~Actor()
{
}

void Actor::Draw(float x, float y, float opacity, float scale)
{
	auto pRenderTarget = mpFrameWork->GetRenderTarget();
	auto size = mpBitmap->GetPixelSize();

	D2D_RECT_F rect{ x, y, x + size.width * scale, y + size.height * scale };

	pRenderTarget->DrawBitmap(mpBitmap, rect, opacity);
}

void Actor::Draw()
{
	Draw(mX, mY, mOpacity, mScale);
}

#include "Bug_ladybug.h"

Bug_ladybug::Bug_ladybug(D2DFramework* pFramework) : Bug(pFramework, L"Data/BH_Bug_flies.png", 0.4f)
{
	GetClientRect(pFramework->GetWindowHandle(), &mTurnPoint);
	mIsTurn = false;
	mSpeed = 2 + (rand() % 2);
}

void Bug_ladybug::Draw()
{
	if ((mX < 0 || mX > mTurnPoint.right || mY < 0 || mY > mTurnPoint.bottom))
	{
		if (++mSteps > 10) 
		{
			mSteps = 0;
			mRotation += (1 - rand() % 3) * 75;
		}
	}

	auto target = UPVECTOR * D2D1::Matrix3x2F::Rotation(mRotation);
	mX += target.x * mSpeed;
	mY += target.y * mSpeed;

	auto matTranslate = D2D1::Matrix3x2F::Translation(mX, mY);

	auto matRotation = D2D1::Matrix3x2F::Rotation(mRotation
		, D2D_POINT_2F{ mWidth * 0.5f, mHeight * 0.5f }
	);

	auto pRT = mpFrameWork->GetRenderTarget();
	pRT->SetTransform(matRotation * matTranslate);

	D2D1_RECT_F rct{ 0, 0, mWidth, mHeight };
	pRT->DrawBitmap(mpBitmap, rct, mOpacity);
}

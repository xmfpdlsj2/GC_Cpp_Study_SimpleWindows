#include "Bug.h"

Bug::Bug(D2DFramework* pFramework) : Bug(pFramework, L"Data/BH_Bug_ecco.png", 0.05f)
{
}

Bug::Bug(D2DFramework* pFramework, std::wstring fileName, float scale) : Actor(pFramework, fileName, 0, 0, scale)
{
	RECT rct;
	GetClientRect(pFramework->GetWindowHandle(), &rct);
	mX = static_cast<float>(rct.left + rand() % (rct.right - rct.left));
	mY = static_cast<float>(rct.top + rand() % (rct.bottom - rct.top));

	// 열거형 방식
	//mDirection = Direction::UP;

	// 벡터방식
	mRotation = 0.0f;

	mSteps = 0;
}
Bug::~Bug()
{

}
void Bug::Draw()
{
	// 열거형 방식
	//if (mSteps++ > 20)
	//{
	//	mSteps = 0;

	//	int dir = static_cast<int>(mDirection);
	//	dir += 1 - rand() % 3;

	//	int count = static_cast<int>(Direction::COUNT);
	//	dir = (dir + count) % count;

	//	mDirection = static_cast<Direction>(dir);
	//}

	//switch (mDirection)
	//{
	//	case Bug::Direction::UP:
	//		mY--;
	//		break;
	//	case Bug::Direction::UP_RIGHT:
	//		mY--;
	//		mX++;
	//		break;
	//	case Bug::Direction::RIGHT:
	//		mX++;
	//		break;
	//	case Bug::Direction::DOWN_RIGHT:
	//		mX++;
	//		mY++;
	//		break;
	//	case Bug::Direction::DOWN:
	//		mY++;
	//		break;
	//	case Bug::Direction::DOWN_LEFT:
	//		mX--;
	//		mY++;
	//		break;
	//	case Bug::Direction::LEFT:
	//		mX--;
	//		break;
	//	case Bug::Direction::UP_LEFT:
	//		mX--;
	//		mY--;
	//		break;
	//	case Bug::Direction::COUNT:
	//		break;
	//	default:
	//		break;
	//}

	// 벡터방식
	if (mSteps++ > 10)
	{
		mSteps = 0;
		mRotation += (1 - rand() % 3) * 45;
	}

	auto target = UPVECTOR * D2D1::Matrix3x2F::Rotation(mRotation);
	mX += target.x;
	mY += target.y;

	auto matTranslate = D2D1::Matrix3x2F::Translation(mX, mY);

	auto matRotation = D2D1::Matrix3x2F::Rotation(mRotation
		, D2D_POINT_2F{mWidth * 0.5f, mHeight * 0.5f}
	);

	auto pRT = mpFrameWork->GetRenderTarget();
	pRT->SetTransform(matRotation * matTranslate);

	D2D1_RECT_F rct{ 0, 0, mWidth, mHeight };
	pRT->DrawBitmap(mpBitmap, rct, mOpacity);
}

bool Bug::IsClicked(POINT& pt)
{
	if (pt.x >= mX && pt.x <= mX + mWidth &&
		pt.y >= mY && pt.y <= mY + mHeight)
	{
		return true;
	}

	return false;
}

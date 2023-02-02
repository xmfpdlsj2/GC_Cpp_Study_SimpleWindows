#include "Bug.h"

Bug::Bug(D2DFramework* pFramework) : Actor(pFramework, L"Data/01_ecco_1.png")
{
	RECT rct;
	GetClientRect(pFramework->GetWindowHandle(), &rct);
	mX = static_cast<float>(rct.left + rand() % (rct.right - rct.left));
	mY = static_cast<float>(rct.top + rand() % (rct.bottom - rct.top));
	mScale = 0.1f;

	mDirection = Direction::UP;
	mSteps = 0;
}

void Bug::Draw()
{
	if (mSteps++ > 20)
	{
		mSteps = 0;

		int dir = static_cast<int>(mDirection);
		dir += 1 - rand() % 3;

		int count = static_cast<int>(Direction::COUNT);
		dir = (dir + count) % count;

		mDirection = static_cast<Direction>(dir);
	}

	switch (mDirection)
	{
		case Bug::Direction::UP:
			mY--;
			break;
		case Bug::Direction::UP_RIGHT:
			mY--;
			mX++;
			break;
		case Bug::Direction::RIGHT:
			mX++;
			break;
		case Bug::Direction::DOWN_RIGHT:
			mX++;
			mY++;
			break;
		case Bug::Direction::DOWN:
			mY++;
			break;
		case Bug::Direction::DOWN_LEFT:
			mX--;
			mY++;
			break;
		case Bug::Direction::LEFT:
			mX--;
			break;
		case Bug::Direction::UP_LEFT:
			mX--;
			mY--;
			break;
		case Bug::Direction::COUNT:
			break;
		default:
			break;
	}

	Actor::Draw();
}

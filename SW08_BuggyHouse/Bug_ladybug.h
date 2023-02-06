#pragma once
#include "Bug.h"
class Bug_ladybug : public Bug
{
	RECT mTurnPoint;
	bool mIsTurn;
	float mSpeed;

public:
	Bug_ladybug() = delete;
	Bug_ladybug(D2DFramework* pFramework);

	virtual void Draw() override;
};


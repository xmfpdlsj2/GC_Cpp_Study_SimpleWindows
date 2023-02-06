#pragma once
#include "Actor.h"

class Bug : public Actor
{
	// ó�� ���������
	//enum class Direction
	//{
	//	UP,
	//	UP_RIGHT,
	//	RIGHT,
	//	DOWN_RIGHT,
	//	DOWN,
	//	DOWN_LEFT,
	//	LEFT,
	//	UP_LEFT,

	//	COUNT
	//};

	//Direction mDirection;

	// ���͹��
protected:
	const D2D_POINT_2F UPVECTOR{ 0.0f, -1.0f };
	float mRotation;

	float mSteps;

public:
	Bug() = delete;
	Bug(D2DFramework* pFramework);
	Bug(D2DFramework* pFramework, std::wstring fileName, float scale);
	virtual ~Bug();

	virtual void Draw() override;

	bool IsClicked(POINT& pt);
};


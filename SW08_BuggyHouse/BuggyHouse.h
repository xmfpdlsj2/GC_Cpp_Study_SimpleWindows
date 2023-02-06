#pragma once
#include <list>
#include "D2DFramework.h"
#include "Actor.h"
#include "Bug_ecco.h"
#include "Bug_ladybug.h"

class BuggyHouse : public D2DFramework
{
	std::shared_ptr<Actor> mspBackground;
	std::list<std::shared_ptr<Bug_ecco>> mBugs;
	std::list<std::shared_ptr<Bug_ladybug>> mLadyBugs;

private:
	bool mIsClicked{ false };

public:
	virtual void Initialize(
		HINSTANCE hInstance,
		LPCWSTR title = L"Bug House Game",
		UINT width = 1024, UINT height = 768
		) override;
	virtual void Release() override;
	virtual void Render() override;

	void CheckBug();
};


#pragma once
#include "Bug.h"

class Bug_ecco : public Bug
{
public:
	Bug_ecco() = delete;
	Bug_ecco(D2DFramework* pFramework);

	virtual void Draw() override;
};


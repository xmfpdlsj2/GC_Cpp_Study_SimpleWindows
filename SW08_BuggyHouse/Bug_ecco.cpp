#include "Bug_ecco.h"

Bug_ecco::Bug_ecco(D2DFramework* pFramework) : Bug(pFramework, L"Data/BH_Bug_ecco.png", 0.1f)
{
}

void Bug_ecco::Draw()
{
	Bug::Draw();
}

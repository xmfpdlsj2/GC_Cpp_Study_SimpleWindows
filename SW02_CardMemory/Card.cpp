#include "Card.h"

namespace solitaire
{
	Card::Card(Type type, unsigned int x, unsigned int y, HWND hwnd, int index) :
		mType {type}, mX{ x }, mY{ y }, mIsFront{ false },
		mHwnd{ hwnd }, mIndex{ index }
	{
		std::wstring filename{};
		switch (type)
		{
			case Type::Wolf:
				filename = L"Data/card_creature_wolf.png";
				break;
			case Type::Dragon:
				filename = L"Data/card_creature_dragon.png";
				break;
			case Type::Bear:
				filename = L"Data/card_creature_bear.png";
				break;
			default:
				break;
		}
		mFront = std::make_unique<Gdiplus::Image>(filename.c_str());
		mBack = std::make_unique<Gdiplus::Image>(L"Data/card_back.png");
	}
	bool Card::CheckClicked(unsigned int x, unsigned int y)
	{
		//// Origin version
		//if (x >= mX && x <= mX + mFront->GetWidth() &&
		//	y >= mY && y <= mY + mFront->GetHeight())
		//{
		//	Flip(!mIsFront);
		//	return true;
		//}
		
		//// Win32 Api
		//RECT winRct{ static_cast<LONG>(mX), static_cast<LONG>(mY),
		//	static_cast<LONG>(mX + mFront->GetWidth()),
		//	static_cast<LONG>(mY + mFront->GetHeight()) };
		//POINT winPt{ x, y };
		//if (PtInRect(&winRct, winPt))
		//{
		//	Flip(!mIsFront);
		//	return true;
		//}

		// GDI+ version
		Gdiplus::Rect gpRct{ static_cast<int>(mX),
			static_cast<int>(mY),
			static_cast<int>(mFront->GetWidth()),
			static_cast<int>(mFront->GetHeight()) };
		if (gpRct.Contains(x, y))
		{
			Flip(!mIsFront);
			return true;
		}

		return false;
	}
	void Card::Flip(bool isFront)
	{
		mIsFront = isFront;
		Invalidate();
	}
	void Card::Draw(Gdiplus::Graphics& graphics)
	{
		if (mIsFront)
		{
			graphics.DrawImage(mFront.get(), static_cast<int>(mX), static_cast<int>(mY));
		}
		else
		{
			graphics.DrawImage(mBack.get(), static_cast<int>(mX), static_cast<int>(mY));
		}
	}
	void Card::Invalidate()
	{
		RECT rect{ static_cast<LONG>(mX), static_cast<LONG>(mY),
			static_cast<LONG>(mX + mFront->GetWidth()),
			static_cast<LONG>(mY + mFront->GetHeight()) };
		InvalidateRect(mHwnd, &rect, false);
	}
}


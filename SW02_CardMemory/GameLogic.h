#pragma once
#include <list>
#include "Card.h"

namespace solitaire
{
	class GameLogic
	{
		const int BOARD_ROW{ 5 }, BOARD_COLUM{ 8 };
		const Gdiplus::RectF COUNT_RECT{ 885.0f,60.0f, 120.f, 30.0f };

	private:
		std::unique_ptr<Gdiplus::Image> mBackground;
		std::list<Card> mDeck;
		int mFlipCount{};

		Card* mpSelectedCard{};

		HWND mHwnd;
		
	private:
		void CreateCards();

	public:
		void Initialize(HWND hwnd);
		void Release();
		void Draw(Gdiplus::Graphics& graphics);
		void OnClick(int x, int y);
	};
}


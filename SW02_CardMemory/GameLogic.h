#pragma once
#include <list>
#include "Card.h"

namespace solitaire
{
	class GameLogic
	{
		const int BOARD_ROW{ 2 }, BOARD_COLUMN{ 5 }, MAX_ROUND{ 3 };
		const Gdiplus::RectF COUNT_RECT{ 885.0f, 60.0f, 130.f, 30.0f };

	private:
		std::unique_ptr<Gdiplus::Image> mBackground;
		std::list<Card> mDeck;
		int mStageCount{};
		int mFlipCount{};

		Card* mpSelectedCard{};

		HWND mHwnd;
		
	private:
		void CreateCards();
		void GameClear();

	public:
		void Initialize(HWND hwnd);
		void Release();
		void Draw(Gdiplus::Graphics& graphics);
		void OnClick(int x, int y);
	};
}


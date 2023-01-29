#pragma once
#include <list>
#include "Card.h"

namespace solitaire
{
	class GameLogic
	{
		const int BOARD_ROW{ 2 }, BOARD_COLUMN{ 5 }, MAX_ROUND{ 3 };
		const Gdiplus::RectF COUNT_RECT_P1{ 890.0f, 100.0f, 144.0f, 70.0f };
		const Gdiplus::RectF COUNT_RECT_P2{ 890.0f, 200.0f, 144.0f, 70.0f };
		
	private:
		std::unique_ptr<Gdiplus::Image> mBackground;
		std::list<Card> mDeck;
		int mStageCount{};
		int mFlipCount{};
		bool mIsPlayer1{ true };

		Card* mpSelectedCard{};
		HWND mHwnd;
		RECT mClientRect;
		
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


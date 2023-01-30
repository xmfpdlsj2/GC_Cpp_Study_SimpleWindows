#pragma once
#include <list>
#include "Card.h"

namespace solitaire
{
	class GameLogic
	{
		const int BOARD_ROW{ 2 }, BOARD_COLUMN{ 5 }, MAX_ROUND{ 3 };
		const Gdiplus::RectF COUNT_RECT_P1{ 890.0f, 100.0f, 144.0f, 100.0f };
		const Gdiplus::RectF COUNT_RECT_P2{ 890.0f, 250.0f, 144.0f, 100.0f };
		
	private:
		std::unique_ptr<Gdiplus::Image> mBackground;
		std::list<Card> mDeck;
		int mStageCount{};
		int mFlipCountP1{}, mMatchCountP1{};
		int mFlipCountP2{}, mMatchCountP2{};
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
		void DrawStringRect(Gdiplus::Graphics& graphics);
		void OnClick(int x, int y);
	};
}


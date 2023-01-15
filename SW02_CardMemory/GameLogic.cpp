#include <vector>
#include <random>	
#include "GameLogic.h"

namespace solitaire
{
	void GameLogic::CreateCards()
	{
		// 40장 - ABC 각각 짝수
		std::vector<Type> types;
		while (types.size() < static_cast<size_t>(BOARD_ROW * BOARD_COLUMN))
		{
			int mod = types.size() % 6;
			switch (mod)
			{
				case 0:
					types.push_back(Type::Bear);
					types.push_back(Type::Bear);
					break;
				case 2:
					types.push_back(Type::Wolf);
					types.push_back(Type::Wolf);
					break;
				case 4:
					types.push_back(Type::Dragon);
					types.push_back(Type::Dragon);
					break;
				default:
					OutputDebugString(L"카드 생성에 문제가 있음!");
					break;
			}
		}

		// 그리고 shuffle
		std::random_device rd;
		std::mt19937 gen(rd());
		std::shuffle(types.begin(), types.end(), gen);

		int posX{ 15 }, posY{ 10 };
		int index{};
		for (int x = 0; x < BOARD_COLUMN; x++)
		{
			posY = 10;
			for (int y = 0; y < BOARD_ROW; y++)
			{
				mDeck.push_back(Card(types[index++], posX, posY, mHwnd, index));
				posY += 150;
			}
			posX += 110;
		}
	}
	void GameLogic::GameClear()
	{
		int msgBoxId = MessageBox(NULL,
			L"축하합니다! 다음 라운드를 플레이 하시겠습니까? 아니라면 종료합니다.",
			L"Game Clear!",
			MB_YESNO);

		switch (msgBoxId)
		{
			case IDYES:
			{
				CreateCards();
				RECT rct = {};
				GetClientRect(mHwnd, &rct);
				InvalidateRect(mHwnd, &rct, false);

				PostMessageA(mHwnd, WM_PAINT, 0, 0);
			}
				break;
			case IDNO:
			default:
				PostMessageA(mHwnd, WM_CLOSE, 0, 0);
				break;
		}
	}
	void GameLogic::Initialize(HWND hwnd)
	{
		mHwnd = hwnd;
		mBackground = std::make_unique<Gdiplus::Image>(L"Data/bg_blank.png");
		CreateCards();
	}
	void GameLogic::Release()
	{
		mDeck.clear();
		mBackground.reset();
	}
	void GameLogic::Draw(Gdiplus::Graphics& graphics)
	{
		graphics.DrawImage(mBackground.get(), 0, 0, 
			mBackground->GetWidth(), mBackground->GetHeight());

		for (auto& card : mDeck)
		{
			card.Draw(graphics);
		}
		Gdiplus::PointF pos(895.0f, 20.0f);
		Gdiplus::SolidBrush brush(Gdiplus::Color(255, 79, 64));
		Gdiplus::Font font(L"맑은 고딕", 20);
		graphics.DrawString(L"클릭수: ", -1, &font, pos, &brush);

		Gdiplus::StringFormat format;
		format.SetAlignment(Gdiplus::StringAlignmentCenter);
		format.SetLineAlignment(Gdiplus::StringAlignmentCenter);

		graphics.DrawString(std::to_wstring(mFlipCount).c_str(), -1,
			&font, COUNT_RECT, &format, &brush);

	}
	void GameLogic::OnClick(int x, int y)
	{
		Card* pCard{};
		for (auto& card : mDeck)
		{
			if (card.CheckClicked(x, y))
			{
				pCard = &card;
				break;
			}
		}

		if (pCard)
		{
			mFlipCount++;
			RECT rct = { static_cast<LONG>(COUNT_RECT.GetLeft()),
				static_cast<LONG>(COUNT_RECT.GetTop()),
				static_cast<LONG>(COUNT_RECT.GetRight()),
				static_cast<LONG>(COUNT_RECT.GetBottom()) };
			InvalidateRect(mHwnd, &rct, false);
			
			if (mpSelectedCard == nullptr)
			{
				mpSelectedCard = pCard;
			}
			else if (mpSelectedCard == pCard)
			{
				mpSelectedCard = nullptr;
			}
			else
			{
				if (pCard->GetType() == mpSelectedCard->GetType())
				{
					UpdateWindow(mHwnd);
					Sleep(500);

					pCard->Invalidate();
					mpSelectedCard->Invalidate();

					mDeck.remove_if([&](Card& card)->bool
						{
							return card.GetIndex() == mpSelectedCard->GetIndex();
						});
					mDeck.remove_if([&](Card& card)->bool
						{
							return card.GetIndex() == pCard->GetIndex();
						});

					mpSelectedCard = nullptr;

					if (mDeck.empty())
					{
						mFlipCount = 0;
						GameClear();
					}
				}
				else
				{
					UpdateWindow(mHwnd);
					Sleep(500);

					pCard->Flip(false);
					mpSelectedCard->Flip(false);

					mpSelectedCard = nullptr;
				}
			}
		}
	}
}
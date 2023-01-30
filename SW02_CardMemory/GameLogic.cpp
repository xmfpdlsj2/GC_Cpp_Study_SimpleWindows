#include <vector>
#include <random>	
#include "GameLogic.h"

namespace solitaire
{
	void GameLogic::CreateCards()
	{
		int row{ BOARD_ROW + mStageCount }, column{ BOARD_COLUMN + mStageCount };
		std::vector<Type> types;
		while (types.size() < static_cast<size_t>(row * column))
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
		for (int x = 0; x < column; x++)
		{
			posY = 10;
			for (int y = 0; y < row; y++)
			{
				mDeck.push_back(Card(types[index++], posX, posY, mHwnd, index));
				posY += 150;
			}
			posX += 110;
		}
	}
	void GameLogic::GameClear()
	{
		int p{};
		std::wostringstream oss;

		if (mStageCount == MAX_ROUND)
		{
			if (mMatchCountP1 == mMatchCountP2)
			{
				if (mFlipCountP1 == mFlipCountP2)
				{
					oss << L"대박! 무승부 입니다!!!!!";
				}
				else
				{
					p = mFlipCountP1 < mFlipCountP2 ? 2 : 1;
					oss << L"와우! 카드매칭 수는 같습니다!\n" << 
						L"하지만 FlipCount 차이로, 최종 승자는 Player " << p << L" 입니다!!";
				}
			}
			else
			{
				p = mMatchCountP1 < mMatchCountP2 ? 2 : 1;
				oss << L"축하합니다! 카드매칭 차이로\n 최종 승자는 Player " << p << L" 입니다!!";
			}

			MessageBox(NULL,
				oss.str().c_str(),
				L"Winer winer chicken dinner!!",
				MB_OK);
			PostMessageA(mHwnd, WM_CLOSE, 0, 0);
			return;
		}

		p = mIsPlayer1 ? 1 : 2;
		oss << L"이어서 다음 라운드를 플레이 하시겠습니까? 아니라면 종료합니다.\n\n선은 계속해서: "
			<< "Player " << p;
		int msgBoxId = MessageBox(NULL, oss.str().c_str(), L"Game Clear!", MB_YESNO);

		switch (msgBoxId)
		{
			case IDYES:
			{
				mStageCount++;
				CreateCards();

				InvalidateRect(mHwnd, &mClientRect, false);

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
		GetClientRect(mHwnd, &mClientRect);
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
			static_cast<INT>(mClientRect.right), static_cast<INT>(mClientRect.bottom));

		for (auto& card : mDeck)
		{
			card.Draw(graphics);
		}
		
		DrawStringRect(graphics);
	}
	void GameLogic::DrawStringRect(Gdiplus::Graphics& graphics)
	{
		Gdiplus::SolidBrush brush(Gdiplus::Color(200, 200, 200));
		Gdiplus::Font font(L"맑은 고딕", 20);
		Gdiplus::StringFormat format;
		format.SetAlignment(Gdiplus::StringAlignmentNear);
		format.SetLineAlignment(Gdiplus::StringAlignmentCenter);

		brush.SetColor(Gdiplus::Color().LightYellow);
		std::wostringstream ossS{};
		ossS << "Stage " << mStageCount + 1;
		Gdiplus::PointF pos{ COUNT_RECT_P1.X, 20 };
		graphics.DrawString(ossS.str().c_str(), -1, &font, pos, &brush);

		Gdiplus::SolidBrush brushP2(Gdiplus::Color(200, 200, 200));
		if (mIsPlayer1)
		{
			brushP2.SetColor(Gdiplus::Color(200, 200, 200));
			brush.SetColor(Gdiplus::Color().GreenYellow);
		}
		else
		{
			brush.SetColor(Gdiplus::Color(200, 200, 200));
			brushP2.SetColor(Gdiplus::Color().GreenYellow);
		}
		
		std::wostringstream oss{};
		oss << "Player1" << L"\n클릭수:" << mFlipCountP1 << L"\n매칭수:" << mMatchCountP1;
		graphics.DrawString(oss.str().c_str(), -1, &font, COUNT_RECT_P1, &format, &brush);

		std::wostringstream oss2{};
		oss2 << "Player2" << L"\n클릭수:" << mFlipCountP2 << L"\n매칭수:" << mMatchCountP2;
		graphics.DrawString(oss2.str().c_str(), -1, &font, COUNT_RECT_P2, &format, &brushP2);
		

		brush.SetColor(Gdiplus::Color().RosyBrown);
		pos = (Gdiplus::PointF{ COUNT_RECT_P1.X, COUNT_RECT_P2.Y + 150 });
		graphics.DrawString(L"총 4 Stage", -1, &font, pos, &brush);
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
			if (mIsPlayer1)
			{
				mFlipCountP1++;
			}
			else
			{ 
				mFlipCountP2++;
			}

			RECT rct = { static_cast<LONG>(COUNT_RECT_P1.GetLeft()),
				static_cast<LONG>(COUNT_RECT_P1.GetTop()),
				static_cast<LONG>(COUNT_RECT_P1.GetRight()),
				static_cast<LONG>(COUNT_RECT_P2.GetBottom()) };
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

					mIsPlayer1 ? mMatchCountP1++ : mMatchCountP2++;
					InvalidateRect(mHwnd, &rct, false);
					if (mDeck.empty())
					{
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

					mIsPlayer1 = !mIsPlayer1;
					std::wostringstream oss;
					int p = mIsPlayer1 ? 1 :2;
					oss << L"플레이어 변경: " << "Player " << p;
					MessageBox(NULL,
						oss.str().c_str(),
						L"Player Change!",
						MB_OK);
				}
			}
		}
	}
}
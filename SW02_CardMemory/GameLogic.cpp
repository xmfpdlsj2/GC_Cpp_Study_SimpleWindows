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
					OutputDebugString(L"ī�� ������ ������ ����!");
					break;
			}
		}

		// �׸��� shuffle
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
		if (mStageCount == MAX_ROUND)
		{
			MessageBox(NULL,
				L"�����մϴ�! ������ ���带 Ŭ�����߽��ϴ�.",
				L"Game Clear!",
				MB_OK);
			PostMessageA(mHwnd, WM_CLOSE, 0, 0);
			return;
		}

		int msgBoxId = MessageBox(NULL,
			L"�����մϴ�! ���� ���带 �÷��� �Ͻðڽ��ϱ�? �ƴ϶�� �����մϴ�.",
			L"Game Clear!",
			MB_YESNO);

		switch (msgBoxId)
		{
			case IDYES:
			{
				mStageCount++;
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
		Gdiplus::PointF pos(885.0f, 16.0f);
		Gdiplus::SolidBrush brush(Gdiplus::Color(255, 79, 64));
		Gdiplus::Font font(L"���� ���", 20);
		std::wostringstream oss{};
		oss << "Stage: " << mStageCount << L"\nŬ����: ";
		graphics.DrawString(oss.str().c_str(), -1, &font, pos, &brush);

		Gdiplus::StringFormat format;
		format.SetAlignment(Gdiplus::StringAlignmentFar);
		format.SetLineAlignment(Gdiplus::StringAlignmentFar);

		graphics.DrawString(std::to_wstring(mFlipCount).c_str(), -1,
			&font, COUNT_RECT_P1, &format, &brush);
		

		Gdiplus::PointF pos2(885.0f, 106.0f);
		Gdiplus::SolidBrush brush2(Gdiplus::Color(255, 255, 64));
		Gdiplus::Font font2(L"���� ���", 20);
		std::wostringstream oss2{};
		oss2 << "Stage: " << mStageCount << L"\nŬ����: ";
		graphics.DrawString(oss2.str().c_str(), -1, &font2, pos2, &brush2);

		Gdiplus::StringFormat format2;
		format2.SetAlignment(Gdiplus::StringAlignmentFar);
		format2.SetLineAlignment(Gdiplus::StringAlignmentFar);
		graphics.DrawString(std::to_wstring(mFlipCount).c_str(), -1,
			&font, COUNT_RECT_P2, &format2, &brush);
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
			RECT rct = { static_cast<LONG>(COUNT_RECT_P1.GetLeft()),
				static_cast<LONG>(COUNT_RECT_P1.GetTop()),
				static_cast<LONG>(COUNT_RECT_P1.GetRight()),
				static_cast<LONG>(COUNT_RECT_P1.GetBottom()) };
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
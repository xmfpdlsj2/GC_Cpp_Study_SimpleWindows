#pragma once
#include <Windows.h>
#include <gdiplus.h>
#include <string>

#include <memory>

#pragma comment (lib, "Gdiplus.lib")

namespace solitaire
{
	enum class Type
	{
		Wolf,
		Dragon,
		Bear
	};

	class Card
	{
	private:
		std::unique_ptr<Gdiplus::Image> mBack;
		std::unique_ptr<Gdiplus::Image> mFront; 

		Type mType;
		unsigned int mX;
		unsigned int mY;
		bool mIsFront;

		HWND mHwnd;
		int mIndex;

	public:
		Card(Type type, unsigned int x, unsigned int y, HWND hwnd, int index);

		bool CheckClicked(unsigned int x, unsigned int y);
		void Flip(bool isFront);
		void Draw(Gdiplus::Graphics& graphics);
		void Invalidate();

		Type GetType() const { return mType; }
		int GetIndex() const { return mIndex; }
	};
}


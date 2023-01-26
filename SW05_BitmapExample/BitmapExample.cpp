#include "BitmapExample.h"

void BitmapExample::Initialize(HINSTANCE hInstance, LPCWSTR title, UINT width, UINT height)
{
	D2DFramework::Initialize(hInstance, title, width, height);

	mspBackBuffer = std::make_unique<UINT8[]>(
		BITMAP_WIDTH * BITMAP_HEIGHT * BITMAP_BYTECOUNT
		);
	
	HRESULT hr = mspRenderTarget->CreateBitmap(
		D2D1::SizeU(BITMAP_WIDTH, BITMAP_HEIGHT),
		D2D1::BitmapProperties(
			D2D1::PixelFormat(DXGI_FORMAT_R8G8B8A8_UNORM, D2D1_ALPHA_MODE_IGNORE)
		),
		mspFrameBitmap.GetAddressOf()
	);

	ThrowIfFailed(hr);
}

void BitmapExample::Render()
{
	mspRenderTarget->BeginDraw();
	mspRenderTarget->Clear(D2D1::ColorF(0.0f, 0.2f, 0.4f));

	ClearBuffer(D2D1::ColorF::Black);

	//DrawPixelToBuffer(100, 100, D2D1::ColorF::Black);
	//FillRectToBuffer(0, 0, 100, 100, D2D1::ColorF::ForestGreen);
	//FillRectToBuffer(50, 50, 100, 100, D2D1::ColorF(D2D1::ColorF::CornflowerBlue, 0.5f));

	//DrawCircleToBuffer(200, 500, 100, D2D1::ColorF::Red);
	DrawLineToBuffer(10, 120, 10, 310, D2D1::ColorF::Red);
	DrawLineToBuffer(100, 235, 100, 110, D2D1::ColorF::Red);
	DrawLineToBuffer(210, 110, 10, 110, D2D1::ColorF::Red);
	DrawLineToBuffer(120, 210, 310, 210, D2D1::ColorF::Red);

	PresentBuffer();
	mspRenderTarget->DrawBitmap(mspFrameBitmap.Get());

	mspRenderTarget->EndDraw();
}

void BitmapExample::PresentBuffer()
{
	mspFrameBitmap->CopyFromMemory(
		nullptr, &mspBackBuffer[0], BITMAP_WIDTH * BITMAP_BYTECOUNT
	);
}

void BitmapExample::ClearBuffer(D2D1::ColorF color)
{
	auto pCurrent = &mspBackBuffer[0];
	for (int count = 0; count < BITMAP_WIDTH * BITMAP_HEIGHT; ++count)
	{
		*pCurrent = static_cast<UINT8>(color.r * 255);
		*(pCurrent + 1) = static_cast<UINT8>(color.g * 255);
		*(pCurrent + 2) = static_cast<UINT8>(color.b * 255);
		*(pCurrent + 3) = static_cast<UINT8>(color.a * 255);
		
		pCurrent += BITMAP_BYTECOUNT;
	}
}

void BitmapExample::DrawPixelToBuffer(int x, int y, D2D1::ColorF color)
{
	int pitch = BITMAP_WIDTH * BITMAP_BYTECOUNT;
	int index = x * BITMAP_BYTECOUNT + y * pitch;

	float inverse = 1.0f - color.a;

	mspBackBuffer[index] = static_cast<UINT8>(mspBackBuffer[index] * inverse + color.r * 255 * color.a);
	mspBackBuffer[index + 1] = static_cast<UINT8>(mspBackBuffer[index + 1] * inverse + color.g * 255 * color.a);
	mspBackBuffer[index + 2] = static_cast<UINT8>(mspBackBuffer[index + 2] * inverse + color.b * 255 * color.a);
	mspBackBuffer[index + 3] = 255;
}

void BitmapExample::FillRectToBuffer(int left, int top, int width, int height, D2D1::ColorF color)
{
	for (int i = 0; i < width; i++)
	{
		for (int j = 0; j < height; j++)
		{
			DrawPixelToBuffer(left + i, top + j, color);
		}
	}
}

void BitmapExample::DrawCircleToBuffer(int x, int y, int r, D2D1::ColorF color)
{
	int left = (x - r);
	int top = (y - r);
	
	if (left < 0)
	{
		left = 0;
	}
	if (top < 0)
	{
		top = 0;
	}

	//// 원을 채우는 방식. 원의 방정식
	//for (int i = 0; i < (2 * r); i++)
	//{
	//	for (int j = 0; j < (2 * r); j++)
	//	{
	//		int a = left + i;
	//		int b = top + j;
	//		if ((std::pow((a - x), 2) + std::pow((b - y), 2)) <= std::pow(r, 2))
	//		{
	//			DrawPixelToBuffer(a, b, color);
	//		}
	//	}
	//}

	//// 원만 그리는 방식. 원의 방정식
	//for (int i = 0; i < (2 * r); i++)
	//{
	//	int aX = left + i;
	//	double yPos = std::sqrt(std::pow(r, 2) - std::pow((aX - x), 2)) + y;
	//	int aY = static_cast<int>(std::round(yPos)); // 반올림

	//	DrawPixelToBuffer(aX, aY, color);
	//	DrawPixelToBuffer(aX, y - (aY - y), color); // 좌표상 대칭되는 점

	//	for (int j = 0; j < (2 * r); j++)
	//	{
	//		int bY = top + j;
	//		double xPos = std::sqrt(std::pow(r, 2) - std::pow((bY - y), 2)) + x;
	//		int bX = static_cast<int>(std::round(xPos));

	//		DrawPixelToBuffer(bX, bY,  color);
	//		DrawPixelToBuffer(x - (bX - x), bY, color);
	//	}
	//}

	// 삼각수를 사용한 방식
	int degree{ 360 };
	float PI{ 3.141592 };

	for (int k = 0; k < degree; k++)
	{
		auto rX = std::cos(k * (PI / 180)) * r;
		auto rY = std::sin(k * (PI / 180)) * r;
		int a = static_cast<int>(std::round(rX));
		int b = static_cast<int>(std::round(rY));
		DrawPixelToBuffer(a + x, b + y, color);
	}
}

void BitmapExample::DrawLineToBuffer(int x1, int y1, int x2, int y2, D2D1::ColorF color)
{
	int xL{}, xR{}, yL{}, yR{};

	if (x1 < x2)
	{
		xL = x1;
		xR = x2;
		yL = y1;
		yR = y2;
	}
	else if (x1 > x2)
	{
		xL = x2;
		xR = x1;
		yL = y2;
		yR = y1;
	}
	else
	{
		xL = x1;
		xR = x2;
		yL = y1 < y2 ? y1 : y2;
		yR = y1 < y2 ? y2 : y1;
	}

	float t = static_cast<float>(yR - yL) / (xR - xL);

	for (int i = 0; i < (xR - xL); i++)
	{
		int y = static_cast<int>(std::round(t * i + yL));
		DrawPixelToBuffer((xL + i), y, color);
	}

	for (int j = 0; j < (yR - yL); j++)
	{
		int x = static_cast<int>(std::round(j / t + xL));
		DrawPixelToBuffer(x, (yL + j), color);
	}
}

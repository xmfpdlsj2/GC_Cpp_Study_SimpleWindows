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

	ClearBuffer(D2D1::ColorF::HotPink);

	//DrawPixelToBuffer(100, 100, D2D1::ColorF::Black);
	//FillRectToBuffer(0, 0, 100, 100, D2D1::ColorF::ForestGreen);
	//FillRectToBuffer(50, 50, 100, 100, D2D1::ColorF(D2D1::ColorF::CornflowerBlue, 0.5f));

	DrawCircleToBuffer(200, 500, 100, D2D1::ColorF::CornflowerBlue);

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

	for (int i = 0; i < (2 * r); i++)
	{
		for (int j = 0; j < (2 * r); j++)
		{
			int a = left + i;
			int b = top + j;
			if ((std::pow((a - x), 2) + std::pow((b - y), 2)) <= std::pow(r, 2))
			{
				DrawPixelToBuffer(left + i, top + j, color);
			}
		}
	}
}

void BitmapExample::DrawLineToBuffer(int x1, int y1, int x2, int y2, D2D1::ColorF color)
{

}

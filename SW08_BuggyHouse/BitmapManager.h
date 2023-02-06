#pragma once

#include <d2d1.h>
#include <wincodec.h>
#include <wrl/client.h>	
#include <map>
#include <string>
#include "ComException.h"

class BitmapManager final
{
public:
	static BitmapManager& Instance()
	{
		static BitmapManager instance;
		return instance;
	}
private:
	BitmapManager() = default;
	BitmapManager(const BitmapManager&) = delete;
	void operator = (const BitmapManager&) = delete;
	// 이동 관련은 추후에 해보자.

private:
	ID2D1HwndRenderTarget *mpRenderTarget{};
	Microsoft::WRL::ComPtr<IWICImagingFactory> mspWICFactory{};
	std::map<std::wstring, Microsoft::WRL::ComPtr<ID2D1Bitmap>> mBitmaps{};

	HRESULT LoadWICBitmap(std::wstring filename, ID2D1Bitmap** ppBitmap);

public:
	HRESULT Initialize(ID2D1HwndRenderTarget* pRT);
	void Release();

	ID2D1Bitmap* LoadBitmaps(std::wstring filename);
};


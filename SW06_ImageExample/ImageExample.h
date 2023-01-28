#pragma once
#include <wincodec.h>
#include <vector>
#include "D2DFramework.h"


class ImageExample : public D2DFramework
{
	Microsoft::WRL::ComPtr<IWICImagingFactory> mspWICFactory;
	Microsoft::WRL::ComPtr<ID2D1Bitmap> mspBitmap_bg00;
	Microsoft::WRL::ComPtr<ID2D1Bitmap> mspBitmap_bg01;
	Microsoft::WRL::ComPtr<ID2D1Bitmap> mspBM_moster;
	Microsoft::WRL::ComPtr<ID2D1Bitmap> mspBM_charc;

private:
	D2D1_RECT_F mBgRect;
	std::vector<D2D1_RECT_F> mMonsRects;
	D2D1_RECT_F mCharcRect;

public:
	virtual void Initialize(HINSTANCE hInstance, LPCWSTR title = L"BMP File Example",
		UINT width = 1024, UINT height = 768) override;
	void Render() override;
	void Release() override;

public:
	HRESULT LoadBMP(LPCWSTR filename);
	HRESULT LoadWICImage(LPCWSTR filename, ID2D1Bitmap** bitamp);

};


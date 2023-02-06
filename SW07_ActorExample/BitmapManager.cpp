#include "BitmapManager.h"

#pragma comment (lib, "WindowsCodecs.lib")

using namespace std;
using namespace Microsoft::WRL;

HRESULT BitmapManager::LoadWICBitmap(std::wstring filename, ID2D1Bitmap** ppBitmap)
{
	HRESULT hr;

	// 1. ���ڴ� ���� (Decoder)
	Microsoft::WRL::ComPtr<IWICBitmapDecoder> decoder;
	hr = mspWICFactory->CreateDecoderFromFilename(
		filename.c_str(),
		nullptr,
		GENERIC_READ,
		WICDecodeMetadataCacheOnLoad,
		decoder.GetAddressOf()
	);
	ThrowIfFailed(hr);

	// 2. ���ڴ����� ������ ȹ�� (BMP���� - 0�� ������)
	Microsoft::WRL::ComPtr<IWICBitmapFrameDecode> frame;
	hr = decoder->GetFrame(0, frame.GetAddressOf());
	ThrowIfFailed(hr);

	// 3. �����ͷ� �����ӿ��� �ȼ� �迭�� ���ؿ;� ��
	Microsoft::WRL::ComPtr<IWICFormatConverter> converter;
	hr = mspWICFactory->CreateFormatConverter(converter.GetAddressOf());
	ThrowIfFailed(hr);

	hr = converter->Initialize(
		frame.Get(),
		GUID_WICPixelFormat32bppPBGRA,
		WICBitmapDitherTypeNone,
		nullptr,
		0,
		WICBitmapPaletteTypeCustom
	);
	ThrowIfFailed(hr);

	// 4.�ȼ��迭�� ��Ʈ�� ����
	hr = mpRenderTarget->CreateBitmapFromWicBitmap(
		converter.Get(),
		ppBitmap
	);
	ThrowIfFailed(hr);

	return S_OK;
}

HRESULT BitmapManager::Initialize(ID2D1HwndRenderTarget* pRT)
{
	if (pRT == nullptr)
	{
		return E_FAIL;
	}

	mpRenderTarget = pRT;

	HRESULT hr = CoCreateInstance(
		CLSID_WICImagingFactory,
		nullptr,
		CLSCTX_INPROC_SERVER,
		IID_PPV_ARGS(mspWICFactory.GetAddressOf())
	);
	ThrowIfFailed(hr);

	return hr;
}

void BitmapManager::Release()
{
	for (auto& e : mBitmaps)
	{
		e.second.Reset();
	}
	mBitmaps.clear();

	mspWICFactory.Reset();
}

ID2D1Bitmap* BitmapManager::LoadBitmaps(std::wstring filename)
{
	// ��� �ڵ�
	if (mspWICFactory == nullptr)
	{
		return nullptr;
	}

	auto result = mBitmaps.insert({ filename, nullptr });
	if (result.second == true)
	{
		auto spBitmap = ComPtr<ID2D1Bitmap>();
		LoadWICBitmap(filename, spBitmap.GetAddressOf());
		result.first->second = spBitmap;
	}

    return result.first->second.Get();
}

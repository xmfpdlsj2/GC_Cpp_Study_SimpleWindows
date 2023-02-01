#include "Actor.h"

Actor::Actor(D2DFramework* pFramework, std::wstring filename) :
	Actor(pFramework, filename, 0.0f, 0.0f, 1.0f)
{
}

Actor::Actor(D2DFramework* pFramework, std::wstring filename, float x, float y, float opacity) :
	mpFrameWork{ pFramework }, mX{ x }, mY{ y }, mOpacity{ opacity }
{
	LoadWICImage(filename.c_str());
}

Actor::~Actor()
{
	mspBitmap.Reset();
}

HRESULT Actor::LoadWICImage(LPCWSTR filename)
{
	HRESULT hr;

	auto pWICFactory = mpFrameWork->GetWICFactory();

	// 1. 디코더 생성 (Decoder)
	Microsoft::WRL::ComPtr<IWICBitmapDecoder> decoder;
	hr = pWICFactory->CreateDecoderFromFilename(
		filename,
		nullptr,
		GENERIC_READ,
		WICDecodeMetadataCacheOnLoad,
		decoder.GetAddressOf()
	);
	ThrowIfFailed(hr);

	// 2. 디코더에서 프레임 획득 (BMP파일 - 0번 프레임)
	Microsoft::WRL::ComPtr<IWICBitmapFrameDecode> frame;
	hr = decoder->GetFrame(0, frame.GetAddressOf());
	ThrowIfFailed(hr);

	// 3. 컨버터로 프레임에서 픽셀 배열을 구해와야 함
	Microsoft::WRL::ComPtr<IWICFormatConverter> converter;
	hr = pWICFactory->CreateFormatConverter(converter.GetAddressOf());
	ThrowIfFailed(hr);

	converter->Initialize(
		frame.Get(),
		GUID_WICPixelFormat32bppPBGRA,
		WICBitmapDitherTypeNone,
		nullptr,
		0,
		WICBitmapPaletteTypeCustom
	);
	ThrowIfFailed(hr);

	// 4.픽셀배열로 비트맵 생성
	auto pRenderTarget = mpFrameWork->GetRenderTarget();
	hr = pRenderTarget->CreateBitmapFromWicBitmap(
		converter.Get(),
		mspBitmap.GetAddressOf()
	);
	ThrowIfFailed(hr);

	return S_OK;
}

void Actor::Draw(float x, float y, float opacity)
{
	auto pRenderTarget = mpFrameWork->GetRenderTarget();
	auto size = mspBitmap->GetPixelSize();

	D2D_RECT_F rect{ x, y, x + size.width, y + size.height };

	pRenderTarget->DrawBitmap(mspBitmap.Get(), rect, opacity);
}

void Actor::Draw()
{
	Draw(mX, mY, mOpacity);
}

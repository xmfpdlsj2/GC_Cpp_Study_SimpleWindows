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

	// 1. ���ڴ� ���� (Decoder)
	Microsoft::WRL::ComPtr<IWICBitmapDecoder> decoder;
	hr = pWICFactory->CreateDecoderFromFilename(
		filename,
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

	// 4.�ȼ��迭�� ��Ʈ�� ����
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

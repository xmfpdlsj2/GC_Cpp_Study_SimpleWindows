#include <fstream>
#include <random>
#include "ImageExample.h"
#include <iostream>

#pragma comment(lib, "WindowsCodecs.lib")

void ImageExample::Initialize(HINSTANCE hInstance, LPCWSTR title, UINT width, UINT height)
{
	CoInitialize(nullptr);

	HRESULT hr = CoCreateInstance(
		CLSID_WICImagingFactory,
		nullptr,
		CLSCTX_INPROC_SERVER,
		IID_PPV_ARGS(mspWICFactory.GetAddressOf())
		);
	ThrowIfFailed(hr);

	D2DFramework::Initialize(hInstance, title, width, height);

	//LoadBMP(L"Data/32.bmp");
	LoadWICImage(L"Data/esl_t_i_bg_sky.png", mspBitmap_bg00.GetAddressOf());
	LoadWICImage(L"Data/esl_t_i_bg_world02.png", mspBitmap_bg01.GetAddressOf());
	LoadWICImage(L"Data/08_dragon__.png", mspBM_moster.GetAddressOf());
	LoadWICImage(L"Data/01_ecco_1.png", mspBM_charc.GetAddressOf());

	D2D1_SIZE_F bgSize = mspRenderTarget->GetSize();
	mBgRect = D2D1::RectF(0.0f, 0.0f, bgSize.width, bgSize.height);

	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<> distX(1, 10);
	std::uniform_int_distribution<> distY(1, 5);

	D2D1_SIZE_F monsterSize = mspBM_moster->GetSize();
	float start{ 70.0f }, ratio{ 0.1f };
	float posW{ monsterSize.width * ratio }, posY{ monsterSize.height * ratio };
	int monsterCount{ 10 };
	for (int i = 0; i < monsterCount; i++)
	{
		float x = start * distX(gen);
		float y = start * distY(gen);
		mMonsRects.push_back(D2D1::RectF(x, y, x + posW, y + posY));
	}

	D2D1_SIZE_F charcSize = mspBM_charc->GetSize();
	start = 550.0f;
	mCharcRect = D2D1::RectF(start, start, start + charcSize.width * 0.2f, start + charcSize.height * 0.2f);
}

void ImageExample::Render()
{
	mspRenderTarget->BeginDraw();
	mspRenderTarget->Clear(D2D1::ColorF(0.0f, 0.2f, 0.4f));
	
	mspRenderTarget->DrawBitmap(mspBitmap_bg00.Get(), mBgRect);
	mspRenderTarget->DrawBitmap(mspBitmap_bg01.Get(), mBgRect);

	for (auto& monsRect : mMonsRects)
	{
		mspRenderTarget->DrawBitmap(mspBM_moster.Get(), monsRect);
	}

	mspRenderTarget->DrawBitmap(mspBM_charc.Get(), mCharcRect);

	mspRenderTarget->EndDraw();
}

void ImageExample::Release()
{
	mspBM_charc.Reset();
	mspBM_moster.Reset();
	mspBitmap_bg01.Reset();
	mspBitmap_bg00.Reset();

	D2DFramework::Release();
	mspWICFactory.Reset();
	CoUninitialize();
}

HRESULT ImageExample::LoadBMP(LPCWSTR filename)
{
	// 1. 파일 열기
	std::ifstream file;
	file.open(filename, std::ios::binary);

	// 2. BITMAPFILEHEADER 구조체
	// POD : Plain Old Data: ex) 구조체
	// 데이터를 한방에 전송
	BITMAPFILEHEADER bfh;
	file.read(reinterpret_cast<char*>(&bfh), sizeof(BITMAPFILEHEADER));

	if (bfh.bfType != 0x4D42) // 리틀엔디언 맞춰서 순서를 바꿔줌
	{
		return E_FAIL;
	}

	// 3. BITMAPINFOHEADER 구조체
	BITMAPINFOHEADER bih;
	file.read(reinterpret_cast<char*>(&bih), sizeof(BITMAPINFOHEADER));

	if (bih.biBitCount != 32)
	{
		return E_FAIL;
	}

	// 4. 실제 픽셀 배열을 읽기
	file.seekg(bfh.bfOffBits);
	std::vector<char> pixels(bih.biSizeImage); // 이미지의 바이트 크기

	// 통째로 읽으면 거꾸로나옴
	//file.read(&pixels[0], bih.biSizeImage); 

	int pitch = bih.biWidth * (bih.biBitCount / 8);
	//// 한줄 (pitch) -> h(마지막)줄
	//// 한줄 -> h-1줄 
	//// ....
	//for (int i = bih.biHeight - 1; i >= 0; i--)
	//{
	//	file.read(&pixels[i * pitch], pitch);
	//}

	// 배경지우고 읽기
	int index {};
	for (int y = bih.biHeight - 1; y >= 0; y--)
	{
		int index = y * pitch;
		for (int x = 0; x < bih.biWidth; x++)
		{
			char r{}, g{}, b{}, a{};
			file.read(&b, 1);
			file.read(&g, 1);
			file.read(&r, 1);
			file.read(&a, 1);

			if (r == (char)30 && g == (char)199 && b == (char)250)
			{
				r = g = b = a = 0;
			}
			pixels[index++] = b;
			pixels[index++] = g;
			pixels[index++] = r;
			pixels[index++] = a;
		}
	}

	file.close();

	// 5. 비트맵 인터페이스 만들기
	HRESULT hr;

	hr = mspRenderTarget->CreateBitmap(
		D2D1::SizeU(bih.biWidth, bih.biHeight),
		D2D1::BitmapProperties(
			D2D1::PixelFormat(DXGI_FORMAT_B8G8R8A8_UNORM, D2D1_ALPHA_MODE_PREMULTIPLIED)
		),
		mspBitmap_bg01.GetAddressOf()
	);
	ThrowIfFailed(hr);

	// 6. 픽셀들 복사
	mspBitmap_bg01->CopyFromMemory(nullptr, &pixels[0], pitch);
	ThrowIfFailed(hr);

	return S_OK;
}

HRESULT ImageExample::LoadWICImage(LPCWSTR filename, ID2D1Bitmap** bitamp)
{
	HRESULT hr;
	// 1. 디코더 생성 (Decoder)
	Microsoft::WRL::ComPtr<IWICBitmapDecoder> decoder;
	hr = mspWICFactory->CreateDecoderFromFilename(
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
	hr = mspWICFactory->CreateFormatConverter(converter.GetAddressOf());
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
	hr = mspRenderTarget->CreateBitmapFromWicBitmap(
		converter.Get(),
		bitamp
	);
	ThrowIfFailed(hr);

	return S_OK;
}

#include "ActorExample.h"

void ActorExample::Initialize(HINSTANCE hInstance, LPCWSTR title, UINT width, UINT height)
{
	D2DFramework::Initialize(hInstance, title, width, height);

	mspBackground = std::make_unique<Actor>(this, L"Data/back1.png");
	mspBug = std::make_unique<Actor>(this, L"Data/01_ecco_1.png");
}

void ActorExample::Release()
{
	mspBug.reset();
	mspBackground.reset();

	D2DFramework::Release();
}

void ActorExample::Render()
{
	HRESULT hr;

	mspRenderTarget->BeginDraw();
	mspRenderTarget->Clear(D2D1::ColorF(0.0f, 0.2f, 0.4f));

	mspBackground->Draw();

	mspBug->Draw();


	hr = mspRenderTarget->EndDraw();
	if (hr == D2DERR_RECREATE_TARGET)
	{
		CreateDeviceResources();
	}
}

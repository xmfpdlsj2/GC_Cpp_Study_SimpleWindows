#include "BuggyHouse.h"

void BuggyHouse::Initialize(HINSTANCE hInstance, LPCWSTR title, UINT width, UINT height)
{
	D2DFramework::Initialize(hInstance, title, width, height);

	mspBackground = std::make_unique<Actor>(this, L"Data/back1_1024.png");

	for (int i = 0; i < 100; i++)
	{
		mBugs.push_back(std::make_unique<Bug>(this));
	}
}

void BuggyHouse::Release()
{
	mBugs.clear();
	mspBackground.reset();

	D2DFramework::Release();
}

void BuggyHouse::Render()
{
	HRESULT hr;

	mspRenderTarget->BeginDraw();
	mspRenderTarget->Clear(D2D1::ColorF(0.0f, 0.2f, 0.4f));

	mspBackground->Draw();

	for (auto& bug : mBugs)
	{
		bug->Draw();
	}


	hr = mspRenderTarget->EndDraw();
	if (hr == D2DERR_RECREATE_TARGET)
	{
		CreateDeviceResources();
	}
}

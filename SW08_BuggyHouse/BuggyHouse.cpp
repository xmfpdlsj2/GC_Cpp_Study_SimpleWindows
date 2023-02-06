#include "BuggyHouse.h"

void BuggyHouse::Initialize(HINSTANCE hInstance, LPCWSTR title, UINT width, UINT height)
{
	D2DFramework::Initialize(hInstance, title, width, height);

	mspBackground = std::make_shared<Actor>(this, L"Data/BH_Bg.png");

	for (int i = 0; i < 100; i++)
	{
		mBugs.push_back(std::make_shared<Bug_ecco>(this));

		if (i % 3 == 0)
		{
			mLadyBugs.push_back(std::make_shared<Bug_ladybug>(this));
		}
	}
}

void BuggyHouse::Release()
{
	mLadyBugs.clear();
	mBugs.clear();
	mspBackground.reset();

	D2DFramework::Release();
}

void BuggyHouse::Render()
{
	HRESULT hr;

	mspRenderTarget->BeginDraw();
	mspRenderTarget->Clear(D2D1::ColorF(0.0f, 0.2f, 0.4f));

	CheckBug();

	mspRenderTarget->SetTransform(D2D1::Matrix3x2F::Identity());
	mspBackground->Draw();

	for (auto& bug : mBugs)
	{
		bug->Draw();
	}

	for (auto& ladybug : mLadyBugs)
	{
		ladybug->Draw();
	}

	hr = mspRenderTarget->EndDraw();
	if (hr == D2DERR_RECREATE_TARGET)
	{
		CreateDeviceResources();
	}
}

void BuggyHouse::CheckBug()
{
	if ((GetAsyncKeyState(VK_LBUTTON) & 0x8001) && !mIsClicked)
	{
		mIsClicked = true;

		POINT pt;
		GetCursorPos(&pt);
		ScreenToClient(mHwnd, &pt);

		// erase - remove idio
		mBugs.erase(std::remove_if(mBugs.begin(), mBugs.end(), [&](std::shared_ptr<Bug_ecco> bug) -> bool
			{
				return bug->IsClicked(pt);
			}), mBugs.end());
	}

	if (!(GetAsyncKeyState(VK_LBUTTON) & 0x8001) && mIsClicked)
	{
		mIsClicked = false;
	}
}

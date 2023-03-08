#include <sstream>
#include "D3DFramework.h"

#pragma comment (lib, "d3d11.lib")

void D3DFramework::InitWindow(HINSTANCE hInstance)
{
	mInstance = hInstance;

	WNDCLASSEX wc;
	ZeroMemory(&wc, sizeof(WNDCLASSEX));

	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpszClassName = CLASS_NAME.c_str();
	wc.hInstance = mInstance;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)COLOR_WINDOW;
	wc.lpfnWndProc = WindowProc;
	wc.cbSize = sizeof(WNDCLASSEX);
	
	if (!RegisterClassEx(&wc))
	{
		MessageBox(nullptr, L"Failed To Register WindowClass!", L"Error", MB_OK);
		return;
	}

	mTitleText = TITLE_NAME;

	RECT wr = { 0, 0, mWindowWidth, mWindowHeight };
	AdjustWindowRect(&wr, WS_OVERLAPPEDWINDOW, FALSE);
	mHwnd = CreateWindowEx(
		NULL,
		CLASS_NAME.c_str(),
		mTitleText.c_str(),
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT,
		wr.right - wr.left, wr.bottom - wr.top,
		NULL,
		NULL,
		mInstance,
		NULL
	);

	if (mHwnd == nullptr)
	{
		MessageBox(nullptr, L"Failed To Create Window!", L"Error", MB_OK);
		return;
	}

	SetWindowLongPtr(mHwnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(this));

	ShowWindow(mHwnd, SW_SHOW);
	SetForegroundWindow(mHwnd);
	SetFocus(mHwnd);
	UpdateWindow(mHwnd);
}

void D3DFramework::InitD3D()
{
	DXGI_SWAP_CHAIN_DESC scd{};

	ZeroMemory(&scd, sizeof(DXGI_SWAP_CHAIN_DESC));
	scd.BufferCount = 1;
	scd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	scd.BufferDesc.Width = mWindowWidth;
	scd.BufferDesc.Height = mWindowHeight;
	scd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	scd.OutputWindow = mHwnd;
	scd.SampleDesc.Count = 1;	// 1x MSAA
	scd.Windowed = TRUE;
	scd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

	D3D11CreateDeviceAndSwapChain(
		NULL,
		D3D_DRIVER_TYPE_HARDWARE,
		NULL,
		0,
		NULL,
		0,
		D3D11_SDK_VERSION,
		&scd,
		mspSwapChain.ReleaseAndGetAddressOf(),
		mspDevice.ReleaseAndGetAddressOf(),
		NULL,
		mspDeviceContext.ReleaseAndGetAddressOf()
	);

	OnResize();
}

void D3DFramework::CalculateFPS()
{
	static int frameCnt{};
	static float timeElapsed{};

	frameCnt++;
	if (mTimer.TotalTime() - timeElapsed >= 1.0f)
	{
		float fps = static_cast<float>(frameCnt);
		float mspf = 1000.0f / fps;

		std::wostringstream oss;
		oss.precision(6);
		oss << mTitleText << L" - " << L"FPS: " << fps << L" , Frame Time : " << mspf << L"(ms)";

		SetWindowText(mHwnd, oss.str().c_str());

		frameCnt = 0;
		timeElapsed += 1.0f;
	}
}

void D3DFramework::OnResize()
{
	if (mspDevice == nullptr)
	{
		return;
	}

	ID3D11RenderTargetView* views[]{ nullptr };
	mspDeviceContext->OMSetRenderTargets(_countof(views), views, nullptr);

	mspRenderTargetView.Reset();
	mspDepthStencilView.Reset();
	mspRenderTarget.Reset();
	mspDepthStencil.Reset();

	mspDeviceContext->Flush();
	mspSwapChain->ResizeBuffers(0, mWindowWidth, mWindowHeight,
		DXGI_FORMAT_UNKNOWN, 0);

	mspSwapChain->GetBuffer(0, IID_PPV_ARGS(mspRenderTarget.ReleaseAndGetAddressOf()));
	mspDevice->CreateRenderTargetView(
		mspRenderTarget.Get(),
		nullptr,
		mspRenderTargetView.ReleaseAndGetAddressOf()
	);

	CD3D11_TEXTURE2D_DESC td(
		DXGI_FORMAT_D24_UNORM_S8_UINT,
		mWindowWidth,
		mWindowHeight,
		1,
		1,
		D3D11_BIND_DEPTH_STENCIL
	);
	mspDevice->CreateTexture2D(&td, nullptr, mspDepthStencil.ReleaseAndGetAddressOf());

	CD3D11_DEPTH_STENCIL_VIEW_DESC dsvd(D3D11_DSV_DIMENSION_TEXTURE2D);
	mspDevice->CreateDepthStencilView(
		mspDepthStencil.Get(),
		&dsvd,
		mspDepthStencilView.ReleaseAndGetAddressOf()
	);

	mspDeviceContext->OMSetRenderTargets(1, mspRenderTargetView.GetAddressOf(), mspDepthStencilView.Get());
}

void D3DFramework::DestroyD3D()
{
	mspSwapChain->SetFullscreenState(FALSE, nullptr);

	mspDepthStencilView.Reset();
	mspDepthStencil.Reset();
	mspRenderTargetView.Reset();
	mspRenderTarget.Reset();

	mspSwapChain.Reset();
	mspDeviceContext.Reset();
	mspDevice.Reset();
}

void D3DFramework::RenderFrame()
{
	CD3D11_VIEWPORT viewport(0.0f, 0.0f, static_cast<float>(mWindowWidth), static_cast<float>(mWindowHeight));
	mspDeviceContext->RSSetViewports(1, &viewport);

	const static float clear_color[4]{ 0.0f, 0.2f, 0.4f, 1.0f };
	mspDeviceContext->ClearRenderTargetView(mspRenderTargetView.Get(), clear_color);
	mspDeviceContext->ClearDepthStencilView(mspDepthStencilView.Get(),
		D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL,
		1.0f,
		0);

	Render();

	HRESULT hr = mspSwapChain->Present(0, 0);
}

void D3DFramework::Initialize(HINSTANCE hInstance, int width, int height)
{
	mWindowWidth = width;
	mWindowHeight = height;
	mPaused = false;

	InitWindow(hInstance);
	InitD3D();
}

void D3DFramework::Destroy()
{
	DestroyD3D();

	DestroyWindow(mHwnd);
	UnregisterClass(CLASS_NAME.c_str(), mInstance);
}

void D3DFramework::GameLoop()
{
	mTimer.Start();
	mInput.Initialize();

	MSG msg{};
	while (true)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);

			if (msg.message == WM_QUIT)
			{
				break;
			}
		}
		else
		{
			mTimer.Update();
			if (mPaused)
			{
				Sleep(100);
			}
			else
			{
				CalculateFPS();

				Update(mTimer.DeltaTime());
				RenderFrame();
			}
		}
	}
}

LRESULT D3DFramework::MessageHandler(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
		case WM_KEYDOWN:
			mInput.SetKeyDown(static_cast<unsigned int>(wParam));
			break;
		case WM_KEYUP:
			mInput.SetKeyUp(static_cast<unsigned int>(wParam));
			break;
		case WM_LBUTTONDOWN:
			mInput.SetKeyDown(VK_LBUTTON);
			break;
		case WM_LBUTTONUP:
			mInput.SetKeyUp(VK_LBUTTON);
			break;
		case WM_RBUTTONDOWN:
			mInput.SetKeyDown(VK_RBUTTON);
			break;
		case WM_RBUTTONUP:
			mInput.SetKeyUp(VK_RBUTTON);
			break;
		case WM_MOUSEMOVE:
			mInput.SetCursor(LOWORD(lParam), HIWORD(lParam));
			break;
		case WM_ACTIVATE:
			if (LOWORD(wParam) == WA_INACTIVE)
			{
				mPaused = true;
				mTimer.Stop();
			}
			else
			{
				mPaused = false;
				mTimer.Resume();
			}
			break;
		case WM_ENTERSIZEMOVE:
			mPaused = true;
			mTimer.Stop();
			mResizing = true;
			break;
		case WM_SIZE:
			if (mspDevice == nullptr)
			{
				return 0;
			}
			mWindowWidth = LOWORD(lParam);
			mWindowHeight = HIWORD(lParam);

			if (wParam == SIZE_MINIMIZED)
			{
				if (!mPaused)
				{
					mTimer.Stop();
				}
				mPaused = true;

				mMinimized = true;
				mMaximized = false;
			}
			else if (wParam == SIZE_MAXIMIZED)
			{
				mTimer.Resume();
				mPaused = false;

				mMinimized = false;
				mMaximized = true;
				OnResize();
			}
			else if (wParam == SIZE_RESTORED)
			{
				if (mMinimized)
				{
					mPaused = false;
					mTimer.Resume();

					mMinimized = false;
					OnResize();
				}
				else if (mMaximized)
				{
					mPaused = false;
					mTimer.Resume();

					mMaximized = false;
					OnResize();
				}
				else if (mResizing)
				{
					RenderFrame();
				}
				else
				{
					OnResize();
				}
			}
			break;
		case WM_EXITSIZEMOVE:
			mPaused = false;
			mTimer.Resume();
			mResizing = false;
			OnResize();
			break;
		case WM_GETMINMAXINFO:
			((MINMAXINFO*)lParam)->ptMinTrackSize.x = 640;
			((MINMAXINFO*)lParam)->ptMinTrackSize.y = 480;
			break;
		case WM_MENUCHAR:
			return MAKELRESULT(0, MNC_CLOSE);
			break;
		case WM_CLOSE:
			DestroyWindow(hwnd);
			break;
		case WM_DESTROY:
			PostQuitMessage(0);
			break;
		default:
			return DefWindowProc(hwnd, message, wParam, lParam);
			break;
	}
	return LRESULT();
}

LRESULT D3DFramework::WindowProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	D3DFramework* pFramework = reinterpret_cast<D3DFramework*>(GetWindowLongPtr(hwnd, GWLP_USERDATA));
	
	if (pFramework == nullptr)
	{
		return DefWindowProc(hwnd, message, wParam, lParam);
	}
	else
	{
		return pFramework->MessageHandler(hwnd, message, wParam, lParam);
	}
}

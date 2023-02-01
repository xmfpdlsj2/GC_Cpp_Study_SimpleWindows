#pragma once
#include <Windows.h>
#include <d2d1.h>
#include <wincodec.h>
#include <wrl/client.h>
#include "ComException.h"


class D2DFramework
{
	const LPCWSTR gClassName{ L"MyWindowClass" };

protected:
	HWND mHwnd{};

	Microsoft::WRL::ComPtr<IWICImagingFactory> mspWICFactory{};
	Microsoft::WRL::ComPtr<ID2D1Factory> mspD2DFactory{};
	Microsoft::WRL::ComPtr<ID2D1HwndRenderTarget> mspRenderTarget{};

protected:
	HRESULT InitWindow(HINSTANCE hInstance, LPCWSTR title, UINT width, UINT height);
	virtual HRESULT InitD2D(HWND hwnd);
	virtual HRESULT CreateDeviceResources();

public:
	virtual void Initialize(HINSTANCE hInstance, LPCWSTR title = L"Direct2D Example", UINT width = 1024, UINT height = 768);
	virtual void Release();
	virtual void Render();
	virtual int GameLoop();

	void ShowErrorMsg(LPCWSTR msg, HRESULT error = E_FAIL, LPCWSTR title = L"Error");

public:
	ID2D1HwndRenderTarget* GetRenderTarget() const
	{
		return mspRenderTarget.Get();
	}

	IWICImagingFactory* GetWICFactory() const
	{
		return mspWICFactory.Get();
	}
	ID2D1Factory* GetD2DFactory() const
	{
		return mspD2DFactory.Get();
	}
};

LRESULT CALLBACK WindowProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);


#pragma once

#define WIN32_LEAN_AND_MEAN

#include <Windows.h>
#include <wrl/client.h>
#include <d3d11.h>
#include <string>



class D3DFramework
{
private:
	const std::wstring CLASS_NAME{ L"MyWindowClass" };
	const std::wstring TITLE_NAME{ L"Direct3D Sample" };
protected:
	int mWindowWidth{ 1024 };
	int mWindowHeight{ 768 };

	HWND mHwnd{};
	HINSTANCE mInstance{};

	bool mMinimized{ false };
	bool mMaximized{ false };
	bool mResizing{ false };

	Microsoft::WRL::ComPtr<ID3D11Device> mspDevice{};
	Microsoft::WRL::ComPtr<ID3D11DeviceContext> mspDeviceContext{};
	Microsoft::WRL::ComPtr<IDXGISwapChain> mspSwapChain{};

	Microsoft::WRL::ComPtr<ID3D11Texture2D> mspRenderTarget{};
	Microsoft::WRL::ComPtr<ID3D11Texture2D> mspDepthStencil{};

	Microsoft::WRL::ComPtr<ID3D11RenderTargetView> mspRenderTargetView{};
	Microsoft::WRL::ComPtr<ID3D11DepthStencilView> mspDepthStencilView{};

private:
	void InitWindow(HINSTANCE hInstance);
	void InitD3D();

protected:
	void OnResize();
	void DestroyD3D();
	void RenderFrame();
	virtual void Render() {};


public:
	virtual void Initialize(HINSTANCE hInstance, int width = 800, int height = 600);
	virtual void Destroy();
	virtual void GameLoop();

public:
	LRESULT CALLBACK MessageHandler(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);
	static LRESULT WindowProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);
};
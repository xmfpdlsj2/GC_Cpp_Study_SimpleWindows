#include <Windows.h>

#include "GameLogic.h"

#pragma comment (lib, "Gdiplus.lib")

const std::wstring gClassName{ L"MyWindowClass" };

solitaire::GameLogic gLogic;

LRESULT CALLBACK WindowProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);

int WINAPI WinMain(
	_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPSTR lpCmdLine,
	_In_ int nShowCmd)
{
	Gdiplus::GdiplusStartupInput gdipluStartupInput;
	ULONG_PTR gdiPlusToken;

	Gdiplus::GdiplusStartup(&gdiPlusToken, &gdipluStartupInput, nullptr);
	

	HWND hwnd;
	WNDCLASSEX wc{};
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpszClassName = gClassName.c_str();
	wc.hInstance = hInstance;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)COLOR_WINDOW;
	wc.lpfnWndProc = WindowProc;
	wc.cbSize = sizeof(WNDCLASSEX);
	
	if (!RegisterClassEx(&wc))
	{
		MessageBox(NULL, L"Failed to Register Window Class!", L"Error", MB_OK);
		return 0;
	}

	RECT wr{ 0, 0, 1024, 768 };
	AdjustWindowRect(&wr, WS_OVERLAPPED | WS_SYSMENU | WS_CAPTION, FALSE);
	hwnd = CreateWindowEx(
		NULL,
		gClassName.c_str(),
		L"Card Memory Game",
		WS_OVERLAPPED | WS_SYSMENU | WS_CAPTION,
		CW_USEDEFAULT, CW_USEDEFAULT, wr.right - wr.left, wr.bottom - wr.top,
		NULL,
		NULL,
		hInstance,
		NULL
	);
	
	if (hwnd == nullptr)
	{
		MessageBox(NULL, L"Failed to Create Window!", L"Error", MB_OK);
		return 0;
	}

	gLogic.Initialize(hwnd);

	ShowWindow(hwnd, nShowCmd);
	UpdateWindow(hwnd);

	MSG msg;
	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	gLogic.Release();

	Gdiplus::GdiplusShutdown(gdiPlusToken);
	return static_cast<int>(msg.wParam);
}

void OnPaint(HWND hwnd)
{
	HDC hdc;
	PAINTSTRUCT ps;
	hdc = BeginPaint(hwnd, &ps);

	Gdiplus::Graphics graphics(hdc);

	gLogic.Draw(graphics);

	EndPaint(hwnd, &ps);

}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
		case WM_PAINT:
			OnPaint(hwnd);
			break;
		case WM_LBUTTONUP:
			gLogic.OnClick(LOWORD(lParam), HIWORD(lParam));
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
	return 0;
}
#include <Windows.h>
#include <d2d1.h>
#include <cmath>

#pragma comment (lib, "d2d1.lib")

const wchar_t gClassName[]{ L"MyWindowClass" };

ID2D1Factory* gpD2DFactory{ };
ID2D1HwndRenderTarget* gpRenderTarget{};

ID2D1SolidColorBrush* gpSolidBrush{};
ID2D1RadialGradientBrush* gpGradientBrush{};


LRESULT CALLBACK WindowProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);
void OnPaint(HWND hwnd);


int WINAPI WinMain(
	_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPSTR lpCmdLine,
	_In_ int nShowCmd
)
{
	// 1. Direct2D ���͸� ����
	HRESULT hr;
	hr = D2D1CreateFactory(
		D2D1_FACTORY_TYPE_SINGLE_THREADED, 
		&gpD2DFactory);

	if (FAILED(hr))
	{
		MessageBox(NULL, L"Failed to Create D2D Factory!", L"Error", MB_OK);
		return 0;
	}

	
	WNDCLASSEX wc{};
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpszClassName = gClassName;
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

	RECT wr{ 0,0,1024, 768 };
	AdjustWindowRect(&wr, WS_OVERLAPPEDWINDOW, FALSE);
	HWND hwnd;
	hwnd = CreateWindowEx(
		NULL,
		gClassName,
		L"Direct2D",
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT,
		wr.right - wr.left, wr.bottom - wr.top,
		NULL,
		NULL,
		hInstance,
		NULL
	);
	
	if (hwnd == NULL)
	{
		MessageBox(NULL, L"Failed to Create Window!", L"Error", MB_OK);
		return 0;
	}


	GetClientRect(hwnd, &wr);
	hr = gpD2DFactory->CreateHwndRenderTarget(
		D2D1::RenderTargetProperties(),
		D2D1::HwndRenderTargetProperties(
			hwnd,
			D2D1::SizeU(wr.right - wr.left, wr.bottom - wr.top)),
		&gpRenderTarget
	);
	if (FAILED(hr))
	{
		MessageBox(NULL, L"Failed to Create D2D RenderTarget!", L"Error", MB_OK);
		return 0;
	}

	gpRenderTarget->CreateSolidColorBrush(
		D2D1::ColorF(D2D1::ColorF::HotPink),
		&gpSolidBrush
	);

	ID2D1GradientStopCollection* pGradientStops{};
	D2D1_GRADIENT_STOP stops[2]{
		{0.0f, D2D1::ColorF(D2D1::ColorF::Yellow)},
		{1.0f, D2D1::ColorF(D2D1::ColorF::Crimson)}
	};
	
	gpRenderTarget->CreateGradientStopCollection(
		stops, 2, &pGradientStops);

	gpRenderTarget->CreateRadialGradientBrush(
		D2D1::RadialGradientBrushProperties(
			D2D1::Point2F(50, 150),
			D2D1::Point2F(0, 0),
			50, 50
		),
		pGradientStops,
		&gpGradientBrush
	);

	if (pGradientStops != nullptr)
	{
		pGradientStops->Release();
		pGradientStops = nullptr;
	}

	ShowWindow(hwnd, nShowCmd);
	UpdateWindow(hwnd);

	MSG msg;
	//while (GetMessage(&msg, NULL, 0, 0))
	//{
	//	OnPaint(hwnd);
	//	TranslateMessage(&msg);
	//	DispatchMessage(&msg);
	//}
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
			OnPaint(hwnd);
		}
	}

	if (gpGradientBrush != nullptr)
	{
		gpGradientBrush->Release();
		gpGradientBrush = nullptr;
	}

	if (gpSolidBrush != nullptr)
	{
		gpSolidBrush->Release();
		gpSolidBrush = nullptr;
	}

	if (gpRenderTarget != nullptr)
	{
		gpRenderTarget->Release();
		gpRenderTarget = nullptr;
	}

	if (gpD2DFactory != nullptr)
	{
		gpD2DFactory->Release();
		gpD2DFactory = nullptr;
	}

	return static_cast<int>(msg.wParam);
}

void OnPaint(HWND hwnd)
{
	HDC hdc;
	PAINTSTRUCT ps;
	hdc = BeginPaint(hwnd, &ps);

	gpRenderTarget->BeginDraw();
	gpRenderTarget->Clear(D2D1::ColorF(0.0f, 0.2f, 0.4f));

	gpRenderTarget->FillRectangle(
		D2D1::RectF(0.0f, 0.0f, 100.0f, 100.0f),
		gpSolidBrush);

	gpSolidBrush->SetOpacity(0.5f);
	gpSolidBrush->SetColor(D2D1::ColorF(D2D1::ColorF::Yellow));
	gpRenderTarget->FillRectangle(
		D2D1::RectF(50.0f, 50.0f, 150.0f, 150.0f),
		gpSolidBrush
	);

	static float angle{};

	gpRenderTarget->FillEllipse(
		D2D1::Ellipse(D2D1::Point2F((75.0f + sinf(angle) * 25.0f), 150.f), 50.0f, 50.0f),
		gpGradientBrush);

	angle += 0.2f;

	gpRenderTarget->EndDraw();

	EndPaint(hwnd, &ps);
}


LRESULT CALLBACK WindowProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
		case WM_PAINT:
			OnPaint(hwnd);
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
}
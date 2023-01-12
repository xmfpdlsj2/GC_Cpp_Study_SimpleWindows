#include <Windows.h>
#include <string>
#include <sstream>

#include <gdiplus.h>
#pragma comment (lib, "Gdiplus.lib")


// 1. "������ Ŭ����" ���
// 2. "������" ����
// 3. "�޽��� ó��(����)"
// 4. "������ ���ν���" �ۼ�

const wchar_t gClassName[]{ L"MyWindowClass" };

LRESULT CALLBACK WindowProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);

void Display(HWND hwnd);

int WINAPI WinMain(
	_In_ HINSTANCE hInstance, 
	_In_opt_ HINSTANCE hPrevInstance, 
	_In_ LPSTR lpCmdLine, 
	_In_ int nShowCmd)
{
	Gdiplus::GdiplusStartupInput gdiSI;
	ULONG_PTR gdiToken;

	Gdiplus::GdiplusStartup(&gdiToken, &gdiSI, nullptr);

	// 1. ������ Ŭ���� ���� & ���
	WNDCLASSEX wc{};
	// ZeroMemory(&wc, sizeof(WNDCLASSEX));  // �ʼ� : 0���� ä���

	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpszClassName = gClassName;				// �ʼ�
	wc.hInstance = hInstance;					// �ʼ�
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)COLOR_WINDOW;
	wc.lpfnWndProc = WindowProc;				// �ʼ�
	wc.cbSize = sizeof(WNDCLASSEX);				// �ʼ�

	if (!RegisterClassEx(&wc))
	{
		MessageBox(nullptr, L"������ Ŭ���� ��� ����!", L"����", MB_OK);
		return 0;
	}

	// 2. ������ ����
	HWND hwnd;

	RECT wr{ 0,0,640,480 };
	AdjustWindowRect(&wr, WS_OVERLAPPEDWINDOW, FALSE);

	hwnd = CreateWindowEx(
		NULL,
		gClassName,
		L"HelloWindw",
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		//640, -> wr.right - wr.left,
		wr.right - wr.left,
		//480, -> wr.bottom - wr.top,
		wr.bottom - wr.top,
		NULL,
		NULL,
		hInstance,
		NULL
		);

	if (hwnd == nullptr)
	{
		MessageBox(nullptr, L"������ ���� ����!", L"����", MB_OK);
		return 0;
	}

	ShowWindow(hwnd, nShowCmd);
	UpdateWindow(hwnd);

	// 3. ������ �޽����� ó���ϴ� ����
	MSG msg;
	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	
	Gdiplus::GdiplusShutdown(gdiToken);

	return static_cast<int>(msg.wParam);
}

// 4. ������ ���ν���
LRESULT CALLBACK WindowProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
		case WM_PAINT:
			Display(hwnd);
			break;

		case WM_KEYDOWN:
		{
			std::wostringstream oss;
			oss << "Virtual key : " << wParam << std::endl;
			//MessageBox(hwnd, oss.str().c_str(), L"Message", MB_OK);
			OutputDebugString(oss.str().c_str());
			break;
		}

		case WM_LBUTTONDOWN:
		{
			std::wostringstream oss;
			//oss << "x : " << (WORD)lParam << ", y : " << (WORD)(lParam >> 16) << std::endl;
			oss << "x : " << LOWORD(lParam) << ", y : " << HIWORD(lParam) << std::endl;
			MessageBox(hwnd, oss.str().c_str(), L"Message", MB_OK);
			break;
		}
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

// GDI : Device Context
void Display(HWND hwnd)
{
	//HDC hdc{}; // Handle of DC
	//hdc = GetDC(hwnd);
	//Rectangle(hdc, 0, 0, 100, 100);
	//ReleaseDC(hwnd, hdc);

	PAINTSTRUCT ps;
	HDC hdc = BeginPaint(hwnd, &ps);

	// GDI version
	//HPEN pen = CreatePen(PS_DOT, 2, RGB(255, 0, 0));
	//HBRUSH brush = CreateHatchBrush(HS_DIAGCROSS, RGB(0, 0, 255));
	//SelectObject(hdc, brush);
	//SelectObject(hdc, pen);

	//Rectangle(hdc, 0, 0, 100, 100);

	//DeleteObject(brush);
	//DeleteObject(pen);

	// GDI+ verstion
	Gdiplus::Graphics graphics(hdc);
	Gdiplus::Pen pen(Gdiplus::Color(255, 255, 0, 0));
	pen.SetWidth(2);
	Gdiplus::HatchBrush brush(Gdiplus::HatchStyleDiagonalCross
		, Gdiplus::Color(255, 0, 0, 255), Gdiplus::Color(255, 255, 255, 255));
	graphics.DrawRectangle(&pen, 0, 0, 100, 100);
	graphics.FillRectangle(&brush, 0, 0, 100, 100);

	Gdiplus::Font font(L"���� ���", 24);
	Gdiplus::SolidBrush solidBrush(Gdiplus::Color(255, 255, 0, 255));
	graphics.DrawString(L"�ȳ��ϼ���~ ���� ��� �Դϴ�.", -1, &font,
		Gdiplus::PointF(110.0f, 10.0f),
		&solidBrush);

	Gdiplus::Image image(L"Data/image_logo.jpg");
	graphics.DrawImage(&image, 200, 200);


	EndPaint(hwnd, &ps);
}
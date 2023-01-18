#include "D2DFramework.h"

int WINAPI WinMain(
	_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPSTR lpCmdLine,
	_In_ int nShowCmd
)
{
	try
	{
		D2DFramework myFramework;
		myFramework.Initialize(hInstance);

		int ret = myFramework.GameLoop();

		myFramework.Release();

		return ret;
	}
	catch (ComException e)
	{
		MessageBoxA(NULL, e.what(), "D2D Exception", MB_OK);
		return 0;
	}

	return 0;
}



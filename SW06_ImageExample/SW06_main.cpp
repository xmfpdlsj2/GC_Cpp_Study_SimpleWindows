#include "ImageExample.h"

int WINAPI WinMain(
	_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPSTR lpCmdLine,
	_In_ int nShowCmd
)
{
	try
	{
		ImageExample myFramework;

		myFramework.Initialize(hInstance);
		int ret = myFramework.GameLoop();
		myFramework.Release();

		return ret;
	}
	catch (const ComException& e)
	{
		OutputDebugStringA(e.what());
	}
}
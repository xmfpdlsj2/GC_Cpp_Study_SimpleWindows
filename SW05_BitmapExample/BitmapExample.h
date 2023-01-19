#pragma once
#include <memory>
#include "D2DFramework.h"

// 가상의 그래픽 카드 (Framebuffer 1024 x 764)
// 시스템 메모리 - BackBuffer
// BackBuffer -> FrameBuffer 복사
// 
// 가상의 그래픽카드에 출력

const int BITMAP_WIDTH{ 1024 }, BITMAP_HEIGHT{ 768 };
const int BITMAP_BYTECOUNT{ 4 }; // BGRA 각 8비트씩

class BitmapExample : public D2DFramework
{
private:
	Microsoft::WRL::ComPtr<ID2D1Bitmap> mspFrameBitmap;
	std::unique_ptr<UINT8[]> mspBackBuffer;
	
public:
	virtual void Initialize(HINSTANCE hInstance, LPCWSTR title, UINT width = 1024, UINT height = 768) override;
	void Render() override;

	void PresentBuffer();
};


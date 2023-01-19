#pragma once
#include <memory>
#include "D2DFramework.h"

// ������ �׷��� ī�� (Framebuffer 1024 x 764)
// �ý��� �޸� - BackBuffer
// BackBuffer -> FrameBuffer ����
// 
// ������ �׷���ī�忡 ���

const int BITMAP_WIDTH{ 1024 }, BITMAP_HEIGHT{ 768 };
const int BITMAP_BYTECOUNT{ 4 }; // BGRA �� 8��Ʈ��

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


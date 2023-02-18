#include "DrawTriangle.h"

#pragma comment(lib, "d3dcomplier.lib")

void DrawTriangle::Initialize(HINSTANCE hInstance, int width, int height)
{
}

void DrawTriangle::Destroy()
{
}

void DrawTriangle::InitTriangle()
{
}

void DrawTriangle::InitPipeline()
{
	VERTEX vertices[]
	{
		{0.0f, 0.5f, 0.0f, {1.0f, 0.0f, 0.0f, 1.0f }},
		{0.45f, -0.5f, 0.0f, {0.0f, 1.0f, 0.0f, 1.0f }},
		{-0.45f, -0.5f, 0.0f, {0.0f, 0.0f, 1.0f, 1.0f }}
	};
	CD3D11_BUFFER_DESC bd(
		sizeof(VERTEX) * 3,
		D3D11_BIND_VERTEX_BUFFER,
		D3D11_USAGE_DYNAMIC,
		D3D11_CPU_ACCESS_WRITE
	);
	mspDevice->CreateBuffer(&bd, nullptr, mspVertexBuffer.ReleaseAndGetAddressOf());
}

void DrawTriangle::Render()
{
}

#pragma once
#include "D3DFramework.h"
#include <d3dcompiler.h>
#include <DirectXMath.h>

class DrawTriangle : public D3DFramework
{
	struct VERTEX
	{
		FLOAT x, y, z;
		FLOAT u, v;
	};

	struct MatrixBuffer
	{
		DirectX::XMMATRIX world;
	};
	
	Microsoft::WRL::ComPtr<ID3D11InputLayout> mspInputLayout;
	Microsoft::WRL::ComPtr<ID3D11Buffer> mspVertexBuffer;
	Microsoft::WRL::ComPtr<ID3D11VertexShader> mspVertexShader;
	Microsoft::WRL::ComPtr<ID3D11PixelShader> mspPixelShader;

	Microsoft::WRL::ComPtr<ID3D11Texture2D> mspTexture;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> mspTextureView;

	Microsoft::WRL::ComPtr<ID3D11SamplerState> mspSamplerState;
	Microsoft::WRL::ComPtr<ID3D11BlendState> mspBlendState;

	Microsoft::WRL::ComPtr<ID3D11Buffer> mspConstantBuffer;

	float mX, mY;
	float mRotation;
	DirectX::XMMATRIX mWorld;

protected:
	void Render() override;
	void Update(float delta) override;

public:
	void Initialize(HINSTANCE hInstance, int width = 800, int height = 600) override;
	void Destroy() override;

private:
	void InitTriangle();
	void InitPipeline();

	HRESULT CreateTextureFromBMP();
};


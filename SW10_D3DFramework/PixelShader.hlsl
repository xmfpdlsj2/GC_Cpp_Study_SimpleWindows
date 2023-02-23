Texture2D shaderTexture;
SamplerState simpleSampler
{
	Filter = MIN_MAG_MIP_LINEAR;
	AddressU = Wrap;
	AddressV = Wrap;
};

float4 main(float4 position : SV_POSITION, float2 tex : TEXCOORD0) : SV_TARGET
{
	return shaderTexture.Sample(simpleSampler, tex);
}
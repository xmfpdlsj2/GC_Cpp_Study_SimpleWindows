Texture2D shaderTexture;
SamplerState simpleSampler;

float4 main(float4 position : SV_POSITION, float2 tex : TEXCOORD0) : SV_TARGET
{
	return shaderTexture.Sample(simpleSampler, tex);
	//float4 result = shaderTexture.Sample(simpleSampler, tex);
	//result[0] = result[1] = result[2] = result[0] * 0.3f + result[1] * 0.59f + result[2] * 0.11f;
	//return result;
}
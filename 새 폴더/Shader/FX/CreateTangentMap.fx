Texture2D _map : register(t0);

SamplerState samp;


struct VertexInput
{
	float4 position : POSITION0;
	float2 uv : TEXCOORD0;
};

struct PixelInput
{
	float4 position : SV_POSITION;
	float2 uv : TEXCOORD0;

};


PixelInput VS(VertexInput input)
{
	PixelInput output;

	output = input;


	return output;
}


float4 PS(PixelInput input) : SV_Target
{
	float pixelWidth = 1.0/420.0;
	float baseMap = _map.Sample(samp, input.uv).r;
	
	float baseMapLeft = _map.Sample(samp, float2(input.uv.x - pixelWidth, input.uv.y)).r;
	float baseMapRight = _map.Sample(samp, float2(input.uv.x + pixelWidth, input.uv.y)).r;
	float baseMapDown = _map.Sample(samp, float2(input.uv.x , input.uv.y - pixelWidth)).r;
	float baseMapUp = _map.Sample(samp, float2(input.uv.x , input.uv.y+ pixelWidth)).r;
	
	float3 normal = cross(float3(0, pixelWidth, baseMapUp - baseMap), float3(pixelWidth,0,baseMapLeft - baseMap));
	normal += cross(float3(0, -pixelWidth, baseMapDown - baseMap), float3( -pixelWidth, 0, baseMapRight - baseMap));
	
	normal = normalize(normal);
	normal.rg += 1;
	normal.rg /= 2;
	
	
	float4 result = float4(normal.rg,1, 1);





    return result;
}	
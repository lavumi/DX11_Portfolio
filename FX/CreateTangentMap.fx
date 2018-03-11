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
	float pixelWidth = 1.0/256.0;
	float baseMap = _map.Sample(samp, input.uv);
   // return baseMap;

	float baseMapLeft = _map.Sample(samp, float2(input.uv.x - pixelWidth, input.uv.y)).r;
	float baseMapRight = _map.Sample(samp, float2(input.uv.x + pixelWidth, input.uv.y)).r;
	float baseMapDown = _map.Sample(samp, float2(input.uv.x , input.uv.y - pixelWidth)).r;
	float baseMapUp = _map.Sample(samp, float2(input.uv.x , input.uv.y+ pixelWidth)).r;
	
    float3 normal = cross(float3(0, baseMapUp - baseMap, pixelWidth), float3(pixelWidth, baseMapLeft - baseMap, 0));
    normal += cross(float3(0, baseMapDown - baseMap, -pixelWidth), float3(-pixelWidth, baseMapRight - baseMap, 0));
	
	normal = normalize(normal);
	normal.rgb += 1;
	normal.rgb /= 2;
	
	
	float4 result = float4(normal.rb,0, 1);





    return result;
}	
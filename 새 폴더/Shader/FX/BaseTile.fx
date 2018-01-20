
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
	float2 uv = input.uv;

	float2 uv_scale = uv;
	uv_scale = frac(uv_scale);

   

	uv_scale -= 0.5;
	uv_scale = abs(uv_scale);

    float result1 = lerp(float4(1, 1, 1, 1),    float4(0, 0, 0, 1),  (uv_scale.x - 0.48)/0.01);
    float result2 = lerp(float4(1, 1, 1, 1), float4(0, 0, 0, 1), (uv_scale.y - 0.48) / 0.01);

    result1 = saturate(result1);
    result2 = saturate(result2);

    result1 += result2;
    result1 -= 1;

    result1 = saturate(result1);


   

    return result1;
}
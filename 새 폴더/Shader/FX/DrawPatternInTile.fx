Texture2D _mapBase : register(t0);
Texture2D _mapPattern : register(t1);

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
    float4 base = _mapBase.Sample(samp, input.uv);
    float4 pattern = _mapPattern.Sample(samp, input.uv);


    float4 result = lerp(base * pattern, 0.2, step(base.r, 0.001));

    return result;

}
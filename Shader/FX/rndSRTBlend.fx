Texture2D _destmap : register(t1);
Texture2D _srcmap : register(t0);

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
    float4 result = _srcmap.Sample(samp, input.uv);
    float4 destmap = _destmap.Sample(samp, input.uv);


    destmap.rgb = destmap.rgb * (1 - result.a) + result.rgb * result.a;
    
    destmap.a = destmap.g;
    return destmap;

}
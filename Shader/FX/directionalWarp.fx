Texture2D _map2 : register(t1);
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
    
    float grad = _map2.Sample(samp, input.uv).g;

    float2 uv = input.uv;
    uv.x += grad;
   

    float4 result = _map.Sample(samp, uv);
    if(result.g>0.001)
        result.a = 1;
    else
        result.a = 0;
   // result.a = result.g;
    result.rgb -= input.uv.y;
    
    return result;

    

}
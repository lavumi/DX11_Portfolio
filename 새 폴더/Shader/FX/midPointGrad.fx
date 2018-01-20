cbuffer regid : register(b0)
{
    float width;
    float band_pos;
}

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

    float grad_a = abs(band_pos - input.uv.y * (1 + band_pos));

    return lerp(0, width, grad_a*grad_a);;
    

}
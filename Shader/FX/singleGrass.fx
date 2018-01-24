SamplerState samp;

cbuffer width : register(b0)
{
    float width;
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
    float2 originuv = input.uv ;


    

    
    float2 uv = originuv - 0.5;
    uv.x *= 100/width;
    uv.y *= 1.3;

    //직사각형 그리기
    float rect = lerp(0, 1, step(abs(uv.x), 0.55));

    //원 그리기
    float circle = lerp(0, 1, step(uv.x * uv.x + uv.y*uv.y, 0.3));

    //둘 합치기
    float grass = lerp(circle, rect, step(0.5, originuv.y));


    return grass;

}
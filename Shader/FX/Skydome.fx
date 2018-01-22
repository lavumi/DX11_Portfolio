cbuffer MatrixBuffer : register(b0)
{
    matrix _world;
    matrix _view;
    matrix _projection;
};



cbuffer LightBuffer : register(b0)
{
    float3 _lightDir;
    float _padding;
};

cbuffer ColorBuffer : register(b1)
{
    float4 _center;
    float4 _apex;
};

struct VertexInput
{
    float4 position : POSITION0;
};

struct PixelInput
{
    float4 position : SV_POSITION;
    float4 wPosition : TEXCOORD0;
};

PixelInput VS(VertexInput input)
{
    PixelInput output;

    input.position.w = 1.0f;

    output.position = mul(input.position, _world);
    output.position = mul(output.position, _view);
    output.position = mul(output.position, _projection);

    output.wPosition = input.position;

    return output;
}

float4 PS(PixelInput input) : SV_TARGET
{
    float height = saturate(input.wPosition.y);
    
    return  lerp(_center, _apex, height);
}
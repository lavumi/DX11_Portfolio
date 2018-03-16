#include "ShaderPreset.hlsli"


struct VertexInput
{
    float4 position : POSITION0;
    float4 color : COLOR0;

};

struct PixelInput
{
    float4 position : SV_POSITION;
    float4 color : COLOR0;

};

PixelInput VS(VertexInput input)
{
    PixelInput output;
    input.position.w = 1;
    output.position = mul(input.position, _world);
    output.position = mul(output.position, _viewXprojection);

    output.color = input.color;


    return output;
}



Texture2D _map : register(t0);

cbuffer color : register(b0)
{
    float4 inputColor;
};



float4 PS(PixelInput input) : SV_Target
{
    float4 color = input.color;

    return color;

}
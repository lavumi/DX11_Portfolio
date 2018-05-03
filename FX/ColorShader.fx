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
    float4 worldpos : TEXCOORD0;
};

PixelInput VS(VertexInput input)
{
    PixelInput output;
    input.position.w = 1;
    output.position = mul(input.position, _world);
    output.position = MulVP(output.position);

    output.worldpos = output.position;
    output.color = input.color;


    return output;
}


float4 PS(PixelInput input) : SV_Target
{
    float4 color = input.color;

    return input.worldpos.z / input.worldpos.w;

}
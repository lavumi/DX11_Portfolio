#include "ShaderPreset.hlsli"

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
    input.position.w = 1;
    output.position = mul(input.position, _world);
    output.position = mul(output.position, _viewXprojection);


    output.uv = input.uv;


    return output;
}



Texture2D _map : register(t10);


SamplerState samp[3];


float4 PS(PixelInput input) : SV_Target
{


  //  float test = input.uv.y / 4;
  //  float b = 1 - test;
  //  float r = 0.50 - test;
  //  float g = 0.75 - test;
  //
  //  float4 color = float4(r, g, b, 1);

    float4 color = _map.Sample(samp[0], input.uv);
   
    return color;
}
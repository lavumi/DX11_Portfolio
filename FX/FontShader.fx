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



Texture2D _map : register(t0);
SamplerState samp[3];

cbuffer PixelBuffer
{
    float4 pixelColor;
};

float4 PS(PixelInput input) : SV_Target
{

    float4 color = _map.Sample(samp[0], input.uv);

  if (color.g < 0.5f){
      color.a = 0.0f;
  } 
  else { 
      color.a = 1.0f; 
      color.rgb = 0; 
  }

    return color;
}
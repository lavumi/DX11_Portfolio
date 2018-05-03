#include "ShaderPreset.hlsli"

struct VertexInput
{
    float4 position : POSITION0;
    float2 uv : TEXCOORD0;
    float4 world0 : INSTMATRIX0;
    float4 world1 : INSTMATRIX1;
    float4 world2 : INSTMATRIX2;
    float4 world3 : INSTMATRIX3;

};

struct PixelInput
{
    float4 position : SV_POSITION;
    float2 uv : TEXCOORD0;
    float4 viewPosition : TEXCOORD1;
};

PixelInput VS(VertexInput input)
{
    PixelInput output;
    input.position.w = 1;

    float4x4 world = float4x4(input.world0, input.world1, input.world2, input.world3);
    output.position = mul(input.position, world);
 //   output.position = mul(input.position, _world);
    output.position = MulVP(output.position);

    output.viewPosition = output.position;

    output.uv = input.uv;


    return output;
}



Texture2D _map : register(t10);


SamplerState samp[3];

Texture2D _lightMap : register(t13);
float4 PS(PixelInput input) : SV_Target
{




    float4 color = float4(0.20784f, 0.48431f, 0.26666f, 1);
    float4 color2 = float4(0.88f, 0.87f, 0.6f, 1);

    color = lerp(color, color2, 0.5f) * 0.6f;


    float2 projectTexCoord;

    projectTexCoord.x = input.viewPosition.x / input.viewPosition.w / 2.0f + 0.5f;
    projectTexCoord.y = -input.viewPosition.y / input.viewPosition.w / 2.0f + 0.5f;
    float shadowValue = _lightMap.Sample(samp[1], projectTexCoord).g;
    return color * shadowValue;
}
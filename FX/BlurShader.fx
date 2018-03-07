#include "ShaderPreset.hlsli"


cbuffer ScreenSize : register(b0)
{
    float2 ScreenSize;
    float2 padding22;
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

    float2 texCoordx1 : TEXCOORD1;
    float2 texCoordx2 : TEXCOORD2;
    float2 texCoordx3 : TEXCOORD3;
    float2 texCoordx4 : TEXCOORD4;
    float2 texCoordx5 : TEXCOORD5;
    float2 texCoordx6 : TEXCOORD6;
    float2 texCoordx7 : TEXCOORD7;
    float2 texCoordx8 : TEXCOORD8;
    float2 texCoordx9 : TEXCOORD9;


    float2 texCoordy1 : TEXCOORD11;
    float2 texCoordy2 : TEXCOORD12;
    float2 texCoordy3 : TEXCOORD13;
    float2 texCoordy4 : TEXCOORD14;
    float2 texCoordy5 : TEXCOORD15;
    float2 texCoordy6 : TEXCOORD16;
    float2 texCoordy7 : TEXCOORD17;
    float2 texCoordy8 : TEXCOORD18;
    float2 texCoordy9 : TEXCOORD19;

};

PixelInput VS(VertexInput input)
{
    PixelInput output;
    float pixelSize;


    output.uv = input.uv;
    input.position.w = 1.0f;


    output.position = mul(input.position, _world);
    output.position = mul(output.position, _viewXprojection);
    
    pixelSize = 4.0f / ScreenSize.x;


    output.texCoordx1 = input.uv + float2(pixelSize * -4.0f, 0.0f);
    output.texCoordx2 = input.uv + float2(pixelSize * -3.0f, 0.0f);
    output.texCoordx3 = input.uv + float2(pixelSize * -2.0f, 0.0f);
    output.texCoordx4 = input.uv + float2(pixelSize * -1.0f, 0.0f);
    output.texCoordx5 = input.uv + float2(pixelSize * 0.0f, 0.0f);
    output.texCoordx6 = input.uv + float2(pixelSize * 1.0f, 0.0f);
    output.texCoordx7 = input.uv + float2(pixelSize * 2.0f, 0.0f);
    output.texCoordx8 = input.uv + float2(pixelSize * 3.0f, 0.0f);
    output.texCoordx9 = input.uv + float2(pixelSize * 4.0f, 0.0f);



    pixelSize = 4.0f / ScreenSize.y;


    output.texCoordy1 = input.uv + float2(0.0f, pixelSize * -4.0f);
    output.texCoordy2 = input.uv + float2(0.0f, pixelSize * -3.0f);
    output.texCoordy3 = input.uv + float2(0.0f, pixelSize * -2.0f);
    output.texCoordy4 = input.uv + float2(0.0f, pixelSize * -1.0f);
    output.texCoordy5 = input.uv + float2(0.0f, pixelSize * 0.0f );
    output.texCoordy6 = input.uv + float2(0.0f, pixelSize * 1.0f );
    output.texCoordy7 = input.uv + float2(0.0f, pixelSize * 2.0f );
    output.texCoordy8 = input.uv + float2(0.0f, pixelSize * 3.0f );
    output.texCoordy9 = input.uv + float2(0.0f, pixelSize * 4.0f );

    return output;
}



Texture2D _map : register(t10);


SamplerState samp[3];


float4 PS(PixelInput input) : SV_Target
{
    float weight0, weight1, weight2, weight3, weight4;

    float4 color = 0;//    _map.Sample(samp[0], input.uv).r;
   //return color;

	weight0 = 0.224215f;
    weight1 = 0.201793f;
    weight2 = 0.123318f;
    weight3 = 0.040358f;
    weight4 = 0.022421f;


    color = float4(0.0f, 0.0f, 0.0f, 0.0f);


    color += _map.Sample(samp[0], input.texCoordx1).g * weight4;
    color += _map.Sample(samp[0], input.texCoordx2).g * weight3;
    color += _map.Sample(samp[0], input.texCoordx3).g * weight2;
    color += _map.Sample(samp[0], input.texCoordx4).g * weight1;
    color += _map.Sample(samp[0], input.texCoordx5).g * weight0;
    color += _map.Sample(samp[0], input.texCoordx6).g * weight1;
    color += _map.Sample(samp[0], input.texCoordx7).g * weight2;
    color += _map.Sample(samp[0], input.texCoordx8).g * weight3;
    color += _map.Sample(samp[0], input.texCoordx9).g * weight4;
                                        
                                        
                                        
    color += _map.Sample(samp[0], input.texCoordy1).g * weight4;
    color += _map.Sample(samp[0], input.texCoordy2).g * weight3;
    color += _map.Sample(samp[0], input.texCoordy3).g * weight2;
    color += _map.Sample(samp[0], input.texCoordy4).g * weight1;
    color += _map.Sample(samp[0], input.texCoordy5).g * weight0;
    color += _map.Sample(samp[0], input.texCoordy6).g * weight1;
    color += _map.Sample(samp[0], input.texCoordy7).g * weight2;
    color += _map.Sample(samp[0], input.texCoordy8).g * weight3;
    color += _map.Sample(samp[0], input.texCoordy9).g * weight4;



   
    color.a = 1.0f;

    return color;

}
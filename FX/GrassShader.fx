#include "ShaderPreset.hlsli"


struct VertexInput
{
    float4 position : POSITION0;
    float4 world0 : INSTMATRIX0;
    float4 world1 : INSTMATRIX1;
    float4 world2 : INSTMATRIX2;
    float4 world3 : INSTMATRIX3;
};

struct PixelInput
{
    float4 position : SV_POSITION;
};

PixelInput VS(VertexInput input)
{
    PixelInput output;
    input.position.w = 1;

    float4x4 world = float4x4(input.world0, input.world1, input.world2, input.world3);
    output.position = mul(input.position, world);

    return output;
}


Texture2D _map : register(t0);
SamplerState samp[3];

cbuffer GsData : register(b0)
{
    float _grassHeight;
    float _grassWidth;
    float _timer;
    float _wind;
}


static float rndTable[36] =
{
    0.42, 0.32, 0.82, 0.52, 0.33, 0.18,
    0.04, 0.81, 0.68, 0.68, 0.13, 0.97,
    0.39, 0.43, 0.66, 0.28, 0.46, 0.39,
    0.97, 0.25, 0.78, 0.59, 0.37, 0.05,
    0.67, 0.21, 0.07, 0.78, 0.38, 0.70,
    0.39, 0.27, 0.59, 0.29, 0.85, 0.16
};



[maxvertexcount(256)]
void GS(point PixelInput input[1], inout TriangleStream<PixelInput> triStream)
{
  
 

    float grassheight =  _grassHeight;
    float grasswidth = _grassWidth;
    
    PixelInput base = input[0];

    int rndSeed = base.position.x * 4 + base.position.z * 62;
    rndSeed %= 36;


    float2 uv = input[0].position.xz / 100;
    float height = _map.SampleLevel(samp[0], uv, 0);

    grassheight += height * 0.3f;


    PixelInput output;


    for (int i = 0; i < 6; i++)
    {
        base.position.x += 1.0 / 6;
        base.position.z -= 1;
        for (int j = 0; j < 6; j++)
        {
            base.position.z += 1.0 / 6;
            output = base;

            int rndX, rndZ;
            rndX = (i * 6 + j + rndSeed) % 36;
            rndZ = (i + j * 6 + rndSeed) % 36;

            output.position.xz += float2(rndTable[rndX], rndTable[rndZ]);
            output.position = mul(output.position, _viewXprojection);
           // output.position = mul(output.position, _projection);
            triStream.Append(output);
    
            output = base;
            output.position.xz += float2(rndTable[rndX], rndTable[rndZ]);
            output.position.y += grassheight / 2;
            output.position.x -= grasswidth / 2 + sin(_timer * 3.141592f) * _wind / 2;
            output.position = mul(output.position, _viewXprojection);
          //  output.position = mul(output.position, _projection);
            triStream.Append(output);
    
    
            output = base;
            output.position.xz += float2(rndTable[rndX], rndTable[rndZ]);
            output.position.y += grassheight / 2;
            output.position.x += grasswidth / 2 - sin(_timer * 3.141592f) * _wind / 2;
            output.position = mul(output.position, _viewXprojection);
          //  output.position = mul(output.position, _projection);
    
            triStream.Append(output);
    
            triStream.RestartStrip();
    
            triStream.Append(output);
    
            output = base;
            output.position.xz += float2(rndTable[rndX], rndTable[rndZ]);
            output.position.y += grassheight / 2;
            output.position.x -= grasswidth / 2 + sin(_timer * 3.141592f) * _wind / 2;
            output.position = mul(output.position, _viewXprojection);
           // output.position = mul(output.position, _projection);
            triStream.Append(output);
    
            output = base;
            output.position.xz += float2(rndTable[rndX], rndTable[rndZ]);
            output.position.y += grassheight;
            output.position.x -= sin(_timer * 3.141592f) * _wind;
            output.position = mul(output.position, _viewXprojection);
           // output.position = mul(output.position, _projection);
            triStream.Append(output);
    
    
            triStream.RestartStrip();
        }
    }

}



float4 PS(PixelInput input) : SV_Target
{
    float4 color = float4(0.20784f, 0.38431f, 0.26666f, 1);

   

    return color;

}
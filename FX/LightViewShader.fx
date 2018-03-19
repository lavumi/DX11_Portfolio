#include "ShaderPreset.hlsli"

struct VertexInput
{
    float4 position : POSITION0;

};

struct GeoInput
{
    float4 position : POSITION0;
    float4 worldPosition : TEXCOORD0;
};

struct PixelInput
{
    float4 position : SV_POSITION;
    float4 worldPosition : TEXCOORD0;
    uint RTIndex : TEXCOORD1;
};

GeoInput VS(VertexInput input)
{
    GeoInput output;
    input.position.w = 1;
    output.position = mul(input.position, _world);

    output.position = mul(output.position, _lightView);
    output.position = mul(output.position, _lightProjection);
    output.worldPosition = output.position;


    return output;
}


cbuffer GsData : register(b0)
{
    matrix cropMatrix[3];
}


[maxvertexcount(27)]
void GS(triangle GeoInput input[3], inout TriangleStream<PixelInput> triStream)
{
    for (uint split = 0; split < 3; split++)
    {
        PixelInput output;
        output.RTIndex = split;
        for (int vertex = 0; vertex < 3; vertex++)
        {
            output.position = input[vertex].position;//
           // output.position = mul(input[vertex].position, cropMatrix[split]);
            output.worldPosition = output.position;
            triStream.Append(output);
        }
        triStream.RestartStrip();
    }
}


struct PixelOutput
{
    float4 split0 : SV_TARGET0;
    float4 split1 : SV_TARGET1;
    float4 split2 : SV_TARGET2;
};


PixelOutput PS(PixelInput input)
{
    PixelOutput output;
    float4 depthValue;

    depthValue = input.worldPosition.z / input.worldPosition.w;

    if(input.RTIndex == 0)
        output.split0 = depthValue;
    else if (input.RTIndex == 1)
        output.split1 = 1;
    else if (input.RTIndex == 2)
        output.split2 = 1;


    return output;

}
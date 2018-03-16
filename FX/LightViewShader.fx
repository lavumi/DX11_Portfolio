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
    uint RTIndex : SV_RenderTargetArrayIndex;
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
            output.position = input[vertex].position; //mul(input[vertex].position, cropMatrix[split]);
            output.worldPosition = output.position;
            triStream.Append(output);
        }
        triStream.RestartStrip();
    }
}


float4 PS(PixelInput input) : SV_Target
{
    float depthValue;

    depthValue = input.worldPosition.z / input.worldPosition.w;

    return depthValue;

}
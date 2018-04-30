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
    float4x4 world = float4x4(input.world0, input.world1, input.world2, input.world3);
    output.position = mul(input.position, world);

    output.position = mul(output.position, _lightView);
    output.position = mul(output.position, _lightProjection);
    output.worldPosition = output.position;


    return output;
}


[maxvertexcount(27)]
void GS(triangle GeoInput input[3], inout TriangleStream<PixelInput> triStream)
{
    [unroll(3)] for (uint split = 0; split < 3; split++)
    {
        PixelInput output;
        output.RTIndex = split;
        for (int vertex = 0; vertex < 3; vertex++)
        {
           // output.position = input[vertex].position;
            output.position = mul(input[vertex].position, cropMatrix[split]);
            output.worldPosition = output.position;
            triStream.Append(output);
        }
        triStream.RestartStrip();
    }
}


struct PixelOutput
{
    float4 split0 : SV_TARGET0;
};


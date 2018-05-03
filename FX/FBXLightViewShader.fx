#include "ShaderPreset.hlsli"

struct VertexInput
{
    float4 position : POSITION0;
    float4 boneIndices : BLENDINDICES;
    float4 boneWeights : BLENDWEIGHT;
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

cbuffer BoneBuffer : register(b0)
{
    matrix _boneScale;
    matrix _boneArray[100];
    uint _skinning;
    float3 _bonePadding;
}


GeoInput VS(VertexInput input)
{
    GeoInput output;
    input.position.w = 1;
    if (_skinning == 0)
    {
        output.position = input.position;
    }
    else
    {
        float4x4 skinTransform = 0;
        skinTransform += mul(input.boneWeights.x, _boneArray[(uint) input.boneIndices.x]);
        skinTransform += mul(input.boneWeights.y, _boneArray[(uint) input.boneIndices.y]);
        skinTransform += mul(input.boneWeights.z, _boneArray[(uint) input.boneIndices.z]);
        skinTransform += mul(input.boneWeights.w, _boneArray[(uint) input.boneIndices.w]);
  
        output.position = mul(input.position, skinTransform);
    }
    output.position = mul(output.position, _world);
    //output.position = mul(input.position, _world);

    output.position = MulLightVP(output.position);
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


//PixelOutput PS(PixelInput input)
//{
//    PixelOutput output;
//    float4 depthValue;
//
//    depthValue = input.worldPosition.z / input.worldPosition.w;
//
//
//    output.split0 = depthValue;
//
//    return output;
//
//}
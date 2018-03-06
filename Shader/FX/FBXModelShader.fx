cbuffer MatrixBuffer : register(b12)
{
    matrix _viewXprojection;
};

cbuffer MatrixBuffer : register(b13)
{
    matrix _world;
};
cbuffer Camera : register(b1)
{
    float3 _cameraPosition;
    float _paddd;
}

cbuffer BoneBuffer : register(b2)
{
    float4x4 _boneScale;
    float4x4 _boneArray[100];
    uint _skinning;
    float3 _bonePadding;
}
struct VertexInput
{
    float4 position : POSITION0;
    float3 normal : NORMAL0;
    float3 tangent : TANGENT0;
    float2 uv : TEXCOORD0;
    float4 boneIndices : BLENDINDICES;
    float4 boneWeights : BLENDWEIGHT;

};

struct PixelInput
{
    float4 position : SV_POSITION;
    float3 normal : NORMAL0;
    float3 tangent : TANGENT0;
    float2 uv : TEXCOORD0;
    float3 viewDirection : TEXCOORD1;

};

PixelInput VS(VertexInput input)
{
    PixelInput output;
    float4 worldPosition;

    if (_skinning == 0)
    {
        output.position = input.position;
        output.normal = input.normal;
    }
    else
    {
        float4x4 skinTransform = 0;
        skinTransform += mul(input.boneWeights.x, _boneArray[(uint) input.boneIndices.x]);
        skinTransform += mul(input.boneWeights.y, _boneArray[(uint) input.boneIndices.y]);
        skinTransform += mul(input.boneWeights.z, _boneArray[(uint) input.boneIndices.z]);
        skinTransform += mul(input.boneWeights.w, _boneArray[(uint) input.boneIndices.w]);

        output.position = mul(input.position, skinTransform);
        output.normal = normalize(mul(input.normal, (float3x3) skinTransform));
        output.tangent = normalize(mul(input.tangent, (float3x3) skinTransform));
    }
	
    worldPosition = mul(output.position, _world);

    output.position = mul(worldPosition, _viewXprojection);
   // output.position = mul(output.position, _projection);

    output.normal = mul(output.normal, (float3x3) _world);
    output.normal = normalize(output.normal);

    output.tangent = mul(output.normal, (float3x3) _world);
    output.tangent = normalize(output.normal);

    output.viewDirection = _cameraPosition.xyz - worldPosition.xyz;
    output.viewDirection = normalize(output.viewDirection);

    output.uv = input.uv;

    return output;
}



Texture2D _map : register(t0);


SamplerState samp[3];


float4 PS(PixelInput input) : SV_Target
{
    float4 color =  _map.Sample(samp[0], input.uv);

   

    return  color;

}
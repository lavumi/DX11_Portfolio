#include "ShaderPreset.hlsli"



cbuffer BoneBuffer : register(b0)
{
    matrix _boneScale;
    matrix _boneArray[100];
    uint _skinning;
    float3 _cameraPosition;
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
    float2 uv : TEXCOORD0;
    float3 halfVector : TEXCOORD1;
    float3 lightDir : TEXCOORD2;
    float3 viewDir : TEXCOORD3;
    float4 viewPosition : TEXCOORD5;

};

PixelInput VS(VertexInput input)
{
    PixelInput output;
    float4 worldPosition;

    float3 normal, tangent;
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
        normal = normalize(mul(input.normal, (float3x3) skinTransform));
        tangent = normalize(mul(input.tangent, (float3x3) skinTransform));
    }
   // output.position = input.position;
    output.position = mul(output.position, _world);

    float3 viewDir = _cameraPosition.xyz - output.position.xyz;
    float3 halfVector = normalize(-_lightDir) + normalize(viewDir);


    float3 n = mul(normal, (float3x3) _world);
    float3 t = mul(tangent, (float3x3) _world);
  float3 b = cross(n, t);
  float3x3 tbnMatrix = float3x3(t.x, b.x, n.x,
                            t.y, b.y, n.y,
                            t.z, b.z, n.z);
  //

    output.halfVector = mul(halfVector, tbnMatrix);
    output.lightDir = mul(-_lightDir, tbnMatrix);
    output.viewDir = mul(viewDir, tbnMatrix);

   
    output.position = mul(output.position, _viewXprojection);
   // output.position = mul(output.position, _projection);
   
    output.viewPosition = output.position;

    output.uv = input.uv;

    return output;
}



Texture2D _map : register(t0);
Texture2D _normalMap : register(t1);
Texture2D _lightMap : register(t13);

SamplerState samp[3];


cbuffer PSBuffer : register(b0)
{
    float4 ambient;
    float4 diffuse;
    float4 specular;
    float4 globalAmbient;
    float shininess;
};



float4 PS(PixelInput input) : SV_Target
{
    float4 diffuseMap =_map.Sample(samp[0], input.uv);


    float3 halfVector = normalize(input.halfVector);


    float3 normal = _normalMap.Sample(samp[0], input.uv).rgb;

    normal = (normal * 2.0f) - 1.0f;
 
    normal = normalize(normal);

    float3 light = normalize(input.lightDir);

    float nDotL = saturate(dot(normal, light));
    float nDotH = saturate(dot(normal, halfVector));

    float power = pow(nDotH, shininess);

    float4 intensity = ambient * globalAmbient + diffuse * nDotL + specular * power;



    float2 projectTexCoord;

    projectTexCoord.x = input.viewPosition.x / input.viewPosition.w / 2.0f + 0.5f;
    projectTexCoord.y = -input.viewPosition.y / input.viewPosition.w / 2.0f + 0.5f;
    float shadowValue = _lightMap.Sample(samp[1], projectTexCoord).g;
   // shadowValue *= 0.3f;
    intensity *= shadowValue;


    return intensity * diffuseMap;

}
#include "ShaderPreset.hlsli"





struct VertexInput
{
    float4 position : POSITION0;
};

struct PixelInput
{
    float4 position : SV_POSITION;
    float4 wPosition : TEXCOORD0;
    float3 vertexPos : TEXCOORD2;

};

PixelInput VS(VertexInput input)
{
    PixelInput output;

    input.position.w = 1.0f;

    output.vertexPos = input.position.xyz;


    output.position = mul(input.position, _world);
    output.position = mul(output.position, _viewXprojection);
   // output.position = mul(output.position, _projection);

    output.wPosition = input.position;

    return output;
}



//float2 getUVbyCircle(float3 pixelVector, float3 uVector, float3 vVector)
//{
//    float3 uvPlaneNormal = cross(uVector, vVector);
//    uvPlaneNormal = normalize(uvPlaneNormal);

//    float3 projectedVector = pixelVector - dot(pixelVector, uvPlaneNormal) * uvPlaneNormal;
    
//   // float length = length(projectedVector);
//    float u = dot(projectedVector, uVector);
//    float v = dot(projectedVector, vVector);

//    if (u > 1)
//        u = 0;
//    if (v > 1)
//        v = 0;


//    return float2(u, v);
//}




Texture2D map;

float4 PS(PixelInput input) : SV_TARGET0
{
    float height = saturate(input.wPosition.y);
    float3 lightPos = _lightDir;
    float3 vertexPos = normalize(input.vertexPos);

    float lightVertexCos = dot(lightPos, vertexPos);
    float vertexGroundCos = dot(vertexPos, float3(0, 1, 0));
    float lightGroundCos = dot(float3(0, 1, 0), lightPos);

    float3 lightRGB;

    //산란되는 빛의 양
    float scatteredLight = 0.2f;// abs(vertexGroundCos) + 0.2f;

    lightVertexCos = saturate(lightVertexCos);

    //r 빛은 산란이 거의 되지 않고 다 직접 광으로 들어온다.
    lightRGB.r = pow(lightVertexCos, 15) ; 

    //파란 빛은 사실상 거의 산란된 빛들
     //산란되는 양을 고정시키고 빛의 각도에 따라 파란빛을 추가시킴
    //TODO : 지금 파란 빛은 페이크가 심함. 더 좋은 방법이 있을까?
    lightRGB.b = scatteredLight + lightGroundCos;

    lightRGB.g = (lightRGB.r + lightRGB.b) /2;
    
      
    lightRGB = saturate(lightRGB);


    float4 result = float4(lightRGB, 1); //lerp(_center, _apex, height);
      


    //달 그리기
    lightVertexCos = dot(-lightPos, vertexPos);

    float moonColor = rndTable[vertexGroundCos*100] + 0.4f;
    result = lerp(0.4f, result, step(lightVertexCos, 0.999f));

 //
 //   float3 moonPos2Vertex = vertexPos - lightPos;
 //
 // //  moonPos2Vertex =  normalize(moonPos2Vertex);
 //   float2 uv = getUVbyCircle(moonPos2Vertex * 1000, float3(0, 0, 1), float3(0, 1, 0));
 //
 //  //  lightVertexCos = dot(lightPos, vertexPos);
 //   result = lerp(0.5f, float4(uv, 0, 1), step(lightVertexCos, 0.999f));

    
    return result;
}
#include "ShaderPreset.hlsli"


struct VertexInput
{
    float4 position : POSITION0;
    float2 world0 : INSTMATRIX0;
};

struct GeoInput
{
    float4 position : SV_POSITION;
 
};



struct PixelInput
{
    float4 position : SV_POSITION;
    float4 screenPosition : TEXCOORD0;
    float ColorTable : TEXCOORD1;
    float clip0 : SV_ClipDistance0;
    float clip1 : SV_ClipDistance1;
 //   float clip2 : SV_ClipDistance2;
};

GeoInput VS(VertexInput input)
{
    GeoInput output;
    input.position.w = 1;

    output.position = input.position;
    output.position.xz += input.world0.xy;
    return output;
}


Texture2D _map : register(t0);
SamplerState samp[3];

cbuffer GsData : register(b0)
{
    float _grassHeight;
    float _grassWidth;
    float _timer;
    //TODO : GrassBuffer 확인해보자 왜 float 1개만큼 밀려났을까
    float4 _characterPos;
    float2 _wind;
}



[maxvertexcount(6)]
void GS(point GeoInput input[1], inout TriangleStream<PixelInput> triStream)
{
    float grassheight = _grassHeight;
    float grasswidth = _grassWidth;
    
    PixelInput base;
    base.position = input[0].position;

    int rndx, rndz;
    rndx = base.position.x * 399;
    rndz = base.position.z * 928;
    int rnd = rndx + rndz;


    rnd %= 100;
    base.ColorTable = rndTable[rnd]; // saturate(rndTable[rnd] * 0.3f + 0.7f);


    //높이값 설정    //이미지에 대한 uv 좌표 처리
    float2 heightuv = base.position.xz / ImageSize;
    heightuv.y *= -1;
    heightuv.y += 1;
    base.position.y = _map.SampleLevel(samp[0], heightuv, 0).r * 64.0f  - 26.2f;

    if (base.position.y < -7 || base.position.y > 0)
        return;
    base.clip0 = -dot(base.position, frustumPlane[0]);
    base.clip1 = dot(base.position, frustumPlane[1]) * dot(base.position, frustumPlane[2]);



  //  base.clip2 = 0;//    -;
    //풀을 표현할 벡터
    float3 heightVector, widthVector;
    heightVector.x = (float) (rndx % 3) - 1;
    heightVector.y = (float) (rndx % 7) + 3;
    heightVector.z = (float) (rndz % 3) - 1;
    heightVector = normalize(heightVector);
   

    heightVector = normalize(heightVector);

    widthVector.z = 1;
    widthVector.y = (float) (rndx % 5);
    widthVector.x = (-heightVector.z - widthVector.y * heightVector.y) / heightVector.x;
    widthVector = normalize(widthVector);


    heightVector *= _grassHeight/2;
    widthVector *= _grassWidth/2;


    PixelInput root, left, right, top;

    root = left = right = top = base;

    float3 leftVector, rightVector, topVector;

    leftVector = heightVector - widthVector;
    rightVector = heightVector + widthVector;
    topVector = heightVector * 2;




    //캐릭터 위치에 따른 기울어짐
    float3 characterToRoot = root.position.xyz - _characterPos.xyz;
    float3 rotationAxisVector = cross(characterToRoot, float3(0, 1, 0));
    rotationAxisVector = normalize(rotationAxisVector);
    float rotationDegree = (1 - saturate((characterToRoot.x * characterToRoot.x + characterToRoot.z * characterToRoot.z)*0.2f)) * 1.570796f * 0.4f;
    
    float3x3 rotationMatrix = RotationMatrix(rotationAxisVector, rotationDegree);
    float3x3 rotationMatrixHalf = RotationMatrix(rotationAxisVector, rotationDegree *0.8f);

    leftVector = mul(leftVector, rotationMatrix);
    rightVector = mul(rightVector, rotationMatrix);
    topVector = mul(topVector, rotationMatrixHalf);



    /****************************************************************************
    *******************************바람 영향 계산*********************************
    float windForceTheta = pow(abs(_timer - 0.5f), 2) * 3.141592f * 0.1f;

    float3x3 windRotation = RotationMatrix(float3(0, 0, 1), windForceTheta);
    float3x3 windRotation2 = RotationMatrix(float3(0, 0, 1), windForceTheta * 2);

    leftVector = mul(leftVector, windRotation);
    rightVector = mul(rightVector, windRotation);
    topVector = mul(topVector, windRotation2);
    *****************************************************************************
    *****************************************************************************/



    left.position.xyz += leftVector;
    right.position.xyz += rightVector;
    top.position.xyz += topVector;
   


    root.position = MulVP(root.position);
    left.position = MulVP(left.position);
    right.position = MulVP(right.position);
    top.position = MulVP(top.position);


    root.screenPosition = root.position;
    left.screenPosition = left.position;
    right.screenPosition = right.position;
    top.screenPosition = top.position;
    

    triStream.Append(root);
    triStream.Append(left);
    triStream.Append(right);
    
    triStream.RestartStrip();
    

    triStream.Append(right);
    triStream.Append(left);
    triStream.Append(top);
    
    triStream.RestartStrip();

}

Texture2D _lightMap : register(t13);
float4 PS(PixelInput input) : SV_Target
{
    //float4 color = float4(0.7137f, 0.90981f, 0.4156f, 1);
    float4 color = float4(0.20784f, 0.48431f, 0.26666f, 1);
   // float4 color2 = float4(0.22784f, 0.44431f, 0.24666f, 1);
    float4 color2 = float4(0.88f, 0.87f, 0.6f, 1);

    color = lerp(color, color2, input.ColorTable) * 0.7f;
   // color *= input.ColorTable;


    float2 projectTexCoord;
    projectTexCoord.x = input.screenPosition.x / input.screenPosition.w / 2.0f + 0.5f;
    projectTexCoord.y = -input.screenPosition.y / input.screenPosition.w / 2.0f + 0.5f;
    float shadowValue = _lightMap.Sample(samp[1], projectTexCoord).g;
    color *= shadowValue;
   
    color *= globalAmbient;



    return color;

}

/*****************************************************************************************************************************
//이전 버젼 
[maxvertexcount(6)]
void GS2(point PixelInput input[1], inout TriangleStream<PixelInput> triStream)
{
  

    float grassheight = _grassHeight;
    float grasswidth = _grassWidth;
    
    PixelInput base = input[0];
    int rnd = base.position.x * 582 + base.position.y * 582 + base.position.z * 928;
    rnd %= 100;
    base.ColorTable = saturate(rndTable[rnd] * 0.3f + 0.7f);


    float2 heightuv = base.position.xz / 256;
    //이미지에 대한 uv 좌표 처리
    heightuv.y *= -1;
    heightuv.y += 1;
    
    base.position.y = _map.SampleLevel(samp[0], heightuv, 0).r * 34.0f - 18.2f;



    PixelInput root, left, right, top;

    root = left = right = top = base;

    ///////////////
    left.position.y += grassheight / 2;
    left.position.x -= grasswidth / 2 * cos(rnd * 0.01f * 3.141592f * 2) + sin(_timer * 3.141592f) * _wind.x / 2;
    left.position.z += grasswidth / 2 * sin(rnd * 0.01f * 3.141592f * 2) + sin(_timer * 3.141592f) * _wind.x / 2;
    ///////////////

    ///////////////
    right.position.y += grassheight / 2;
    right.position.x += grasswidth / 2 * cos(rnd * 0.01f * 3.141592f * 2) - sin(_timer * 3.141592f) * _wind.x / 2;
    right.position.z -= grasswidth / 2 * sin(rnd * 0.01f * 3.141592f * 2) - sin(_timer * 3.141592f) * _wind.x / 2;
    ///////////////

    ///////////////
    top.position.y += grassheight;
    top.position.x -= sin(_timer * 3.141592f) * _wind.x;
    top.position.z += sin(_timer * 3.141592f) * _wind.x;
    ///////////////


    
    root.position = mul(root.position, _viewXprojection);
    left.position = mul(left.position, _viewXprojection);
    right.position = mul(right.position, _viewXprojection);
    top.position = mul(top.position, _viewXprojection);


    root.screenPosition = root.position;
    left.screenPosition = left.position;
    right.screenPosition = right.position;
    top.screenPosition = top.position;




    triStream.Append(root);
    triStream.Append(left);
    triStream.Append(right);
    
    triStream.RestartStrip();
    

    triStream.Append(right);
    triStream.Append(left);
    triStream.Append(top);
    
    triStream.RestartStrip();
}

**************************************************//////
cbuffer VSBuffer : register(b0)
{
    float4 clipPlane;
    matrix worldInverseTransposeMatrix;
    float3 _lightDir;
};


cbuffer MatrixBuffer : register(b12)
{
    matrix _viewXprojection;
};

cbuffer MatrixBuffer : register(b13)
{
    matrix _world;
};


struct VertexInput
{
    float4 position : POSITION0;
    float2 uv : TEXCOORD0;
    float3 normal : NORMAL0;
    float3 tangent : NORMAL1;

};

struct PixelInput
{
    float4 position : SV_POSITION;
    float2 uv : TEXCOORD0;
    float worldPosition : TEXCOORD1;
    float3 lightDir : TEXCOORD2;
    float3 normal : NORMAL0;
    float3 worldNormal : NORMAL1;
    
    float3 viewPosition : TEXCOORD5;
   
    float4 clip :    SV_ClipDistance0;
};




PixelInput VS(VertexInput input)
{



    PixelInput output;

    
    input.position.w = 1.0f;

    output.position = mul(input.position, _world);
    output.worldPosition = output.position.y;
    output.position = mul(output.position, _viewXprojection);
   // output.position = mul(output.position, _projection);
   

   
     output.viewPosition = output.position.xyw;

    output.uv = input.uv;

    float3 n = mul(input.normal, (float3x3) worldInverseTransposeMatrix);
    float3 t = mul(input.tangent, (float3x3) worldInverseTransposeMatrix);
    float3 b = cross(n, t);

    float3x3 tbnMatrix = float3x3(t.x, b.x, n.x,
	                            t.y, b.y, n.y,
	                            t.z, b.z, n.z);


    output.lightDir = normalize(mul(-_lightDir, tbnMatrix));



    output.uv = input.uv;
    output.normal = normalize(mul(input.normal, tbnMatrix));
    output.worldNormal = abs(input.normal);


  //  output.clip = dot(mul(input.position, _world), clipPlane);
    output.clip = clipPlane;
    return output;

}









Texture2D _map[3] : register(t10);

Texture2D _lightMap : register(t1);
Texture2D _normalMap : register(t2);

SamplerState samp[3];


cbuffer PSBuffer : register(b0)
{
    float4 ambient;
    float4 diffuse;
    float4 globalAmbient;
    //   matrix cameraSpace;
};



struct PixelOut
{
    float4 albedo : SV_TARGET0;
    float4 depthMap : SV_TARGET1;
    float4 viewSpaceVector : SV_TARGET2;
};

PixelOut PS(PixelInput input)
{
    
    //float ddx = fwidth(input.worldPosition.z);
    //return ddx;
    PixelOut output;



    float2 uv = input.uv;

    float3 normal =  _normalMap.Sample(samp[0], uv).rgb;
    normal = (normal * 2.0f) - 1.0f;
    normal = normalize(normal);

    float3 light = normalize(input.lightDir);
    float nDotL = saturate(dot(normal, light));
    float4 intensity = ambient * globalAmbient + diffuse * nDotL;



    //그림자
    float2 projectTexCoord;
    projectTexCoord.x = input.viewPosition.x / input.viewPosition.z / 2.0f + 0.5f;
    projectTexCoord.y = -input.viewPosition.y / input.viewPosition.z / 2.0f + 0.5f;
    
    float shadowValue = _lightMap.Sample(samp[1], projectTexCoord).g*0.3f;
    intensity *= shadowValue;





    //텍스쳐링
    float3 blending = input.worldNormal;
    blending.y -= (blending.x + blending.z) / 2;
    blending = normalize(max(blending, 0.0001f));
   
    float4 Mountain = _map[2].Sample(samp[0], uv);
    float4 Grass = _map[0].Sample(samp[0], uv);
   
    float4 landNmountain = Mountain * blending.x + Grass * blending.y + Mountain * blending.z;



    float blendFactor = saturate((input.worldPosition + 7.7f) / 0.4f);

    float4 diffuseMap = lerp(_map[1].Sample(samp[0], uv), landNmountain, blendFactor);

   // diffuseMap.a = input.clip;




 

    output.albedo = intensity * diffuseMap;

    float3 depthValue =  input.position.w / 300;
    output.depthMap = float4(depthValue, 1);


   //output.viewSpaceVector.rgb = mul(input.worldNormal, (float3x3) cameraSpace);
   //
   //output.viewSpaceVector.a = 1;
    return output;
    
}


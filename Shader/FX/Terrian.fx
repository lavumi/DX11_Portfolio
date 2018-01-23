cbuffer MatrixBuffer : register(b0)
{
    matrix _world;
    matrix _view;
    matrix _projection;

};

cbuffer Camera : register(b1)
{
    float3 _cameraPosition;
    float _paddd;
}

cbuffer LightBuffer : register(b2)
{
    matrix _lightView;
    matrix _lightProjection;
    float3 _lightDir;
    float _baseLight;
};


cbuffer ReflectionPlane : register(b11)
{
    float4 clipPlane;
};


cbuffer ExtraBuffer : register(b3)
{
    matrix worldInverseTransposeMatrix;
}

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
    float3 worldPosition : TEXCOORD1;
    float3 lightDir : TEXCOORD2;
    float3 normal : NORMAL0;
    float3 worldNormal : NORMAL1;
    
    float4 viewPosition : TEXCOORD5;

    float clip : SV_ClipDistance0;
};




PixelInput VS(VertexInput input)
{



    PixelInput output;

    
    input.position.w = 1.0f;

    output.position = mul(input.position, _world);
    output.worldPosition = output.position.xyz;
    output.position = mul(output.position, _view);
    output.position = mul(output.position, _projection);
   
    output.viewPosition = output.position;

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
    output.worldNormal = input.normal;


    output.clip = dot(mul(input.position, _world), clipPlane);

    return output;

}





Texture2D _map[3] : register(t10);

Texture2D _lightMap : register(t1);
Texture2D _normalMap : register(t2);

SamplerState samp[3];


cbuffer Material : register(b0)
{
    float4 ambient;
    float4 diffuse;
    float4 globalAmbient;
   
};

float4 PS(PixelInput input) : SV_TARGET
{


    float2 uv = input.uv;

    float3 normal =  _normalMap.Sample(samp[0], uv).rgb;
    normal = (normal * 2.0f) - 1.0f;
    normal = normalize(normal);

    float3 light = normalize(input.lightDir);
    float nDotL = saturate(dot(normal, light));
    float4 intensity = ambient * globalAmbient + diffuse * nDotL;



    //그림자
    float2 projectTexCoord;
    projectTexCoord.x = input.viewPosition.x / input.viewPosition.w / 2.0f + 0.5f;
    projectTexCoord.y = -input.viewPosition.y / input.viewPosition.w / 2.0f + 0.5f;
    
    float shadowValue = _lightMap.Sample(samp[1], projectTexCoord).g*0.3f;
    intensity *= shadowValue;





    //텍스쳐링
    float3 blending = abs(input.worldNormal);
    blending.y -= (blending.x + blending.z) / 2;
    blending = normalize(max(blending, 0.0001f));
   
    float4 Mountain = _map[2].Sample(samp[0], uv);
    float4 Grass = _map[0].Sample(samp[0], uv);
   
    float4 landNmountain = Mountain * blending.x + Grass * blending.y + Mountain * blending.z;



    float blendFactor = saturate((input.worldPosition.y + 7.7f) / 0.4f);
    float4 diffuseMap = lerp(_map[1].Sample(samp[0], uv), landNmountain, blendFactor);



    return intensity * diffuseMap;
}


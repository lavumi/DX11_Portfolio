cbuffer CameraBuffer : register(b0)
{
    matrix _view;
    matrix _projection;
    float3 _cameraPosition;
    float _paddd;
};

cbuffer WorldBuffer : register(b1)
{
    matrix _world;
}

cbuffer LightBuffer : register(b2)
{
    matrix _lightView;
    matrix _lightProjection;
    float3 _lightDir;
    float _baseLight;
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
    float3 lightDir : TEXCOORD2;
    float3 normal : NORMAL0;
    float4 viewPosition : TEXCOORD5;


};




PixelInput VS(VertexInput input)
{



    PixelInput output;

    
    input.position.w = 1.0f;

    output.position = mul(input.position, _world);
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
  

    return output;

}





Texture2D _map : register(t0);

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

   // return float4(light, 1);

    float4 intensity = ambient * globalAmbient + diffuse * nDotL;

        




    float2 projectTexCoord;
    projectTexCoord.x = input.viewPosition.x / input.viewPosition.w / 2.0f + 0.5f;
    projectTexCoord.y = -input.viewPosition.y / input.viewPosition.w / 2.0f + 0.5f;
    
    float shadowValue = _lightMap.Sample(samp[1], projectTexCoord).g*0.3f;
    intensity *= shadowValue;



    float4 diffuseMap =  _map.Sample(samp[0], uv);
    return intensity * diffuseMap;
}


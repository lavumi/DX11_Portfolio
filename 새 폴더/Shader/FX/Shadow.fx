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
    float4 lightWorldPosition : TEXCOORD0;
    float3 normal : TEXCOORD1;
    float3 lightDir : TEXCOORD2;
};




PixelInput VS(VertexInput input)
{
    PixelInput output;

    input.position.w = 1.0f;

    output.position = mul(input.position, _world);

    output.lightWorldPosition = output.position;
    output.lightWorldPosition = mul(output.lightWorldPosition, _lightView);
    output.lightWorldPosition = mul(output.lightWorldPosition, _lightProjection);

    output.position = mul(output.position, _view);
    output.position = mul(output.position, _projection);

    output.normal = input.normal;
    output.lightDir = -_lightDir;


    return output;

}





Texture2D _lightMap : register(t0);

SamplerState samp[3];



float4 PS(PixelInput input) : SV_TARGET
{    
 

    float3 normal = normalize(input.normal);
    float3 lightDir = normalize(input.lightDir);
  //  float3 light = normalize(input.lightDir);


    float shadow = 0.3;
    float2 projectTexCoord;
    float lightIntensity;
    projectTexCoord.x = input.lightWorldPosition.x / input.lightWorldPosition.w / 2.0f + 0.5f;
    projectTexCoord.y = -input.lightWorldPosition.y / input.lightWorldPosition.w / 2.0f + 0.5f;




    float depthValue;
    float lightDepthValue;
    if ((saturate(projectTexCoord.x) == projectTexCoord.x) && (saturate(projectTexCoord.y) == projectTexCoord.y))
    {
        depthValue = _lightMap.Sample(samp[0], projectTexCoord).r;

        lightDepthValue = input.lightWorldPosition.z / input.lightWorldPosition.w;

        lightDepthValue = lightDepthValue - 0.01;

        if (lightDepthValue < depthValue)
        {
            lightIntensity = saturate(dot(normal, lightDir));

            if (lightIntensity > 0.01f)
            {
               shadow = 1;
            }
           
        }
    }


   

    return float4(shadow, shadow, shadow, 1);
}















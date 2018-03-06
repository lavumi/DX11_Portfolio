
cbuffer MatrixBuffer : register(b13)
{
    matrix _world;
};
cbuffer MatrixBuffer : register(b12)
{
    matrix _viewXprojection;
};





cbuffer VS_buffer : register(b0)
{
    matrix _lightView;
    matrix _lightProjection;
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
};




PixelInput VS(VertexInput input)
{
    PixelInput output;

    input.position.w = 1.0f;

    output.position = mul(input.position, _world);

    output.lightWorldPosition = output.position;
    output.lightWorldPosition = mul(output.lightWorldPosition, _lightView);
    output.lightWorldPosition = mul(output.lightWorldPosition, _lightProjection);

    output.position = mul(output.position, _viewXprojection);
 //   output.position = mul(output.position, _projection);

    output.normal = input.normal;



    return output;

}





Texture2D _lightMap : register(t0);

SamplerState samp[3];

cbuffer PS_buffer : register(b0)
{
    float4 _lightDir;
};


float4 PS(PixelInput input) : SV_TARGET
{    
 
    //return lightDir;
    float3 normal = normalize(input.normal);
    float3 lightDir = normalize(-_lightDir.xyz);


    float shadow = 0.3f;
    float2 projectTexCoord;
  
    projectTexCoord.x = input.lightWorldPosition.x / input.lightWorldPosition.w / 2.0f + 0.5f;
    projectTexCoord.y = -input.lightWorldPosition.y / input.lightWorldPosition.w / 2.0f + 0.5f;




    float depthValue;
    float lightDepthValue;
    float lightIntensity;

    //라이트 텍스쳐가 존재하는 부분만 그림자 처리 연산
    if ((saturate(projectTexCoord.x) == projectTexCoord.x) && (saturate(projectTexCoord.y) == projectTexCoord.y))
    {
        depthValue = _lightMap.Sample(samp[2], projectTexCoord).r;

        lightDepthValue = input.lightWorldPosition.z / input.lightWorldPosition.w;


        //TODO : bias 외부에서 받아서 셋팅할수 있게 하자
        lightDepthValue = lightDepthValue - 0.01f;

        if (lightDepthValue < depthValue)
        {
           lightIntensity = saturate(dot(normal, lightDir.xyz));
          
           if (lightIntensity > 0.01f)
           {
               shadow = 1;
            }
           
        }
    }
    else
        shadow = 1;




   

    return float4(shadow, shadow, shadow, 1);
}















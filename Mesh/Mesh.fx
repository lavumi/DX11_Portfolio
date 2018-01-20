

cbuffer CameraBuffer : register(b0)
{
    matrix _view;
    matrix _projection;
}

cbuffer WorldBuffer : register(b1)
{
    matrix _world;
};

cbuffer SunBuffer : register(b0)
{
    float3 _lightDir;
    float _padding;
};



struct VertexInput
{
    float4 position : POSITION0;
    float2 uv : TEXCOORD0;
    float3 normal : NORMAL0;
};

struct PixelInput
{
    float4 position : SV_POSITION;
    float4 wPosition : TEXCOORD0;
    float3 normal : NORMAL0;
   
};

PixelInput VS(VertexInput input)
{
    PixelInput output;

    input.position.w = 1.0f;
   
    output.position = mul(input.position, _world);
    output.position = mul(output.position, _view);
    output.position = mul(output.position, _projection);

    output.normal = input.normal;
    output.wPosition = input.position;




    return output;
}


float4 PS(PixelInput input) : SV_Target
{

    float height = saturate(input.wPosition.y);
    float4 result = lerp(float4(1, 1, 0, 1), float4(1, 0, 0, 1), height);


    //float4 result = float4(0.7, 0.7, 0.7, 1);
    //float3 light = -_lightDir;
    //float intensity = saturate(dot(input.normal, light));
    //result += float4(1, 1, 1, 1) * intensity;
    //result = saturate(result);

    return result;
}
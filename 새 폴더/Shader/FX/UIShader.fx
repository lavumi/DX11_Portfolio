Texture2D _map;
SamplerState samp;

cbuffer CameraBuffer : register(b0)
{
    matrix _view;
    matrix _projection;
}

cbuffer WorldBuffer : register(b1)
{
    matrix _world;
};

cbuffer UIBuffer : register(b2)
{
    float2 _UIposition;
    float _UIscale;
    float __padding;
}


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
    float2 uv : TEXCOORD0;
    float3 normal : NORMAL0;
   
};

PixelInput VS(VertexInput input)
{
    PixelInput output;

    output.position = input.position ;
    

    float scaleFactor = 1 / _UIscale;

    output.position.w = scaleFactor;
    output.position.x -= scaleFactor;
    output.position.y -= scaleFactor+1;
    output.position.xy += scaleFactor*2*_UIposition;

    output.uv = input.uv;



    return output;
}


float4 PS(PixelInput input) : SV_Target
{
    
    float4 diffuseMap = _map.Sample(samp, input.uv);

    float4 BlackFrame = lerp(float4(0, 0, 0, 1), diffuseMap, step(input.uv.x, 0.99f));
    BlackFrame = lerp(float4(0, 0, 0, 1), BlackFrame, step(0.01f, input.uv.x));
    BlackFrame = lerp(float4(0, 0, 0, 1), BlackFrame, step(input.uv.y, 0.99f));
    BlackFrame = lerp(float4(0, 0, 0, 1), BlackFrame, step(0.01f, input.uv.y));





    return BlackFrame;

}
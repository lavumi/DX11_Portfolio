cbuffer MatrixBuffer : register(b13)
{
    matrix _world;
};
cbuffer MatrixBuffer : register(b12)
{
    matrix _viewXprojection;
};

cbuffer Camera : register(b1)
{
    float3 _cameraPosition;
    float _paddd;
}

struct VertexInput
{
    float4 position : POSITION0;
    float2 uv : TEXCOORD0;

};

struct PixelInput
{
    float4 position : SV_POSITION;
    float2 uv : TEXCOORD0;

};

PixelInput VS(VertexInput input)
{
    PixelInput output;
    input.position.w = 1;
    output.position = mul(input.position, _world);
    output.position = mul(output.position, _viewXprojection);


    output.uv = input.uv;


    return output;
}



Texture2D _map : register(t0);


SamplerState samp[3];


float4 PS(PixelInput input) : SV_Target
{
    float4 color = _map.Sample(samp[0], input.uv);

   

    return  color;

}
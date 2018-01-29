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

struct VertexInput
{
    float4 position : POSITION0;
    float2 uv : TEXCOORD0;
    float4 world0 : INSTMATRIX0;
    float4 world1 : INSTMATRIX1;
    float4 world2 : INSTMATRIX2;
    float4 world3 : INSTMATRIX3;
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

    float4x4 world = float4x4(input.world0, input.world1, input.world2, input.world3);

    output.position = mul(input.position, world);
    output.position = mul(output.position, _view);
    output.position = mul(output.position, _projection);

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
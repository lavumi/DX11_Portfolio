cbuffer MatrixBuffer : register(b0)
{
    matrix _world;
    matrix _view;
    matrix _projection;

};


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
    output.position = mul(output.position, _view);
    output.position = mul(output.position, _projection);

    output.uv = input.uv;


    return output;
}



Texture2D _map : register(t0);

cbuffer color : register(b0)
{
    float4 inputColor;
};



float4 PS(PixelInput input) : SV_Target
{
    float4 color = inputColor;

    return  color;

}
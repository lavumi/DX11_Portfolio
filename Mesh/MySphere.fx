Texture2D _map[2] : register(t0);
SamplerState _samp : register(s0);

cbuffer VS_Matrix : register(b0)
{
    matrix _world;
    matrix _view;
    matrix _projection;
};

cbuffer PS_Light : register(b0)
{
    float3 _lightDir;
    float  _padding;
};

cbuffer PS_Material : register(b1)
{
    float4 _ambient;
    float4 _diffuse;
};

cbuffer Blend : register(b2)
{
    float _selected;
    float _linearblend;
    float _gammacorrection;
    float _padding2;
};

struct VertexInput
{
    float4 position : POSITION0;
    float4 color : COLOR0;
    float3 normal : NORMAL0;
    float2 uv : TEXCOORD0;
};

struct PixelInput
{
    float4 position : SV_POSITION;
    float4 color : COLOR0;
    float3 normal : NORMAL0;
    float2 uv : TEXCOORD0;
};

PixelInput VS(VertexInput input)
{
    PixelInput output;

    output.position = mul(input.position,  _world);
    output.position = mul(output.position, _view);
    output.position = mul(output.position, _projection);

    output.normal = mul(input.normal, (float3x3) _world);
    output.normal = normalize(output.normal);

    output.uv = input.uv;
    output.color = input.color;

    return output;
}

float4 PS(PixelInput input) : SV_TARGET
{
    float4 diffuseMap = _map[0].Sample( _samp, input.uv);
   // float4 diffuseMap2 = _map[1].Sample(_samp, input.uv);

    float4 result = _ambient;

    float3 light = -_lightDir;
    float intensity = saturate(dot(input.normal, light));

    float added = lerp(_diffuse * intensity, 0, step(intensity, 0));
    result += added;
    
    result = saturate(result) * float4(0.2, 0.2, 0.4, 0.3);
    return result;
}
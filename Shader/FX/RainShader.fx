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
SamplerState samp[3];

cbuffer time : register(b0)
{
    float4 time;
};


float rain(float2 uv_input, float scale)
{
    float2 uv = uv_input;
    uv.y -= time.a /scale;
    uv *= scale*3;
    uv *= 5;
    float2 uv_int = ceil(uv);
    uv = frac(uv);
    
    uv -= 0.5;
    uv.x *= 100 / 4;
    uv.x += sin(uv_int.x * uv_int.y + uv_int.x)*scale*3;

    float circle = lerp(0, 1, (0.3 - (uv.x * uv.x + uv.y * uv.y)));
    circle = saturate(circle);

    return circle * 0.5;
}

float4 PS(PixelInput input) : SV_Target0
{
    float2 uv = input.uv;
    uv = frac(uv);
    float4 mapdist = _map.Sample(samp[0], uv);
  

    
    float c = 0;
   // c += rain(uv, 5.);

    mapdist.r *= 10;

    for (float i = 1; i <4; i++)
    {
        c += rain(uv, i * 2 + 1);
    }

    return c;
}
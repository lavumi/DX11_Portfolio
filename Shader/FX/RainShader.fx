cbuffer MatrixBuffer : register(b10)
{
    matrix _world;


};
cbuffer MatrixBuffer : register(b11)
{
    matrix _view;
    matrix _projection;

};


struct VertexInput
{
    float4 position : POSITION0;
    float3 uvq : TEXCOORD0;

};

struct PixelInput
{
    float4 position : SV_POSITION;
    float3 uvq : TEXCOORD0;

    float4 projPosition : TEXCOORD1;

};

PixelInput VS(VertexInput input)
{
    PixelInput output;
    input.position.w = 1;
    output.position = mul(input.position, _world);
    output.position = mul(output.position, _view);
    output.position = mul(output.position, _projection);

    output.projPosition = output.position;
    output.uvq = input.uvq;


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
    uv *= scale*2;
    uv *= 3;
    float2 uv_int = ceil(uv);
    uv = frac(uv);
    
    uv -= 0.5;
    uv.x *= 50 / 4;
    uv.x += sin(uv_int.y * uv_int.x* 4 + uv_int.x * 6) * scale;

    float circle = lerp(0, 1, (0.3 - (uv.x * uv.x + uv.y * uv.y)));
    circle = saturate(circle);

    return circle * 0.5;
}

float4 PS(PixelInput input) : SV_Target0
{
    float2 uv = input.uvq.xy;
    uv.x/= input.uvq.z; // 다시 원래 텍스쳐 uv로 되돌리기 위해 나눠준다

    float2 uv_depthmap;
    uv_depthmap = input.projPosition.xy / input.projPosition.w / 2.0f + 0.5f;
    uv_depthmap.y *= -1;

    float mapdist = 1;
    mapdist =  _map.Sample(samp[0], uv_depthmap).r;
    //return mapdist;

    uv.x += time.r/10;
    float c = 0;
   // c += rain(uv, 5.);
   // mapdist += sin(time)/10;
   mapdist *= 10;
    

    for (float i = 1; i <mapdist; i++)
    {
        c += rain(uv, i * 2 + 1);
    }

    return c;
}
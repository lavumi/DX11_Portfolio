cbuffer MatrixBuffer : register(b13)
{
    matrix _world;
};
cbuffer MatrixBuffer : register(b12)
{
    matrix _viewXprojection;
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
    output.position = mul(output.position, _viewXprojection);

    output.uv = input.uv;


    return output;
}

cbuffer SkyBuffer : register(b0)
{
    float _translation;
    float _scale;
};


Texture2D _perlin : register(t0);

SamplerState samp[3];


float4 PS(PixelInput input) : SV_Target0
{

    float4 perturbValue;
    float4 cloudColor;

    float2 uv = input.uv*2;

   uv.x = uv.x + _translation;

    float2 moveUV = uv;
    moveUV -= 0.5f;
    moveUV = abs(moveUV);
    perturbValue = _perlin.Sample(samp[0], moveUV);
   perturbValue *= _scale;
   uv += perturbValue.xy + _translation;


    //cloudColor = _map.Sample(samp[0], uv);
    cloudColor = _perlin.Sample(samp[0], uv);




    cloudColor = lerp(cloudColor, 0, (cloudColor - 0.3f) / 0.1f);


    cloudColor = saturate(cloudColor);


    return cloudColor;
}
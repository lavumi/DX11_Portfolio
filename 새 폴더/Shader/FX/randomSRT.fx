Texture2D _map : register(t0);

SamplerState samp;

cbuffer rndMatrix : register(b0)
{
    matrix _scale;
    matrix _rotation;
    matrix _translation;

}
cbuffer rndMatrix : register(b0)
{
    float4 _color1;
    float4 _color2;
    float _minScale;
    float _maxScale;
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
    float RNDNNUM : TEXCOORD1;

};


float random(float2 p)
{
    float2 K1 = float2(52.49024323412, 11.231512035);

    float result = frac(cos(dot(p, K1)) * 41214.3215);


    result = lerp(result + 0.2, result - 0.2, step(result, 0.5));

    return result;
}

PixelInput VS(VertexInput input)
{
    PixelInput output;

    
   output.uv = input.uv;
   // input.position.y *= ref;
   output.position = mul(input.position, _scale);
   output.position = mul(output.position, _rotation);
    
   output.position += float4(_translation._14, _translation._24, _translation._34, 0);

 // if(output.position.x >= 1)        output.position.x = 1 - output.position;


   output.RNDNNUM = _scale._11;
    
   return output;
}


float4 PS(PixelInput input) : SV_Target
{
    float4 grass = _map.Sample(samp, input.uv);
    
    float4 colorg = grass.g * _color1;//float4(grass.g * 0.20784, grass.g * 0.48431, grass.g * 0.26666, 1);
    float4 colory = grass.g * _color2; //float4(grass.g * 0.88, grass.g * 0.87, grass.g * 0.6, 1);
    float4 result = lerp(colory, colorg, (input.RNDNNUM - _minScale + (_maxScale - _minScale)/3) / (_maxScale - _minScale));


    return result; 
    
}
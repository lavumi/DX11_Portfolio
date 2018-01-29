cbuffer rndSeed : register(b0)
{
    float4 seed;
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

    output = input;
    
    return output;
}
float CosineLerp(float x, float y, float fractional)
{
    float ft = 3.141592f * fractional;
    float f = (1.0f - cos(ft)) * 0.5f;

    return x * (1.0f - f) + y * f;
}


//망함. 쓰지 말자
float Noise(float2 uv)
{
   float result = frac(cos(dot(uv, float2(132.472583f, 141.1326f))) * 41624.31315);
   
    result = lerp(result, result - 0.8f, step(result , 0.5f));
   
    return result;
}




float SmoothNoise(float x, float y)
{
    float corners =     (
    Noise(float2(x - 1, y - 1)) + 
    Noise(float2(x + 1, y + 1)) + 
    Noise(float2(x + 1, y - 1)) + 
    Noise(float2(x - 1, y + 1))
    ) / 16.0f;

    float sides =  (
    Noise(float2(x, y - 1)) + 
    Noise(float2(x, y + 1)) + 
    Noise(float2(x + 1, y)) + 
    Noise(float2(x - 1, y))
    ) / 8.0f;

    float center = Noise(float2(x, y)) / 4.0f;

    return corners + sides + center;
}

float LerpedNoise(float x, float y)
{

    float int_x = floor(x), frac_x = frac(x);
    float int_y = floor(y), frac_y = frac(y);

    float p1 = SmoothNoise(int_x,       int_y);
    float p2 = SmoothNoise(int_x + 1,   int_y);
    float p3 = SmoothNoise(int_x,       int_y + 1);
    float p4 = SmoothNoise(int_x + 1,   int_y + 1);

    p1 = CosineLerp(p1, p2, frac_x);
    p2 = CosineLerp(p3, p4, frac_x);

    return CosineLerp(p1, p2, frac_y);
}

float CreatePerlinNoise(float x, float y)
{
    float result =0.0f, amplitude =0.5f, frequency = 1.0f, persistance = 0.5f;
    frequency *= 2;
    for (int i = 0; i <8; i++)
    {
        result += LerpedNoise(x * frequency + frequency, y * frequency - frequency) * amplitude;
        frequency *= 2;
        amplitude *= persistance;
     }

    return result;
}


float4 PS(PixelInput input) : SV_Target
{
    float index = CreatePerlinNoise(input.uv.x +seed.x  , input.uv.y + seed.y );
    return float4(index, index, index, 1);
    float4(index * 0.172f, index * 0.117f, index*0.09f, 1);
}


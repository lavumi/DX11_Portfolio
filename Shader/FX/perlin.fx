cbuffer rndSeed : register(b0)
{
    float4 color;
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



float Noise(float2 uv)
{
   float result = frac(cos(dot(uv, float2(132.472583f, 141.1326f))) * 41624.31315);
   // float result = cos(dot(uv, float2(3, 5)));

   
    return result;
}



//망함. 쓰지 말자
//float SmoothNoise(float x, float y)
//{
//    float corners =     (
//    Noise(float2(x - 1, y - 1)) + 
//    Noise(float2(x + 1, y + 1)) + 
//    Noise(float2(x + 1, y - 1)) + 
//    Noise(float2(x - 1, y + 1))
//    ) / 16.0f;

//    float sides =  (
//    Noise(float2(x, y - 1)) + 
//    Noise(float2(x, y + 1)) + 
//    Noise(float2(x + 1, y)) + 
//    Noise(float2(x - 1, y))
//    ) / 8.0f;

//    float center = Noise(float2(x, y)) / 4.0f;

//    return corners + sides + center;
//}

float LerpedNoise(float x, float y, float seed,float freq)
{

   float int_x = floor(x), frac_x = frac(x);
   float int_y = floor(y), frac_y = frac(y);

    float maxX = (2 + seed) * freq - 1;
    float maxY = (2 - seed) * freq - 1;

    float p1, p2, p3, p4;

    p1 = p2 = p3 = p4 = 0;
    float next_x, next_y;



    next_x = lerp(int_x + 1, freq * (seed + 1), step(maxX, int_x));
    next_y = lerp(int_y + 1, freq * (1 - seed), step(maxY, int_y));

    p1 = Noise(float2(int_x, int_y)); //SmoothNoise(int_x,       int_y);
    p2 = Noise(float2(next_x, int_y)); //SmoothNoise(int_x + 1,   int_y);
    p3 = Noise(float2(int_x, next_y)); //SmoothNoise(int_x,       int_y + 1);
    p4 = Noise(float2(next_x, next_y)); //SmoothNoise(int_x + 1,   int_y + 1);


    p1 = CosineLerp(p1, p2, frac_x);
    p3 = CosineLerp(p3, p4, frac_x);

   return CosineLerp(p1, p3, frac_y);
}

float CreatePerlinNoise(float x, float y, float seed)
{
    x += seed;
    y -= seed;
    float result =0.0f, amplitude =0.5f, frequency = 1.0f, persistance = 0.5f;
    frequency *= 2;

    for (int i = 0; i <8; i++)
    {
        result += LerpedNoise((x +  1) * frequency , (y  + 1) * frequency , seed, frequency) * amplitude;
        frequency *= 2;
        amplitude *= persistance;
     }
    result -= 0.1f;
    return result;
}


float4 PS(PixelInput input) : SV_Target
{
 

    float2 uv = input.uv;
    float rndSeed =  ceil(color.a);
    float index = CreatePerlinNoise(uv.x, uv.y , rndSeed);

    // index = LerpedNoise(input.uv.x, input.uv.y);
    float4 result =  float4(index * color.r, index* color.g, index * color.b, 1);

    return result;

    float4(index * 0.172f, index * 0.117f, index*0.09f, 1);
   

}


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



float Noise(float3 uv)
{
   float result = frac(cos(dot(uv, float3(132.472583f, 141.1326f, 421.6938f))) * 41624.31315);
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

float LerpedNoise(float x, float y, float z, float seed,float freq)
{

   float int_x = floor(x), frac_x = frac(x);
   float int_y = floor(y), frac_y = frac(y);
   float int_z = floor(z), frac_z = frac(z);

    float maxX = (2 + seed) * freq - 1;
    float maxY = (2 - seed) * freq - 1;
    float maxZ = (2 - seed) * freq - 1;

    float p1, p2, p3, p4, p5, p6, p7, p8;

    p1 = p2 = p3 = p4 = p5 = p6 = p7 = p8 = 0;
    float next_x, next_y, next_z;



    next_x = int_x + 1;
    next_y = int_y + 1;
    next_z = int_z + 1;

    p1 = Noise(float3(int_x, int_y, int_z)); //SmoothNoise(int_x,       int_y);
    p2 = Noise(float3(next_x, int_y, int_z)); //SmoothNoise(int_x + 1,   int_y);
    p3 = Noise(float3(int_x, next_y, int_z)); //SmoothNoise(int_x,       int_y + 1);
    p4 = Noise(float3(next_x, next_y, int_z)); //SmoothNoise(int_x + 1,   int_y + 1);

    p5 = Noise(float3(int_x, int_y, next_z)); //SmoothNoise(int_x,       int_y);
    p6 = Noise(float3(next_x, int_y, next_z)); //SmoothNoise(int_x + 1,   int_y);
    p7 = Noise(float3(int_x, next_y, next_z)); //SmoothNoise(int_x,       int_y + 1);
    p8 = Noise(float3(next_x, next_y, next_z)); //SmoothNoise(int_x + 1,   int_y + 1);

    p1 = CosineLerp(p1, p2, frac_x);
    p3 = CosineLerp(p3, p4, frac_x);

    p5 = CosineLerp(p5, p6, frac_x);
    p7 = CosineLerp(p7, p8, frac_x);



    p1 = CosineLerp(p1, p3, frac_y);
    p5 = CosineLerp(p5, p7, frac_y);

    return CosineLerp(p1, p5, frac_z);
}

float CreatePerlinNoise(float x, float y, float z, float seed)
{
    x += seed;
    y -= seed;
    float result =0.0f, amplitude =0.5f, frequency = 1.0f, persistance = 0.5f;
    frequency *= 2;

    for (int i = 0; i <8; i++)
    {
        result += LerpedNoise((x + 1) * frequency, (y + 1) * frequency, (z + 1) * frequency,     seed, frequency) * amplitude;
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

    float x, y, z;

    x = frac(input.uv.x * 4);
    y = frac(input.uv.y * 4);

    z = (floor(input.uv.x * 4) + floor(input.uv.y * 4)*4) / 16;
   // z = 1;

    float index = CreatePerlinNoise(x, y ,z, rndSeed);

    // index = LerpedNoise(input.uv.x, input.uv.y);
    float4 result =  float4(index * color.r, index* color.g, index * color.b, 1);

    return result;

    float4(index * 0.172f, index * 0.117f, index*0.09f, 1);
   

}


cbuffer PatternDatas : register(b0)
{
    float4 _color1;
    float4 _color2;
    float LineWidth;
    float _LineCount;

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

    output = input;


    return output;
}


float MultiCircle(float2 uv, float size)
{
    uv *= size;
    uv = frac(uv);
    uv -= 0.5f;

    float result = lerp(1, 0, step(uv.x * uv.x + uv.y * uv.y, 0.25));
    result = lerp(result, 1, step(uv.x * uv.x + uv.y * uv.y, 0.16));
    result = lerp(result, 0, step(uv.x * uv.x + uv.y * uv.y, 0.09));
    result = lerp(result, 1, step(uv.x * uv.x + uv.y * uv.y, 0.04));
    result = lerp(result, 0, step(uv.x * uv.x + uv.y * uv.y, 0.01));

    return result;
}

float MultiTriangle(float2 uv, float size)
{

    uv.y *= size;

    float lineCount = floor(uv.y)/size;

    uv.x += lineCount  - 1/size;
    uv.x *= size/2;
    uv = frac(uv);



    float first = lerp(0, 1, step(uv.x*2- uv.y, 1));
    float second = lerp(1, 0, step(uv.x * 2 + uv.y, 1));

    float result = min(first, second);

    return result;
}

float Multi3DTriangle(float2 uv, float size)
{

    uv.y *= size;

    float lineCount = floor(uv.y) / size;

    uv.x += lineCount;
    uv.x *= size / 2;
    uv = frac(uv);



    float first = lerp(0.5, 1, step(uv.x * 2 - uv.y, 1));
    float second = lerp(1, 0, step(uv.x * 2 + uv.y, 1));

    float result = min(first, second);

    return result;
}

float MultiCube(float2 uv, float size)
{

    uv.y *= size*3;

    float lineCount = floor(uv.y) ;

    uv.x += lineCount / size;
    uv.x *= size / 2*3;
    uv = frac(uv);
    float first;
    float second;
    float result;
    if (frac(lineCount / 3) > 0.6)
    {
        result = lerp(0, 0.5, step(frac(uv.x ), 0.5));  
    }
    else if (frac(lineCount / 3) > 0.3)
    {
        second = lerp(1, 0.5, step(uv.x * 2 - uv.y, 1));
        first =  lerp(0, 1, step(uv.x * 2 + uv.y, 1));

       result = lerp(second, first, step(frac(uv.x), 0.5));

    }
    else
    {
        second = lerp(0.5, 1, step(uv.x * 2 - uv.y, 1));
        first = lerp(1, 0, step(uv.x * 2 + uv.y, 1));
        result = min(first, second);
       
    }

    
    return result;
}

float X(float2 uv, float size)
{

    uv *= size;
    uv = frac(uv);
   // uv -= 0.5f;



    float first = lerp(1, 0, step(abs(uv.x - uv.y), 0.1));
    float second = lerp(1, 0, step(abs(uv.x  + uv.y-1), 0.1));

    float result = min(first, second);

    return result;
}

float Stripe(float2 uv, float size)
{

    uv *= size;
    uv = frac(uv);
   // uv -= 0.5f;



    float result = lerp(1, 0, step(abs( uv.y), 0.5));


    return result;
}

float Gradation(float2 uv, float size)
{

    uv *= size;
    uv = frac(uv);
   // uv -= 0.5f;



    float result = lerp(1, 0, uv.y);


    return result;
}


float Square3D(float2 uv, float size)
{

    uv.y *=size;

    float lineCount = floor(uv.y) / size;

    uv.x += lineCount  - 1/size;
    uv.x *= size/2;
    uv = frac(uv);


    float sub1 = lerp(0, 1, step((uv.x * 2 - uv.y), 1.5));
    float sub2 = lerp(0, 1, step((uv.x * 2 - uv.y), 0.5));


    float first = lerp(sub1, sub2, step((uv.x * 2 - uv.y), 1));

    float sub3 = lerp(0, 1, step((uv.x * 2 + uv.y), 1.5));
    float sub4 = lerp(0, 1, step((uv.x * 2 + uv.y), 0.5));

    float second = lerp(sub3, sub4, step((uv.x * 2 + uv.y), 1));
    
    float result = lerp(first,second,  step(frac(uv.x), 0.5));

    return result;
}

float Gray(float2 uv, float size)
{
    uv *= size;
    uv = frac(uv);

    float result1 = lerp(1, 0, step(cos(uv.x*3.141592*2)*0.9+0.1, uv.y * 2 - 1));
    float result2 = lerp(0,1, step(cos(uv.x * 3.141592 * 2) * 0.9 - 0.1, uv.y * 2 - 1));

    return min(result1, result2);
}




float4 PS(PixelInput input) : SV_Target
{
    float2 uv = input.uv;

  uv.x = (input.uv.x + input.uv.y);
  uv.y = (input.uv.y - input.uv.x);


    float lineIndex = _LineCount;
    _LineCount * 2;

    float lineCount = uv.y * lineIndex;
    float result = 0;//

    


    if (lineCount >= lineIndex-4 )
    {
        result = X(uv, LineWidth);
    }
    else if (lineCount >= lineIndex-6)
    {
        result = MultiCircle(uv, LineWidth);
    }
    else if (lineCount >= lineIndex-8)
    {
        result = MultiCube(uv, LineWidth);
    }
    else if (lineCount >= lineIndex-10)
    {
        result = MultiTriangle(uv, LineWidth);
    }
    else if (lineCount >= lineIndex-12)
    {
        result = Stripe(uv, LineWidth);
    }
    else if (lineCount >= lineIndex-14)
    {
        result = Gradation(uv, LineWidth);
    }
    else if (lineCount >= lineIndex-16)
    {
        result = MultiCircle(uv, LineWidth);
    }
    else if (lineCount >= lineIndex-18)
    {
        result = Gray(uv, LineWidth);
    }
    else if (lineCount >= lineIndex-20)
    {
        result = MultiTriangle(uv, LineWidth);
    }
    else if (lineCount >= lineIndex-22)
    {
        result = X(uv, LineWidth);
    }
           
    
    return lerp(_color1, _color2, result);
}
Texture2D _map : register(t0);

SamplerState samp;


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
    float3 instancePosition : INSTVECTOR0;
    float scale : INSTVECTOR1;
    float rotation : INSTVECTOR2;

};

struct PixelInput
{
    float4 position : SV_POSITION;
    float2 uv : TEXCOORD0;
    float RNDNNUM : TEXCOORD4;

};




PixelInput VS(VertexInput input)
{
    PixelInput output;
    
   output.uv = input.uv;
   output.position = input.position;


   matrix scaleMatrix = 0;
   scaleMatrix._11 = input.scale;
   scaleMatrix._22 = input.scale;
   scaleMatrix._33 = input.scale;
   scaleMatrix._44 =1;

   matrix rotationMatrix = 0;
   rotationMatrix._11 = cos(input.rotation);
   rotationMatrix._22 = cos(input.rotation);
   rotationMatrix._21 = sin(input.rotation);
   rotationMatrix._12 =-1* sin(input.rotation);
   rotationMatrix._33 = 1;
   rotationMatrix._44 = 1;

  
   output.position = mul(input.position, scaleMatrix);
   output.position = mul(output.position, rotationMatrix);
   
   
   output.position += float4(input.instancePosition, 0);
  
   output.RNDNNUM = input.scale;
    

   return output;
}


float4 PS(PixelInput input) : SV_Target
{
    float4 grass = _map.Sample(samp, input.uv);
    
    float4 colorg = grass.g * _color1;//float4(grass.g * 0.20784, grass.g * 0.48431, grass.g * 0.26666, 1);
    float4 colory = grass.g * _color2; //float4(grass.g * 0.88, grass.g * 0.87, grass.g * 0.6, 1);
    float4 result = lerp(colory, colorg, (input.RNDNNUM - _minScale + (_maxScale - _minScale)/3) / (_maxScale - _minScale));
    
    //result.a *= 10;
    return result;
    
}
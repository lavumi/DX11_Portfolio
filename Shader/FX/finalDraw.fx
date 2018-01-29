Texture2D _map : register(t0);

SamplerState samp;


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

   float scaleFactor = 1;
   
  output.position.w = scaleFactor;
    output.position.x /= 2;
 //output.position.x -= scaleFactor/1.2;
 //output.position.y += scaleFactor/1.5;
 


    return output;
}


float4 PS(PixelInput input) : SV_Target
{
    float4 result = _map.Sample(samp, input.uv);
    return result;

}
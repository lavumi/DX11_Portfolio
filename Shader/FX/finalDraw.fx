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

   float scaleFactor = 1.3;
   
  output.position.w = scaleFactor;
    output.position.x /= 2;
 //output.position.x -= scaleFactor/1.2;
 //output.position.y += scaleFactor/1.5;
 


    return output;
}


float4 PS(PixelInput input) : SV_Target
{
    float4 result = _map.Sample(samp, input.uv);
    

    //wood
    //result = result * 5;
    //result -= floor(result);

    //cloud
    //result = lerp(result, 0, (result - 0.3)/0.05);



    //Map
  //float4 color = 0;
  //if(result.g < 0.3f)
  //    color = float4(0, 0.5, 1, 1);
  //else if (result.g < 0.4f)
  //    color = float4(51.0, 255.0, 153.0, 1) / 255.0;
  //else if (result.g < 0.5f)
  //    color = float4(0, 151.0, 0, 1) / 255.0;
  //else if (result.g < 0.6f)
  //    color = float4(102.0, 51.0, 0, 1) / 255.0;
  //else if (result.g < 0.7f)
  //    color = float4(51.0, 25, 0, 1)/255.0;
  //else 
  //    color = float4(0, 1, 1, 1);

    
    return result;

}
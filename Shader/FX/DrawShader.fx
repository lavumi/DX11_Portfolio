cbuffer textureData : register(b0)
{
    float color[128 * 128];

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


float4 PS(PixelInput input) : SV_Target
{
    float result = color[(int) (input.uv.x * 128) + (int) (input.uv.y * 128 * 128)];
    return result;

}
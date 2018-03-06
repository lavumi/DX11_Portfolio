
cbuffer MatrixBuffer : register(b13)
{
    matrix _world;
};
cbuffer MatrixBuffer : register(b12)
{
    matrix _viewXprojection;
};


cbuffer LightView : register(b0)
{
    matrix _lightView;
    matrix _lightProjection;
};



struct VertexInput
{
    float4 position : POSITION0;

};

struct PixelInput
{
    float4 position : SV_POSITION;
    float4 worldPosotion : TEXCOORD0;

};

PixelInput VS(VertexInput input)
{
    PixelInput output;
    input.position.w = 1;
    output.position = mul(input.position, _world);
  //output.position = mul(output.position, _view);
  //output.position = mul(output.position, _projection);

    output.position = mul(output.position, _lightView);
    output.position = mul(output.position, _lightProjection);
    output.worldPosotion = output.position;


    return output;
}


float4 PS(PixelInput input) : SV_Target
{
    float depthValue;

    depthValue = input.worldPosotion.z / input.worldPosotion.w;

    return depthValue;

}
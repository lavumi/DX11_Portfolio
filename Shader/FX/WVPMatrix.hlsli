
cbuffer MatrixBuffer : register(b13)
{
    matrix _world;
};
cbuffer MatrixBuffer : register(b12)
{
    matrix _viewXprojection;
};

cbuffer LightData : register(b11)
{
    matrix _lightView;
    matrix _lightProjection;
    float3 _lightDir;
    float padding;
}
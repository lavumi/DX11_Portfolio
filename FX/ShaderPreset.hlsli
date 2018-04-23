
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

cbuffer CascadeShadow : register(b10)
{
    matrix cropMatrix[4];
}

Texture2D _perlin : register(t14);


float3x3 RotationMatrix(float3 axis, float theta)
{
    float t = theta;
    float x = axis.x;
    float y = axis.y;
    float z = axis.z;

    return float3x3(
                        cos(t) - x * x * (cos(t) - 1), x * y * (1 - cos(t)) - z * sin(t), x * z * (1 - cos(t)) + y * sin(t),
                        y * x * (1 - cos(t)) + z * sin(t), cos(t) + y * y * (1 - cos(t)), y * z * (1 - cos(t)) - x * sin(t),
                        z * x * (1 - cos(t)) - y * sin(t), z * y * (1 - cos(t)) + x * sin(t), cos(t) + z * z * (1 - cos(t))
    );

}

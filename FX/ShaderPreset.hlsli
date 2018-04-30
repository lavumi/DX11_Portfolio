
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
static float rndTable[100] =
{
    0.11f, 0.38f, 0.88f, 0.77f, 0.09f, 0.95f, 0.80f, 0.48f, 0.72f, 0.14f,
    0.96f, 0.74f, 0.15f, 0.93f, 0.71f, 0.14f, 0.36f, 0.21f, 0.51f, 0.46f,
    0.75f, 0.07f, 0.23f, 0.20f, 0.73f, 0.98f, 0.80f, 0.07f, 0.41f, 0.32f,
    0.83f, 0.35f, 0.68f, 0.68f, 0.24f, 0.31f, 0.93f, 0.12f, 0.94f, 0.46f,
    0.35f, 0.70f, 0.92f, 0.95f, 0.93f, 0.96f, 0.65f, 0.47f, 0.25f, 0.95f,
    0.25f, 0.43f, 0.89f, 0.74f, 0.81f, 0.20f, 0.17f, 0.97f, 0.03f, 0.23f,
    0.35f, 0.09f, 0.52f, 0.90f, 0.42f, 0.57f, 0.58f, 0.55f, 0.29f, 0.10f,
    0.17f, 0.10f, 0.84f, 0.20f, 0.96f, 0.31f, 0.77f, 0.40f, 0.72f, 0.02f,
    0.55f, 0.30f, 0.18f, 0.58f, 0.46f, 0.26f, 0.86f, 0.61f, 0.34f, 0.40f,
    0.05f, 0.91f, 0.63f, 0.27f, 0.12f, 0.11f, 0.29f, 0.65f, 0.15f, 0.20f,
};


static const float splitedFrustum[3] = { 0.996013939f, 0.999374747f, 1 };
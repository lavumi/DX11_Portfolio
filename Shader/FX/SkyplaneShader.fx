cbuffer CameraBuffer : register(b0)
{
    matrix _view;
    matrix _projection;
    float3 _cameraPosition;
    float _paddd;
};



cbuffer WorldBuffer : register(b1)
{
    matrix _world;
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
    input.position.w = 1;
    output.position = mul(input.position, _world);
    output.position = mul(output.position, _view);
    output.position = mul(output.position, _projection);

    output.uv = input.uv;


    return output;
}

cbuffer SkyBuffer : register(b0)
{
    float _translation;
    float _scale;
    float _brightness;
    float _padding;
};

Texture2D _map : register(t0);
Texture2D _perlin : register(t1);

SamplerState samp[3];


float4 PS(PixelInput input) : SV_Target
{

    float4 perturbValue;
    float4 cloudColor;

    float2 uv = input.uv;
	// Translate the texture coordinate sampling location by the translation value.
    uv.x = uv.x + _translation;

	// Sample the texture value from the perturb texture using the translated texture coordinates.
    perturbValue = _perlin.Sample(samp[0], uv);

	// Multiply the perturb value by the perturb scale.
    perturbValue = perturbValue * _scale;

	// Add the texture coordinates as well as the translation value to get the perturbed texture coordinate sampling location.
    perturbValue.xy = perturbValue.xy +uv + _translation;

	// Now sample the color from the cloud texture using the perturbed sampling coordinates.
    cloudColor = _map.Sample(samp[0], perturbValue.xy);

	// Reduce the color cloud by the brightness value.
    cloudColor = cloudColor * _brightness;


   

    return cloudColor;

}
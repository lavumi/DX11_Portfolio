Texture2D _diffuseMap : register(t0);
Texture2D _normalMap : register(t1);
Texture2D _specularMap : register(t2);
SamplerState _samp;

cbuffer CameraBuffer : register(b0)
{
    matrix _view;
    matrix _projection;
	float3 _cameraPosition;
	float _cameraPadding;
};

cbuffer WorldBuffer : register(b1)
{
    matrix _world;
}

cbuffer BoneBuffer : register(b2)
{
    float4x4 _boneScale;
    float4x4 _boneArray[100];
    uint _skinning;
    float3 _bonePadding;
}

cbuffer SunBuffer : register(b0)
{
	float4 _lightDiffsueColor;
	float4 _lightSpecularColor;
	float _lightSpecularFactor;
    float3 _lightDir;
};

struct VertexInput
{
    float4 position : POSITION0;
    float3 normal : NORMAL0;
	float3 tangent : TANGENT0;
    float2 uv : TEXCOORD0;
    float4 boneIndices : BLENDINDICES;
    float4 boneWeights : BLENDWEIGHT;
};

struct PixelInput
{
    float4 position : SV_POSITION;
    float3 normal : NORMAL0;
	float3 tangent : TANGENT0;
    float2 uv : TEXCOORD0;
	float3 viewDirection : TEXCOORD1;
};

float3 NormalSampleToWoldSpace(float3 normalMapSample, float3 inputNormal, float3 inputTangent)
{
	// [0,1] -> [-1, 1]
	float3 normalT = 2.0f * normalMapSample - 1.0f;

	// 탄젠트공간의 정규 직교 기저 구하기
	float3 N = inputNormal;
	float3 T = normalize(inputTangent - dot(inputTangent, N)*N);
	float3 B = cross(N, T);

	float3x3 TBN = float3x3(T, B, N);

	float3 outputNormal = mul(normalT, TBN);

	return outputNormal;
}

PixelInput VS(VertexInput input)
{
    PixelInput output;
	float4 worldPosition;

    if (_skinning == 0)
    {
        output.position = input.position;
        output.normal = input.normal;
    }
    else
    {
        float4x4 skinTransform = 0;
        skinTransform += mul(input.boneWeights.x, _boneArray[(uint)input.boneIndices.x]);
		skinTransform += mul(input.boneWeights.y, _boneArray[(uint)input.boneIndices.y]);
		skinTransform += mul(input.boneWeights.z, _boneArray[(uint)input.boneIndices.z]);
		skinTransform += mul(input.boneWeights.w, _boneArray[(uint)input.boneIndices.w]);

        output.position = mul(input.position, skinTransform);
        output.normal = normalize(mul(input.normal, (float3x3)skinTransform));
		output.tangent = normalize(mul(input.tangent, (float3x3)skinTransform));
    }
	
	worldPosition = mul(output.position, _world);

    output.position = mul(worldPosition, _view);
    output.position = mul(output.position, _projection);

	output.normal = mul(output.normal, (float3x3) _world);
	output.normal = normalize(output.normal);

	output.tangent = mul(output.normal, (float3x3) _world);
	output.tangent = normalize(output.normal);

	output.viewDirection = _cameraPosition.xyz -worldPosition.xyz;
	output.viewDirection = normalize(output.viewDirection);

    output.uv = input.uv;

    return output;
}

float4 PS(PixelInput input) : SV_TARGET
{
	float4 textureColor = _diffuseMap.Sample(_samp, input.uv);

	float3 normalMapSample = _normalMap.Sample(_samp, input.uv).rgb;
	float3 bumpedNormal = NormalSampleToWoldSpace(normalMapSample, input.normal, input.tangent);

    float3 lightDir = _lightDir * -1;
    float lightIntensity = saturate(dot(bumpedNormal, lightDir));
    float4 color = textureColor * lightIntensity;
	
	if (lightIntensity > 0.0f)
	{
		float4 specularIntensity = _specularMap.Sample(_samp, input.uv);

		float3 reflection = normalize(2 * lightIntensity * bumpedNormal - lightDir);
		float4 specular = pow(saturate(dot(reflection, input.viewDirection)), _lightSpecularFactor);
		specular = specular * specularIntensity;

		color = saturate(color + specular);
	}

    return color;
}

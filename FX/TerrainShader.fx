#include "ShaderPreset.hlsli"

struct VertexIn
{
    float4 position : POSITION0;
    float2 uv : TEXCOORD0;
    float3 normal : NORMAL0;
    float3 tangent : NORMAL1;
};

struct VertexOut
{
    float4 position : POSITION0;
    float2 uv : TEXCOORD0;
    float3 normal : NORMAL0;
    float3 tangent : NORMAL1;
};

VertexOut VS(VertexIn vin)
{
    VertexOut vout;
    
    vout.position = vin.position;
    vout.position.w = 1;

    vout.uv = vin.uv;
    vout.normal = vin.normal;
    vout.position = vin.position;

    return vout;
}



struct PatchTess
{
    float EdgeTess[4] : SV_TessFactor;
    float InsideTess[2] : SV_InsideTessFactor;
};

PatchTess ConstantHS(InputPatch<VertexOut, 4> patch, uint patchID : SV_PrimitiveID)
{
    PatchTess pt;

    pt.EdgeTess[0] = 64.0f;
    pt.EdgeTess[1] = 64.0f;
    pt.EdgeTess[2] = 64.0f;
    pt.EdgeTess[3] = 64.0f;

    pt.InsideTess[0] = 64.0f;
    pt.InsideTess[1] = 64.0f;

    return pt;
}

struct HullOut
{
    float4 position : POSITION0;
    float2 uv : TEXCOORD0;
    float3 normal : NORMAL0;
    float3 tangent : NORMAL1;
    
    
};

[domain("quad")]
[partitioning("integer")]
[outputtopology("triangle_cw")]
[outputcontrolpoints(4)]
[patchconstantfunc("ConstantHS")]
[maxtessfactor(64.0f)]
HullOut HS(InputPatch<VertexOut, 4> p,
	uint i : SV_OutputControlPointID,
	uint patchId : SV_PrimitiveID)
{
    HullOut hout;

    hout.position = p[i].position;
    hout.uv         = p[i].uv;
    hout.normal = p[i].normal;
    hout.position = p[i].position;

    return hout;
}




cbuffer VSBuffer : register(b0)
{
    float4 clipPlane;
    matrix worldInverseTransposeMatrix;
};

struct DomainOut
{
    float4 position : SV_POSITION;
    float2 uv : TEXCOORD0;
    float worldPosition : TEXCOORD1;
    float3 lightDir : TEXCOORD2;
    float3 normal : NORMAL0;
    float3 worldNormal : NORMAL1;
    
    float3 viewPosition : TEXCOORD5;
   
    float4 clip : SV_ClipDistance0;
};

Texture2D _map : register(t0);
SamplerState samp[3];

[domain("quad")]
DomainOut DS(PatchTess patchTess,
	float2 uv : SV_DomainLocation,
	const OutputPatch<HullOut, 4> quad)
{
    DomainOut dout;

    HullOut outData;
	// Bilinear interpolation.
    float4 p1 = lerp(quad[0].position, quad[1].position, uv.x);
    float4 p2 = lerp(quad[2].position, quad[3].position, uv.x);
    outData.position = lerp(p1, p2, uv.y);


    float2 uv1 = lerp(quad[0].uv, quad[1].uv, uv.x);
    float2 uv2 = lerp(quad[2].uv, quad[3].uv, uv.x);
    outData.uv = lerp(uv1, uv2, uv.y);

    float3 n1 = lerp(quad[0].normal, quad[1].normal, uv.x);
    float3 n2 = lerp(quad[2].normal, quad[3].normal, uv.x);
    outData.normal = lerp(n1, n2, uv.y);

    float3 t1 = lerp(quad[0].tangent, quad[1].tangent, uv.x);
    float3 t2 = lerp(quad[2].tangent, quad[3].tangent, uv.x);
    outData.tangent = lerp(t1, t2, uv.y);




    float2 hightuv = outData.position.xz / 256;
    outData.position.y = _map.SampleLevel(samp[0], hightuv, 0).r * 20.0f;

    dout.position = mul(outData.position, _world);
    dout.worldPosition = dout.position.y;
    dout.position = mul(dout.position, _viewXprojection);

    dout.viewPosition = dout.position.xyw;

    dout.uv = outData.uv;

    float3 n = mul(outData.normal, (float3x3) worldInverseTransposeMatrix);
    float3 t = mul(outData.tangent, (float3x3) worldInverseTransposeMatrix);
    float3 b = cross(n, t);

    float3x3 tbnMatrix = float3x3(t.x, b.x, n.x,
	                            t.y, b.y, n.y,
	                            t.z, b.z, n.z);


    dout.lightDir = normalize(mul(-_lightDir, tbnMatrix));



    dout.uv = outData.uv;
    dout.normal = normalize(mul(outData.normal, tbnMatrix));
    dout.worldNormal = abs(outData.normal);


    dout.clip = dot(mul(outData.position, _world), clipPlane);
   // output.clip = clipPlane;
    return dout;


}




Texture2D _texture[3] : register(t0);

Texture2D _lightMap : register(t13);
Texture2D _normalMap : register(t5);
cbuffer PSBuffer : register(b0)
{
    float4 ambient;
    float4 diffuse;
    float4 globalAmbient;

};

struct PixelOut
{
    float4 albedo : SV_TARGET0;
    float4 depthMap : SV_TARGET1;
    float4 viewSpaceVector : SV_TARGET2;
};

PixelOut PS(DomainOut input) : SV_Target
{
    PixelOut output;



    float2 uv = input.uv;

    float3 normal = _normalMap.Sample(samp[0], uv).rgb;
    normal = (normal * 2.0f) - 1.0f;
    normal = normalize(normal);

    float3 light = normalize(input.lightDir);
    float nDotL = saturate(dot(normal, light));
    float4 intensity = ambient * globalAmbient + diffuse * nDotL;



    //그림자
    float2 projectTexCoord;
    projectTexCoord.x = input.viewPosition.x / input.viewPosition.z / 2.0f + 0.5f;
    projectTexCoord.y = -input.viewPosition.y / input.viewPosition.z / 2.0f + 0.5f;
    
    float shadowValue = _lightMap.Sample(samp[1], projectTexCoord).g * 0.3f;
    intensity *= 1; //    shadowValue;





    //텍스쳐링
    float3 blending = input.worldNormal;
    blending.y -= (blending.x + blending.z) / 2;
    blending = normalize(max(blending, 0.0001f));
   
    float4 Mountain = _texture[2].Sample(samp[0], uv);
    float4 Grass = _texture[0].Sample(samp[0], uv);
   
    float4 landNmountain = Mountain * blending.x + Grass * blending.y + Mountain * blending.z;



    float blendFactor = saturate((input.worldPosition + 7.7f) / 0.4f);

    float4 diffuseMap = lerp(_texture[1].Sample(samp[0], uv), landNmountain, blendFactor);

    diffuseMap.a = input.clip;




 

    output.albedo = intensity * diffuseMap;

    float3 depthValue = input.position.w / 300;
    output.depthMap = float4(depthValue, 1);


   //output.viewSpaceVector.rgb = mul(input.worldNormal, (float3x3) cameraSpace);
   //
   //output.viewSpaceVector.a = 1;
    return output;
}
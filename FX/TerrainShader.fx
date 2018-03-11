#include "ShaderPreset.hlsli"

struct VertexIn
{
    float4 position : POSITION0;
    float2 uv : TEXCOORD0;
    //float3 normal : NORMAL0;
    //float3 tangent : NORMAL1;
};

struct VertexOut
{
    float4 position : POSITION0;
    float2 uv : TEXCOORD0;
   // float3 normal : NORMAL0;
   // float3 tangent : NORMAL1;
};

VertexOut VS(VertexIn vin)
{
    VertexOut vout;


    //vout.position = vin.position;
    vout.position = mul(vin.position, _world);
    vout.position.w = 1;


    vout.uv = vin.uv;
    vout.position = vin.position;

    return vout;
}


cbuffer HSBuffer : register(b0)
{
    float3 _cameraPosition;
    float hpadding;
};




struct PatchTess
{
    float EdgeTess[4] : SV_TessFactor;
    float InsideTess[2] : SV_InsideTessFactor;
};

PatchTess ConstantHS(InputPatch<VertexOut, 4> patch, uint patchID : SV_PrimitiveID)
{
    PatchTess pt;

    float edgeDistance[4];
    edgeDistance[0] = distance(0.5f * (patch[0].position + patch[2].position).xyz, _cameraPosition);
    edgeDistance[1] = distance(0.5f * (patch[0].position + patch[1].position).xyz, _cameraPosition);
    edgeDistance[2] = distance(0.5f * (patch[1].position + patch[3].position).xyz, _cameraPosition);
    edgeDistance[3] = distance(0.5f * (patch[2].position + patch[3].position).xyz, _cameraPosition);

    pt.EdgeTess[0] = 64.0f / ceil(edgeDistance[0] / 50);
    pt.EdgeTess[1] = 64.0f / ceil(edgeDistance[1] / 50);
    pt.EdgeTess[2] = 64.0f / ceil(edgeDistance[2] / 50);
    pt.EdgeTess[3] = 64.0f / ceil(edgeDistance[3] / 50);

    float tess = 0.25f * (pt.EdgeTess[0] + pt.EdgeTess[1] + pt.EdgeTess[2] + pt.EdgeTess[3]);

    pt.InsideTess[0] = tess;
    pt.InsideTess[1] = tess;

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
    float gNormal : NORMAL1;
    
    float3 viewPosition : TEXCOORD5;
   
    float4 clip : SV_ClipDistance0;
};

Texture2D _map : register(t0);
Texture2D _worldNormalMap : register(t1);
SamplerState samp[3];

[domain("quad")]
DomainOut DS(PatchTess patchTess,
	float2 uv : SV_DomainLocation,
	const OutputPatch<HullOut, 4> quad)
{
    DomainOut dout;

    HullOut outData;

    float4 p1 = lerp(quad[0].position, quad[1].position, uv.x);
    float4 p2 = lerp(quad[2].position, quad[3].position, uv.x);
    outData.position = lerp(p1, p2, uv.y);


    float2 heightuv = outData.position.xz / 256;
    //�̹����� ���� uv ��ǥ ó��
    heightuv.y *= -1;
    heightuv.y += 1;
    
    outData.position.y = _map.SampleLevel(samp[0], heightuv, 0).r * 34.0f - 18.0f;


    float2 uv1 = lerp(quad[0].uv, quad[1].uv, uv.x);
    float2 uv2 = lerp(quad[2].uv, quad[3].uv, uv.x);
    outData.uv = lerp(uv1, uv2, uv.y);

    outData.normal = _worldNormalMap.SampleLevel(samp[0], heightuv, 0).rgb*2-1;

   
    
   // float3 t1 = lerp(quad[0].tangent, quad[1].tangent, uv.x);
   // float3 t2 = lerp(quad[2].tangent, quad[3].tangent, uv.x);
   // outData.tangent = lerp(t1, t2, uv.y);




    
    dout.position = outData.position;
    dout.worldPosition = dout.position.y;
    dout.position = mul(dout.position, _viewXprojection);

    dout.viewPosition = dout.position.xyw;
    dout.uv = outData.uv;
    
   // float3 n = mul(outData.normal, (float3x3) worldInverseTransposeMatrix);
   // float3 t = mul(outData.tangent, (float3x3) worldInverseTransposeMatrix);
   // float3 b = cross(n, t);
   //
   //
   // float3x3 tbnMatrix = float3x3(t.x, b.x, n.x,
	//                            t.y, b.y, n.y,
	//                            t.z, b.z, n.z);
   //

   // dout.lightDir = normalize(mul(-_lightDir, tbnMatrix));
   // dout.normal = normalize(mul(outData.normal, tbnMatrix));


    dout.uv = outData.uv;

    


    dout.gNormal = abs(outData.normal).y;


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

   // float3 normal = _normalMap.Sample(samp[0], uv).rgb;
   // normal = (normal * 2.0f) - 1.0f;
   // normal = normalize(normal);
   //
   // float3 light = normalize(input.lightDir);
   // float nDotL = saturate(dot(normal, light));
   // float4 intensity = ambient * globalAmbient + diffuse * nDotL;



    //�׸���
    float2 projectTexCoord;
    projectTexCoord.x = input.viewPosition.x / input.viewPosition.z / 2.0f + 0.5f;
    projectTexCoord.y = -input.viewPosition.y / input.viewPosition.z / 2.0f + 0.5f;
    
    float shadowValue = _lightMap.Sample(samp[1], projectTexCoord).g * 0.3f;
   // intensity *= shadowValue;





    //�ؽ��ĸ�
    float landNormal = input.gNormal;
   // blending.y -=   (blending.x + blending.z) /2;
    //blending = normalize(blending);
       
    float4 Mountain = _texture[2].Sample(samp[0], uv);
    float4 Grass = _texture[0].Sample(samp[0], uv);

    float4 landNmountain = lerp(Mountain, Grass, landNormal * landNormal * landNormal );


    float blendFactor2 = saturate((input.worldPosition + 8.0f) / 0.7f);
    float4 diffuseMap = lerp(_texture[1].Sample(samp[0], uv), landNmountain, blendFactor2);

    diffuseMap.a = input.clip;








    output.albedo = diffuseMap * shadowValue;

    float3 depthValue = input.position.w / 300;
    output.depthMap = float4(depthValue, 1);


   //output.viewSpaceVector.rgb = mul(input.worldNormal, (float3x3) cameraSpace);
   //
   //output.viewSpaceVector.a = 1;
    return output;
}
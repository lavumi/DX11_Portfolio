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

cbuffer SunBuffer : register(b2)
{
    float3 _lightDir;
    float _baseLight;
};

cbuffer ExtraBuffer : register(b3)
{
    matrix worldInverseTransposeMatrix;

}

struct VertexInput
{
    float4 position : POSITION0;
    float2 uv : TEXCOORD0;
    float3 normal : NORMAL0;
    float3 tangent : NORMAL1;
};

struct PixelInput
{
    float4 position : SV_POSITION;
    float2 uv : TEXCOORD0;
    float3 halfVector : TEXCOORD1;
    float3 lightDir : TEXCOORD2;
    float3 normal : NORMAL0;
    float3 viewDir : TEXCOORD3;



};




PixelInput main(  VertexInput input) 
{
    PixelInput output;

    input.position.w = 1.0f;

    output.position = mul(input.position, _world);


    float3 viewDir = _cameraPosition - output.position.xyz;
    



    float3 halfVector = normalize(normalize(-_lightDir) + normalize(viewDir));




    float3 n = mul(input.normal, (float3x3) worldInverseTransposeMatrix);
    float3 t = mul(input.tangent, (float3x3) worldInverseTransposeMatrix);
    float3 b = cross(n, t);
    float3x3 tbnMatrix = float3x3(t.x, b.x, n.x,
	                              t.y, b.y, n.y,
	                              t.z, b.z, n.z);
    output.halfVector = mul(halfVector, tbnMatrix);
    output.lightDir = mul(-_lightDir, tbnMatrix);
    output.viewDir = mul(viewDir, tbnMatrix);

   
    output.position = mul(output.position, _view);
    output.position = mul(output.position, _projection);
   
    output.uv = input.uv;
    output.normal = mul(input.normal, (float3x3) _world);




  
  // float lightIntensity = saturate(dot(-_lightDir, input.normal));
  // output.diffuse = _baseLight + lerp(lightIntensity, 0, step(lightIntensity, 0));
  
  
  
  
  // output.tangent = input.tangent;
  // output.bitangent = cross(input.tangent, input.normal);
   

    return output;

}
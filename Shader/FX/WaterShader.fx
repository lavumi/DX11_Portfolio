cbuffer MatrixBuffer : register(b10)
{
    matrix _world;


};
cbuffer MatrixBuffer : register(b11)
{
    matrix _view;
    matrix _projection;

};


cbuffer Camera : register(b1)
{
    float3 _cameraPosition;
    float _paddd;
}


cbuffer LightBuffer : register(b2)
{
    matrix _lightView;
    matrix _lightProjection;
    float3 _lightDir;
    float _baseLight;
};

cbuffer ExtraBuffer : register(b3)
{
    matrix worldInverseTransposeMatrix;
    float shadowBias;

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

    float3 normal : NORMAL0;
    float2 uv : TEXCOORD0;
    float3 halfVector : TEXCOORD1;
    float3 lightDir : TEXCOORD2;
    float3 viewDir : TEXCOORD3;
    float4 viewPosition : TEXCOORD5;
  
    float4 worldPosition : TEXCOORD6;
};




PixelInput VS(VertexInput input)
{
    PixelInput output;

    
    input.position.w = 1.0f;

    output.position = mul(input.position, _world);


    float3 viewDir = _cameraPosition - output.position.xyz;

    float3 halfVector =  normalize(normalize(-_lightDir) + normalize(viewDir));

   


    float3 n = mul(input.normal, (float3x3) worldInverseTransposeMatrix);
    float3 t = mul(input.tangent, (float3x3) worldInverseTransposeMatrix);
    float3 b = cross(n, t);
    float3x3 tbnMatrix = float3x3(t.x, b.x, n.x,
	                              t.y, b.y, n.y,
	                              t.z, b.z, n.z);

    output.halfVector =  mul(halfVector, tbnMatrix);
    output.lightDir = normalize(mul(-_lightDir, tbnMatrix));




    output.viewDir = viewDir;
   
    output.position = mul(output.position, _view);
    output.position = mul(output.position, _projection);
   
    output.viewPosition = output.position;

    output.uv = input.uv;
    output.normal = normalize(mul(input.normal, tbnMatrix));


   



    return output;

}





Texture2D _map : register(t0);
Texture2D _normalMap : register(t1);
Texture2D _perlin : register(t2);
Texture2D _reflectionMap : register(t3);
Texture2D _refractionMap : register(t4);

SamplerState samp[3];


cbuffer Material : register(b0)
{
    float4 ambient;
    float4 diffuse;
    float4 specular;
    float4 globalAmbient;
    float shininess;
    
};

cbuffer water : register(b1)
{
    float _translation;
    float _scale;
    float _angle;
    float padding;
}

struct PixelOutput
{
    float4 albedo : SV_TARGET0;
    float4 depthMap : SV_TARGET1;
    float4 viewSpaceVector : SV_TARGET2;
};

PixelOutput PS(PixelInput input) : SV_TARGET0
{

    //반사, 굴절된 이미지의 Texcoord 세팅
    float2 projectTexCoord;
    projectTexCoord.x = input.viewPosition.x / input.viewPosition.w / 2.0f + 0.5f;
    projectTexCoord.y = -input.viewPosition.y / input.viewPosition.w / 2.0f + 0.5f;


    //터레인의 a값에 저장된 수면부터의 거리값 받아와 깊이 설정
    //망함 이걸로 하면 안된다
    //뭔가 다른 방식으로 물의 깊이값을 받아오자
    float waterDepth = _refractionMap.Sample(samp[1], projectTexCoord).a;
    waterDepth = saturate(waterDepth * waterDepth*0.05f);



    //노말맵을 서로 다른 크기, 서로 다른 방향으로 흘러보내 불규칙한 모습을 구현
    float2 temp_uv = input.uv * _scale;
    float2 temp_uv2 = input.uv * _scale/2;
  
    //흐르는 셋팅
    temp_uv.y += _translation * cos(_angle);
    temp_uv.x -= _translation * sin(_angle);

    temp_uv2.x -= _translation * cos(_angle);
    temp_uv2.y += _translation * sin(_angle);
    
    //노말맵 각도 회전
    
    temp_uv -= _scale/2;

    float2 uv;
    uv.x = temp_uv.x * cos(_angle) + temp_uv.y * sin(_angle);
    uv.y = temp_uv.y * cos(_angle) - temp_uv.x * sin(_angle);
    uv += _scale / 2;

    float2 uv2;
    uv2.x = temp_uv2.x * cos(_angle) + temp_uv2.y * sin(_angle);
    uv2.y = temp_uv2.y * cos(_angle) - temp_uv2.x * sin(_angle);
    uv2 += _scale / 2;

    float3 normal = _normalMap.Sample(samp[0], uv).rgb;
    float3 normal2 = _normalMap.Sample(samp[0], uv2).rgb;


    normal = (normal * 2 + normal2*1) / 3;
    normal = (normal * 2.0f) - 1.0f;
 //   normal = normalize(normal);


    normal = lerp(float3(0, 0, 1), normal, waterDepth);


    normal = normalize(normal * 0.3f);
    //블린퐁
    float3 halfVector = normalize(input.halfVector);
    float3 light = normalize(input.lightDir);

    float nDotL = saturate(dot(normal, light));
    float nDotH = saturate(dot(normal, halfVector));

    float power = pow(nDotH, shininess);

    float4 intensity = ambient * globalAmbient + diffuse * nDotL + specular * power;



    //이미지 흔들리는 효과를 위해 노멀맵과 연산
    projectTexCoord += normal.x * 0.05f;
    

    float4 reflection = _reflectionMap.Sample(samp[1], projectTexCoord);
    float4 refraction = _refractionMap.Sample(samp[1], projectTexCoord);



    float3 viewDir = normalize(input.viewDir);




    float blendFactor = dot(float3(0, 1, 0), viewDir);

    blendFactor = 0.02037f + 0.97963f * (1 - blendFactor) * (1 - blendFactor) * (1 - blendFactor) * (1 - blendFactor) * (1 - blendFactor);
   

    float4 diffuse = lerp(refraction, reflection, blendFactor);
            


    PixelOutput output;
    output.albedo = diffuse * intensity;
    float3 depthValue = input.position.w / 300;
    output.depthMap = float4(depthValue, 1);

    return output;
}



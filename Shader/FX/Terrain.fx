cbuffer MatrixBuffer : register(b10)
{
    matrix _world;
};



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
}

cbuffer ReflectionPlane : register(b11)
{
    float4 clipPlane;
};




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
    float worldPosition : TEXCOORD1;
    float3 lightDir : TEXCOORD2;
    float3 normal : NORMAL0;
    float3 worldNormal : NORMAL1;
    
    float3 viewPosition : TEXCOORD5;

    float clip : SV_ClipDistance0;
};




PixelInput VS(VertexInput input)
{



    PixelInput output;

    
    input.position.w = 1.0f;

    output.position = mul(input.position, _world);
    output.worldPosition = output.position.y;
   //output.position = mul(output.position, _view);
   //output.position = mul(output.position, _projection);
   


     output.viewPosition = output.position.xyw;

    output.uv = input.uv;

    float3 n = mul(input.normal, (float3x3) worldInverseTransposeMatrix);
    float3 t = mul(input.tangent, (float3x3) worldInverseTransposeMatrix);
    float3 b = cross(n, t);

    float3x3 tbnMatrix = float3x3(t.x, b.x, n.x,
	                            t.y, b.y, n.y,
	                            t.z, b.z, n.z);


    output.lightDir = normalize(mul(-_lightDir, tbnMatrix));



    output.uv = input.uv;
    output.normal = normalize(mul(input.normal, tbnMatrix));
    output.worldNormal = abs(input.normal);


    output.clip = dot(mul(input.position, _world), clipPlane);

    return output;

}


cbuffer MatrixBuffer : register(b0)
{
    matrix _view;
    matrix _projection;
};




static int arrayindex[30]=
{
4, 1, 1,
1, 4, 1,
1, 1, 4,
3, 2, 1,
3, 1, 2,
2, 3, 1, 
2, 1, 3,
1, 2, 3,
1, 3, 2,
2, 2, 2
};


[maxvertexcount(51)]
void GS(triangle PixelInput input[3], inout TriangleStream<PixelInput> TriStream)
{

    PixelInput output;
    output = input[0];


    output.position = mul(output.position, _view);
    output.position = mul(output.position, _projection);
    output.viewPosition = output.position.xyw;
    TriStream.Append(output);


    output = input[1];
    output.position = mul(output.position, _view);
    output.position = mul(output.position, _projection);
     output.viewPosition = output.position.xyw;
    TriStream.Append(output);



    output = input[2];
    output.position = mul(output.position, _view);
    output.position = mul(output.position, _projection);
     output.viewPosition = output.position.xyw;
    TriStream.Append(output);
   

    TriStream.RestartStrip();


    float3 blending = input[0].worldNormal;
    blending.y -= (blending.x + blending.z) / 2;
    

    if (input[0].position.y > -7.7f && blending.y > 0.8f)
    {
        int a, b, c;
        float grassHeight = 1;
        
        for (int i = 0; i < 10; i++)
        {
            a = arrayindex[i*3];
            b = arrayindex[i*3 + 1];
            c = arrayindex[i*3 + 2];

            float4 centerPos = output.position = (input[0].position * a + input[1].position * b + input[2].position * c) / (a + b + c);


            output = input[0];
            output.position = centerPos;
            output.position = mul(output.position, _view);
            output.position = mul(output.position, _projection);
             output.viewPosition = output.position.xyw;
            TriStream.Append(output);


            output = input[0];
            output.position = centerPos;
            output.position.y += grassHeight/2;
            output.position.x -= 0.05f;
            output.position = mul(output.position, _view);
            output.position = mul(output.position, _projection);
             output.viewPosition = output.position.xyw;
            TriStream.Append(output);


            output = input[0];
            output.position = centerPos;
            output.position.y += grassHeight / 2;
            output.position.x += 0.05f;
            output.position = mul(output.position, _view);
            output.position = mul(output.position, _projection);
             output.viewPosition = output.position.xyw;
            TriStream.Append(output);


   
            TriStream.RestartStrip();




            TriStream.Append(output);

            output = input[0];
            output.position = centerPos;
            output.position.y += grassHeight / 2;
            output.position.x -= 0.05f;
            output.position = mul(output.position, _view);
            output.position = mul(output.position, _projection);
             output.viewPosition = output.position.xyw;
            TriStream.Append(output);

            output = input[0];
            output.position = centerPos;
            output.position.y += grassHeight;
            output.position = mul(output.position, _view);
            output.position = mul(output.position, _projection);
             output.viewPosition = output.position.xyw;
            TriStream.Append(output);

            TriStream.RestartStrip();
        }
        
    }
    
}














Texture2D _map[3] : register(t10);

Texture2D _lightMap : register(t1);
Texture2D _normalMap : register(t2);

SamplerState samp[3];


cbuffer Material : register(b0)
{
    float4 ambient;
    float4 diffuse;
    float4 globalAmbient;
   
};


cbuffer cameraSpace : register(b1)
{
    matrix cameraSpace;
   
};

struct PixelOut
{
    float4 albedo : SV_TARGET0;
    float4 depthMap : SV_TARGET1;
    float4 viewSpaceVector : SV_TARGET2;
};

PixelOut PS(PixelInput input)
{
    //float ddx = fwidth(input.worldPosition.z);
    //return ddx;
    PixelOut output;


    float2 uv = input.uv;

    float3 normal =  _normalMap.Sample(samp[0], uv).rgb;
    normal = (normal * 2.0f) - 1.0f;
    normal = normalize(normal);

    float3 light = normalize(input.lightDir);
    float nDotL = saturate(dot(normal, light));
    float4 intensity = ambient * globalAmbient + diffuse * nDotL;



    //그림자
    float2 projectTexCoord;
    projectTexCoord.x = input.viewPosition.x / input.viewPosition.z / 2.0f + 0.5f;
    projectTexCoord.y = -input.viewPosition.y / input.viewPosition.z / 2.0f + 0.5f;
    
    float shadowValue = _lightMap.Sample(samp[1], projectTexCoord).g*0.3f;
    intensity *= shadowValue;





    //텍스쳐링
    float3 blending = input.worldNormal;
    blending.y -= (blending.x + blending.z) / 2;
    blending = normalize(max(blending, 0.0001f));
   
    float4 Mountain = _map[2].Sample(samp[0], uv);
    float4 Grass = _map[0].Sample(samp[0], uv);
   
    float4 landNmountain = Mountain * blending.x + Grass * blending.y + Mountain * blending.z;



    float blendFactor = saturate((input.worldPosition + 7.7f) / 0.4f);

    float4 diffuseMap = lerp(_map[1].Sample(samp[0], uv), landNmountain, blendFactor);

    diffuseMap.a = input.clip;




 

    output.albedo = intensity * diffuseMap;

    float3 depthValue =  input.position.w / 300;
    output.depthMap = float4(depthValue, 1);


    output.viewSpaceVector.rgb = mul(input.worldNormal, (float3x3) cameraSpace);

    output.viewSpaceVector.a = 1;
    return output;
}


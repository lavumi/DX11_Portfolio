cbuffer MatrixBuffer : register(b10)
{
    matrix _world;


};
cbuffer MatrixBuffer : register(b11)
{
    matrix _view;
    matrix _projection;

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
    matrix _inverseWorld;
    float4 options;

}

cbuffer MirrorBuffer : register(b4)
{
    matrix _mirrorview;
    matrix _mirrorprojection;
    float3 _mirrorcameraPosition;
    float _padddd;
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
    float3 halfVector : TEXCOORD1;
    float3 lightDir : TEXCOORD2;
    float3 normal : NORMAL0;
    float3 viewDir : TEXCOORD3;

    //float options : TEXCOORD4;
    float4 viewPosition : TEXCOORD5;


};




PixelInput VS(VertexInput input)
{
    PixelInput output;

    input.position.w = 1.0f;

    output.position = mul(input.position, _world);


    float3 viewDir = _mirrorcameraPosition - output.position.xyz;

    
    float3 halfVector = normalize(normalize(-_lightDir) + normalize(viewDir));




    float3 n = mul(input.normal, (float3x3) _inverseWorld);
    float3 t = mul(input.tangent, (float3x3) _inverseWorld);
    float3 b = cross(n, t);
    float3x3 tbnMatrix = float3x3(t.x, b.x, n.x,
	                              t.y, b.y, n.y,
	                              t.z, b.z, n.z);
    output.halfVector = mul(halfVector, tbnMatrix);
    output.lightDir = normalize(mul(-_lightDir, tbnMatrix));
    output.viewDir = mul(viewDir, tbnMatrix);




    output.viewPosition = output.position;

    output.viewPosition = mul(output.viewPosition, _view);
    output.viewPosition = mul(output.viewPosition, _projection);

    output.position = mul(output.position, _mirrorview);
    output.position = mul(output.position, _mirrorprojection);

    output.uv = input.uv;
    output.normal = normalize(mul(input.normal, tbnMatrix));

    //output.options = options.x;
   

    return output;
}





Texture2D _map : register(t0);
Texture2D _normalMap : register(t1);
Texture2D _heightMap : register(t2);
Texture2D _lightMap : register(t3);

SamplerState samp[3];


cbuffer Material : register(b0)
{
    float4 ambient;
    float4 diffuse;
    float4 specular;
    float4 globalAmbient;
    float shininess;
    
};

cbuffer parallex : register(b1)
{
    float scale;
    float layer;
    float drawTexture;
}

float4 PStest(PixelInput input) : SV_TARGET
{
    float4 diffuseMap = _normalMap.Sample(samp[0], input.uv);
    return float4(input.viewDir, 1);
}

float4 PS(PixelInput input) : SV_TARGET0
{

    //노말맵의 layer 
    float2 uv;
    float3 viewDir = normalize(input.viewDir);
    float layer_depth = 1.0 / layer;
    float cur_layer_depth = 0.0;
    float2 delta_uv = viewDir.xy * scale / (viewDir.z * layer);
    float2 cur_uv = input.uv;



    float depth_from_tex = _heightMap.Sample(samp[0], cur_uv).r;
    for (int layerstep = 0; layerstep < 32; layerstep++)
    {
        cur_layer_depth += layer_depth;
        cur_uv -= delta_uv;
        depth_from_tex = _heightMap.Sample(samp[0], cur_uv).r;
        if (depth_from_tex < cur_layer_depth)
        {
            break;
        }
    }
    uv = cur_uv;
           

    float2 prev_uv = cur_uv + delta_uv;
    float next = depth_from_tex - cur_layer_depth;
    float prev = _heightMap.Sample(samp[0], prev_uv).r - cur_layer_depth + layer_depth;
    float weight = next / (next - prev);
    uv = lerp(cur_uv, prev_uv, weight);
    

    //여기까지 parallex occlusive normalmap의 uv 계산




    //blin pong 쉐이딩
    float3 light = normalize(input.lightDir);
    float4 diffuseMap = _map.Sample(samp[2], uv);

    float3 normal = _normalMap.Sample(samp[0], uv).rgb;
    normal = (normal * 2.0f) - 1.0f;
    normal = normalize(normal);
    float3 halfVector = normalize(input.halfVector);


    float nDotL = saturate(dot(normal, light));
    float nDotH = saturate(dot(normal, halfVector));

    float power = pow(nDotH, shininess);

    float4 intensity = ambient * globalAmbient + diffuse * nDotL;//    +specular * power;

  

   //그림자 계산 시작
   float shadow = 0.3f;
   float2 projectTexCoord;
   
   projectTexCoord.x = input.viewPosition.x / input.viewPosition.w / 2.0f + 0.5f;
   projectTexCoord.y = -input.viewPosition.y / input.viewPosition.w / 2.0f + 0.5f;
   
   
   float shadowValue = _lightMap.Sample(samp[0], projectTexCoord).g;
   shadowValue *= 0.3;
   
   
    //intensity *= shadowValue;
    diffuseMap *= intensity;

    diffuseMap.a = 0.5f;

    return diffuseMap;
}



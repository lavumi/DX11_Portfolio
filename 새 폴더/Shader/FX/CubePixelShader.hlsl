Texture2D _map : register(t0);
Texture2D _normalMap : register(t1);
Texture2D _heightMap : register(t2);

SamplerState samp;


cbuffer SunBuffer : register(b0)
{
    float3 _lightDir;
    float _baseLight;
};

cbuffer Material : register(b1)
{
    float4 ambient;
    float4 diffuse;
    float4 specular;
    float4 globalAmbient;
    float shininess;
    
};

cbuffer parallex : register(b2)
{
    float scale;
    float layer;
    float drawTexture;
}


struct PixelInput
{
    float4 position : SV_POSITION;
    float2 uv : TEXCOORD0;
    float3 halfVector : TEXCOORD1;
    float3 lightDir : TEXCOORD2;
    float3 normal : NORMAL0;
    
    float3 viewDir : TEXCOORD3;

};





float4 NoBump(PixelInput input) : SV_TARGET
{


    float4 diffuseMap = lerp(_map.Sample(samp, input.uv), float4(0.4, 0.2, 1, 1), step(drawTexture, 0.5));


    float3 lightDir = -_lightDir;

    float4 lightIntensity = saturate(dot(input.normal, lightDir) + ambient * globalAmbient);



    float4 color = saturate(diffuseMap * lightIntensity);

    //color = color * diffuseMap;
    return     color;


}

float4 NormalMapping(PixelInput input) : SV_TARGET
{


    float4 diffuseMap = lerp(_map.Sample(samp, input.uv), float4(0.4, 0.2, 1, 1), step(drawTexture, 0.5));




    float4 bumpMap = _normalMap.Sample(samp, input.uv);
    bumpMap = (bumpMap * 2.0f) - 1.0f;
 
  // float3 bumpNormal = input.normal + bumpMap.x * input.tangent + bumpMap.y * input.bitangent;
 
  // bumpNormal = normalize(bumpNormal);
 
 
    float3 light = normalize(input.lightDir);
  
    float lightIntensity = saturate(dot(bumpMap.rgb, light)) * 2;
 
 
 
    float4 color = saturate(diffuseMap * lightIntensity);
 
  // color = color * diffuseMap;


    return color;


}

float4 Blinn_Phong(PixelInput input) : SV_TARGET
{
    float4 diffuseMap = lerp(_map.Sample(samp, input.uv), float4(0.4, 0.2, 1, 1), step(drawTexture, 0.5));


    float3 halfVector = normalize(input.halfVector);





    float3 normal = _normalMap.Sample(samp, input.uv).rgb;

    normal = (normal * 2.0f) - 1.0f;
 
    normal = normalize(normal);

    float3 light = normalize(input.lightDir);

    float nDotL = saturate(dot(normal, light));
    float nDotH = saturate(dot(normal, halfVector));

    float power = pow(nDotH, shininess);

    float4 intensity = ambient * globalAmbient + diffuse * nDotL + specular * power;

    return intensity * diffuseMap;
}


float4 ParallexMapping(PixelInput input) : SV_TARGET
{
  


    float2 uv;

    float height = _heightMap.Sample(samp, input.uv).r;
        

    float3 viewDir = normalize(input.viewDir);
    uv = input.uv - (height * viewDir.xy * scale) / viewDir.z;




    float3 normal = _normalMap.Sample(samp, uv).rgb;
    normal = (normal * 2.0f) - 1.0f;
 
    normal = normalize(normal);

    float3 light = normalize(input.lightDir);
    float3 halfVector = normalize(input.halfVector);

    float nDotL = saturate(dot(normal, light));
    float nDotH = saturate(dot(normal, halfVector));

    float power = pow(nDotH, shininess) ;

    float4 intensity = ambient * globalAmbient + diffuse *nDotL + specular  * power;


    float4 diffuseMap = lerp(_map.Sample(samp, uv), float4(0.4, 0.2, 1, 1), step(drawTexture, 0.5));
    return intensity * diffuseMap;
}



float4 ParallexOcclusionMapping(PixelInput input) : SV_TARGET
{
    float2 uv;
  

    float3 viewDir = normalize(input.viewDir);
    float layer_depth = 1.0 / layer;
    float cur_layer_depth = 0.0;
    float2 delta_uv = viewDir.xy * scale / (viewDir.z * layer);
    float2 cur_uv = input.uv;

    float depth_from_tex = _heightMap.Sample(samp, cur_uv).r;

    //layerstep;
    for (int layerstep = 0; layerstep < 32; layerstep++)
    {
        cur_layer_depth += layer_depth;
        cur_uv -= delta_uv;
        depth_from_tex = _heightMap.Sample(samp, cur_uv).r;
        if (depth_from_tex < cur_layer_depth)
        {
            break;
        }
    }


   uv = cur_uv;
           
   float2 prev_uv = cur_uv + delta_uv;
   float next = depth_from_tex - cur_layer_depth;
   float prev = _heightMap.Sample(samp, prev_uv).r - cur_layer_depth + layer_depth;
   float weight = next / (next - prev);
   uv = lerp(cur_uv, prev_uv, weight);
    

    float3 light = normalize(input.lightDir);






    float4 diffuseMap = lerp(_map.Sample(samp, uv), float4(0.4, 0.2, 1, 1), step(drawTexture, 0.5));
    float3 normal = _normalMap.Sample(samp, uv).rgb;
    normal = (normal * 2.0f) - 1.0f;
 
    normal = normalize(normal);

    
    float3 halfVector = normalize(input.halfVector);

    float nDotL = saturate(dot(normal, light));
    float nDotH = saturate(dot(normal, halfVector));

    float power = pow(nDotH, shininess);

    float4 intensity = ambient * globalAmbient + diffuse *  nDotL + specular * power;

    return intensity * diffuseMap;
}


float4 Shadow(PixelInput input) : SV_TARGET
{
    float2 uv;
  

    float3 viewDir = normalize(input.viewDir);
    float layer_depth = 1.0 / layer;
    float cur_layer_depth = 0.0;
    float2 delta_uv = viewDir.xy * scale / (viewDir.z * layer);
    float2 cur_uv = input.uv;

    float depth_from_tex = _heightMap.Sample(samp, cur_uv).r;

    //layerstep;
    for (int layerstep = 0; layerstep < 32; layerstep++)
    {
        cur_layer_depth += layer_depth;
        cur_uv -= delta_uv;
        depth_from_tex = _heightMap.Sample(samp, cur_uv).r;
        if (depth_from_tex < cur_layer_depth)
        {
            break;
        }
    }


    uv = cur_uv;
           
    float2 prev_uv = cur_uv + delta_uv;
    float next = depth_from_tex - cur_layer_depth;
    float prev = _heightMap.Sample(samp, prev_uv).r - cur_layer_depth + layer_depth;
    float weight = next / (next - prev);
    uv = lerp(cur_uv, prev_uv, weight);
    

    float3 light = normalize(input.lightDir);




    //그림자 만들기
    float shadowIntensity = 0;
    float2 shadowDelta_uv = light.xy * scale / (light.z * layer);
    cur_layer_depth -= layer_depth;
    int totalStep = layerstep;
    for (; layerstep > 0; layerstep--)
    {
        cur_layer_depth -= layer_depth;
        cur_uv = shadowDelta_uv * layerstep;
        depth_from_tex = _heightMap.Sample(samp, cur_uv).r;
        if (depth_from_tex < cur_layer_depth)
        {
            shadowIntensity += (cur_layer_depth - depth_from_tex) * (1 - (float) layerstep / (float) totalStep);
        }
    }
   
    shadowIntensity = saturate(1 - shadowIntensity);




    float4 diffuseMap = lerp(_map.Sample(samp, uv), float4(0.4, 0.2, 1, 1), step(drawTexture, 0.5));
    float3 normal = _normalMap.Sample(samp, uv).rgb;
    normal = (normal * 2.0f) - 1.0f;
 
    normal = normalize(normal);

    
    float3 halfVector = normalize(input.halfVector);

    float nDotL = saturate(dot(normal, light));
    float nDotH = saturate(dot(normal, halfVector));

    float power = pow(nDotH, shininess);

    float4 intensity = ambient * globalAmbient + diffuse * shadowIntensity * nDotL + specular * power;

    return intensity * diffuseMap;
}
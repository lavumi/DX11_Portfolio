#include "ShaderPreset.hlsli"


struct VertexInput
{
    float4 position : POSITION0;
    float4 world0 : INSTMATRIX0;
    float4 world1 : INSTMATRIX1;
    float4 world2 : INSTMATRIX2;
    float4 world3 : INSTMATRIX3;
};


struct PixelInput
{
    float4 position : SV_POSITION;
    float4 lightWorldPosition[4] : LIGHTTEX;
    
};




PixelInput VS(VertexInput input)
{
    PixelInput output;

    input.position.w = 1.0f;

    float4x4 world = float4x4(input.world0, input.world1, input.world2, input.world3);
    output.position = mul(input.position, world);

    float4 lightWorldPosition;
    lightWorldPosition = MulLightVP(output.position);
    
    
   [unroll] for (int i = 0; i < 3; i++)
   {
      output.lightWorldPosition[i] = mul(lightWorldPosition, cropMatrix[i]);
   }

    //3번에는 view matrix 들어있다
    output.lightWorldPosition[3] = MulVP(output.position);

    output.position = MulVP(output.position);


    return output;

}








Texture2DArray _lightMap : register(t13);


SamplerComparisonState samp[3];

//static const float splitedFrustum[3] = { 7.5f, 60.0f, 200.0f };
//static const float splitedFrustum[3] = { 0.997112471f, 0.998811450f, 1 };

float4 PS(PixelInput input) : SV_TARGET
{
    float fDistance = input.lightWorldPosition[3].z / input.lightWorldPosition[3].w;
    float shadow = 1;
   
    [unroll] for (int i = 0; i < 3; i++)
    {
    
        if (fDistance < splitedFrustum[i])
        {
            float3 projectTexCoord;
            float lightDepthValue;
           
            projectTexCoord.x =  input.lightWorldPosition[i].x / input.lightWorldPosition[i].w / 2.0f + 0.5f;
            projectTexCoord.y = -input.lightWorldPosition[i].y / input.lightWorldPosition[i].w / 2.0f + 0.5f;
            projectTexCoord.z = i;
            lightDepthValue = input.lightWorldPosition[i].z / input.lightWorldPosition[i].w;
            float bias = 0.0001f * pow(i + 1, 4);

             //라이트 텍스쳐가 존재하는 부분만 그림자 처리 연산
            if ((saturate(projectTexCoord.x) == projectTexCoord.x) &&
                 (saturate(projectTexCoord.y) == projectTexCoord.y) &&
                 (lightDepthValue > 0))
            {
        
               //marginBias = acos(saturate(dot(normal, lightDir)));
               //epsilon = 0.001 / marginBias;
               //epsilon = clamp(epsilon, 0, 0.1f);
                shadow = _lightMap.SampleCmpLevelZero(samp[2], projectTexCoord, lightDepthValue - bias) + 0.3f;
                shadow = saturate(shadow);

            }
           break;
        }
    }
   // shadow += 0.3f;
    return float4(shadow, shadow, shadow, 1);
 
}







float4 PS2(PixelInput input) : SV_TARGET
{
   // float3 normal = normalize(input.normal);
    float3 lightDir = normalize(_lightDir.xyz);
    float shadow = 0.2f;
    float3 projectTexCoord;
    float lightDepthValue;
    float marginBias, epsilon, bias;
    float lightIntensity;
   
   
    
   
   
   
 //   projectTexCoord.x = input.lightWorldPosition.x / input.lightWorldPosition.w / 2.0f + 0.5f;
 //   projectTexCoord.y = -input.lightWorldPosition.y / input.lightWorldPosition.w / 2.0f + 0.5f;
 //   projectTexCoord.z = 1;
 //   lightDepthValue = input.lightWorldPosition.z / input.lightWorldPosition.w;
   
   
    bias = 0.0015f;
    //라이트 텍스쳐가 존재하는 부분만 그림자 처리 연산
    if ((saturate(projectTexCoord.x) == projectTexCoord.x) &&
        (saturate(projectTexCoord.y) == projectTexCoord.y) &&
        (lightDepthValue > 0))
    {
        
      //  marginBias = acos(saturate(dot(normal, lightDir)));
        epsilon = 0.001 / marginBias;
        epsilon = clamp(epsilon, 0, 0.1f);
        shadow = _lightMap.SampleCmpLevelZero(samp[2], projectTexCoord, lightDepthValue - bias) + 0.3f;
        shadow = saturate(shadow);
   
   

        //***************cmplvlzero 사용안하는 버젼*****************   
        //                                                            
        //depthValue = _lightMap.Sample(samp[2], projectTexCoord).r;  
        //                                                            
        ////TODO : bias 외부에서 받아서 셋팅할수 있게 하자              
        // lightDepthValue = lightDepthValue - epsilon;               
        //                                                            
        //if (lightDepthValue < depthValue)                           
        //{                                                           
        //    lightIntensity = saturate(dot(normal, lightDir));       
        //                                                            
        //    if (lightIntensity > 0.01f)                             
        //    {                                                       
        //        shadow = 1;                                         
        //    }                                                       
        //                                                            
        //}                                                           
        //                                                            
        //*********************************************************         
    }
}







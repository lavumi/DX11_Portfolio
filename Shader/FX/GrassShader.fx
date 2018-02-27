cbuffer MatrixBuffer : register(b10)
{
    matrix _world;


};

cbuffer MatrixBuffer : register(b0)
{
    matrix _view;
    matrix _projection;

};

cbuffer Camera : register(b1)
{
    float3 _cameraPosition;
    float _paddd;
}

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
};

PixelInput VS(VertexInput input)
{
    PixelInput output;
    input.position.w = 1;

    float4x4 world = float4x4(input.world0, input.world1, input.world2, input.world3);

   // float4x4 worldtemp = float4x4(1, 0, 0, 0,
   // 0, 1, 0, 0,
   // 0, 0, 1, 0,
   // 0, 0, 0, 1);

   // output.position = input.position;
    output.position = mul(input.position, world);
    //output.position = mul(output.position, _view);
    //output.position = mul(output.position, _projection);

    return output;
}

//cbuffer MatrixBuffer : register(b0)
//{
//    matrix _view;
//    matrix _projection;

//};


[maxvertexcount(3)]
void GS(point PixelInput input[1], inout TriangleStream<PixelInput> triStream)
{
    
    PixelInput output;

    
   //for (int i = 0; i < 3; i++)
   //{
   //
   //    output = input[i];
   //    output.position = mul(output.position, _view);
   //    output.position = mul(output.position, _projection);
   //    triStream.Append(output);
   //}
   //triStream.RestartStrip();
   //return;

    for (int i = 0; i < 1; i ++)
    {
        output = input[i];
        output.position = mul(output.position, _view);
        output.position = mul(output.position, _projection);

        triStream.Append(output);
   //triStream.Append(input[1]);
   //triStream.Append(input[2]);
   //triStream.RestartStrip();




        output = input[i];
        output.position.y += 1;
        output.position = mul(output.position, _view);
        output.position = mul(output.position, _projection);
        triStream.Append(output);

   

        output = input[i];
        output.position.x += 0.5f;
        output.position = mul(output.position, _view);
        output.position = mul(output.position, _projection);
        triStream.Append(output);




        triStream.RestartStrip();
    }
   

    //float grassheight = 1.0f;
    //
    //PixelInput base = input[0];
    //PixelInput output;
    //output = base;
    //output.position = mul(output.position, _view);
    //output.position = mul(output.position, _projection);
    //triStream.Append(output);
    //
    //output = base;
    //output.position.y += grassheight / 2;
    //output.position.x += 1.1f;
    //output.position = mul(output.position, _view);
    //output.position = mul(output.position, _projection);
    //triStream.Append(output);
    //
    //
    //output = base;
    //output.position.y += grassheight / 2;
    //output.position.x -= 1.1f;
    //output.position = mul(output.position, _view);
    //output.position = mul(output.position, _projection);
    //
    //triStream.Append(output);
    //
    //triStream.RestartStrip();
    //
    //triStream.Append(output);
    //
    //output = base;
    //output.position.y += grassheight / 2;
    //output.position.x += 1.1f;
    //output.position = mul(output.position, _view);
    //output.position = mul(output.position, _projection);
    //triStream.Append(output);
    //
    //output = base;
    //output.position.y += grassheight;
    //output.position = mul(output.position, _view);
    //output.position = mul(output.position, _projection);
    //triStream.Append(output);
    //
    //
    //triStream.RestartStrip();
}



float4 PS(PixelInput input) : SV_Target
{
    float4 color = float4(0,1,0,1);

   

    return  color;

}
//-----------------------------------------------------------------------------------------
// Input and Output Structures
//-----------------------------------------------------------------------------------------
cbuffer CB_CAMERA
{
  float4x4 mWorld;
  float4x4 mView;
  float4x4 mProj;
};

struct VSIn
{
	float4 position		: POSITION;
	float3 normal     : NORMAL;
	float4 color      : COLOR;
};

struct PSIn
{
	float4 Pos		: SV_Position;
	//float3 Color	: COLOR;
};


//-----------------------------------------------------------------------------------------
// VertexShader: VSScene
//-----------------------------------------------------------------------------------------
PSIn FVertexShader(VSIn input)
{
	PSIn output = (PSIn)0;

   output.Pos		= input.position; 
	//output.Pos = mul(input.position, mWorld);
	//output.Pos = mul(output.Pos, mView);
  //output.Pos = mul(output.Pos, mProj);

	return output;

}
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
	float4 normal     : NORMAL;
	float4 color      : COLOR;
};

struct PSIn
{
	float4 Pos		: SV_Position;
	float4 Texture  : TEXTCOORD; 
	//float3 Color	: COLOR;
};


//-----------------------------------------------------------------------------------------
// VertexShader: VSScene
//-----------------------------------------------------------------------------------------
PSIn FVertexShader(VSIn input)
{
	PSIn output = (PSIn)0;

  output.Pos		= input.position; 
  
	float4 Pos = mul(input.position, mWorld);
	Pos = mul(Pos, mView);
  Pos = mul(Pos, mProj);

  float invWidth  = 1/ 800;
  float invHeight = 1/ 600;
  
  //Projected co-ords.
  output.Texture     = output.Pos;
  output.Texture.xy  = float2(output.Texture.x * 0.5, -output.Texture.y * 0.5);
  output.Texture.xy += (0.5 * output.Texture.w);  
  //output.Texture.x  += 0.5f * invWidth * output.Texture.w;
  //output.Texture.y  += 0.5f * invHeight * output.Texture.w;
	
	return output;

}
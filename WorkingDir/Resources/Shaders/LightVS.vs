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
	float2 TextCoord  : TEXCOORD; 
};

struct PSIn
{
	float4 Pos		: SV_Position;
	float2 TextCoord  : TEXCOORD; 
};


//-----------------------------------------------------------------------------------------
// VertexShader: VSScene
//-----------------------------------------------------------------------------------------
PSIn FVertexShader(VSIn input)
{
	PSIn output = (PSIn)0;

	output.Pos		= input.position; 
  

  float invWidth  = 1/ 800;
  float invHeight = 1/ 600;
  
  //Projected co-ords.
  output.TextCoord     = output.Pos;
  //output.TextCoord = float2(output.TextCoord.x * 0.5, -output.TextCoord.y * 0.5);
  //output.TextCoord.xy += (0.5 * output.TextCoord.w); 
   
  output.TextCoord.x  -= invWidth ;
  output.TextCoord.y  -= invHeight ;
	
	return output;

}
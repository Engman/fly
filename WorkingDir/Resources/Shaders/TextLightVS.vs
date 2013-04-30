
//-----------------------------------------------------------------------------------------
// Input and Output Structures
//-----------------------------------------------------------------------------------------
cbuffer CB_CAMERA
{
  float4x4 mWorld;
  float4x4 mView;
  float4x4 mProj;
  float4x4 mWorldInvTrans;
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
  
  
  //Projected co-ords.
  output.TextCoord     = input.TextCoord;
  
  //output.TextCoord.xy  = float2(output.TextCoord.x * 0.5, -output.TextCoord.y * 0.5);
  //output.TextCoord.xy += (0.5); //* output.TextCoord.w); 
   
  //output.TextCoord.x  -= invWidth ;
  //output.TextCoord.y  -= invHeight ;
	
	return output;
}
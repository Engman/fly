struct VSIn
{
	float4 position		: POSITION;
	float2 TextCoord  	: TEXCOORD;

};

struct PSIn
{
	float4 Pos			: SV_Position;
	float4 ScreenPos	: Position;
	float2 TextCoord  	: TEXCOORD; 

};

//-----------------------------------------------------------------------------------------
// VertexShader: VSScene
//-----------------------------------------------------------------------------------------
PSIn FVertexShader(VSIn input)
{
	PSIn output = (PSIn)0;
	//fullscreen quad already in view space
	output.Pos		= input.position; 
	output.ScreenPos = input.position;
	output.TextCoord     = input.TextCoord;   
	
	return output;
}
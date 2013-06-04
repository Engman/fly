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

struct VS_IN
{
  float4 position 	: POSITION;
  //float4 normal 	: NORMAL;
  //float2 textCoord 	: TEXCOORD;
};

struct PS_IN
{
  float4 position 		: SV_POSITION;
  float4 ScreenPos 		: TEXCOORD;
  //float2 textCoord 		: TEXCOORD;
};


//-----------------------------------------------------------------------------------------
// VertexShader: VSScene
//-----------------------------------------------------------------------------------------
PS_IN FVertexShader(VS_IN input)
{
	PS_IN output;
  
	output.position.w = 1.0f;
	float4 positionWorld = mul( input.position, mWorld );

	output.position = mul(  positionWorld, mView );
	output.position = mul(  output.position, mProj );
	
	float4 pos = output.position; 
	output.ScreenPos = pos;  
	output.ScreenPos.z = 0; 
	//output.textCoord = input.textCoord;

	return output;
}
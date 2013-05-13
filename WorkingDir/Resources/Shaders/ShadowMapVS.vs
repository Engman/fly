
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
	float4 normal 		: NORMAL;
	float2 TextCoord  	: TEXCOORD;

};


//-----------------------------------------------------------------------------------------
// VertexShader: VSScene
//-----------------------------------------------------------------------------------------
float4 FVertexShader(VSIn input): SV_Position
{
	float4 position;

  position.w = 1.0f;
  position = mul( input.position, mWorld );
  
  position = mul(  position, mView );
  position = mul(  position, mProj );

	
	return position;
}
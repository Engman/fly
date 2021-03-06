
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
  float4 normal 	: NORMAL;
  float2 textCoord 	: TEXCOORD;
};

struct PS_IN
{
  float4 position 		: SV_POSITION;
  float4 positionWorld 	: POSITION;
  float4 normal 		: NORMAL;
  float2 textCoord 		: TEXCOORD;
};

PS_IN FVertexShader( VS_IN input)
{
	PS_IN output;
  
	output.position.w = 1.0f;
	output.positionWorld = mul( input.position, mWorld );
  
	output.position = mul(  output.positionWorld, mView );
	output.position = mul(  output.position, mProj );
  
	output.normal = mul(input.normal, mWorldInvTrans );
	output.textCoord = input.textCoord;

	return output;
}


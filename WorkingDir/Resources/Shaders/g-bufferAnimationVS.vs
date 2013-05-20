
cbuffer CB_CAMERA : register (b0)
{
  float4x4 mWorld;
  float4x4 mView;
  float4x4 mProj;
  float4x4 mWorldInvTrans;  
};
cbuffer weights : register (b1)
{
	float weight0;
	//float weight1;
};

struct VS_IN
{
  float4 position0 	: POSITION0;
  float4 normal0 	: NORMAL0;
  float2 textCoord0 : TEXCOORD0;
  
  float4 position1 	: POSITION1;
  float4 normal1 	: POSITION2;
  float2 textCoord1 : POSITION3;
  
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
	
	float4 position = input.position0; 
	float4 normal = input.normal0;
	
	position += (input.position1 - input.position0) * weight0;
	normal += (input.normal1 - input.normal0) * weight0;
	
  	//position += (input.position2 - input.position0) * weight1;
	//normal += (input.normal2 - input.normal0) * weight1;
	
	
	output.position.w = 1.0f;
	output.positionWorld = mul( position, mWorld );
  
	output.position = mul(  output.positionWorld, mView );
	output.position = mul(  output.position, mProj );
  
	output.normal = mul(normal, mWorldInvTrans );
	output.textCoord = input.textCoord0;

	return output;
}


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
	//output.textCoord = input.textCoord;
	
	
	//output.ScreenPos = output.position; 
	//output.ScreenPos = float4(output.position.r/1200, output.position.g/600, output.position.b/(1/1000), 1);
	return output;


	/*PSIn output = (PSIn)0;

	output.position		= input.position; 
 
   output.position.w = 1.0f;
   float4x4 wv; 
   wv  =mul(mWorld, mView);
   float3 pos = input.position.xyz + float3(wv._41, wv._42,wv._43);
  
  output.position = mul ( float4(pos,1.0f), mProj);
  
  //output.position = mul( input.position, mWorld );
  
  //output.position = mul(  output.position, mView );
  
  //output.position = mul(  output.position, mProj );
  
  float2 UV = 0.5f * (float2(output.position.x, - output.position.y) + 1) - float2(1.0f/800.0f, 1.0f/600.0f);
  
  //Projected co-ords.
   float2 textCoord     = output.position;
   textCoord = float2(output.position.x * 0.5, -output.position.y * 0.5);

  textCoord.x = textCoord.x/80;
  textCoord.y  = textCoord.y/60; 
  textCoord.x += 0.5;
  textCoord.y += 0.5;
  output.TextCoord = input.TextCoord - float2(1/800, 1/600); // textCoord; 
  
  //output.TextCoord = UV;
  //output.TextCoord     = clamp(output.position.xy, 0,1);
  //output.TextCoord = float2(output.TextCoord.x * 0.5, -output.TextCoord.y * 0.5);
  //output.TextCoord.xy += (0.5 * output.TextCoord.w); 
   
   
	return output;*/

}
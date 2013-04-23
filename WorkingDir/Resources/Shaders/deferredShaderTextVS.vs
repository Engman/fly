
cbuffer CB_CAMERA
{
  float4x4 mWorld;
  float4x4 mView;
  float4x4 mProj;
  float4x4 mWorldInvTrans;
};


  
struct VS_IN
{
  float4 position : POSITION;
  float4 normal : NORMAL;
  //float4 tangent :TANGENT;
  float2 textCoord : TEXCOORD;
};

struct PS_IN
{
  float4 position : SV_POSITION;
  float3 normal : NORMAL;
  //float4 tangent :TANGENT;
  float2 textCoord : TEXCOORD;
};

PS_IN FVertexShader( VS_IN input)
{
  PS_IN output;


  output.position.w = 1.0f;
  output.position = mul( input.position, mWorld );
  output.position = mul(  output.position, mView );
  output.position = mul(  output.position, mProj );
  
  output.normal = mul(input.normal.xyz, (float3x3)mWorldInvTrans ); //borde gånga med invTrans

  output.textCoord = input.textCoord;

  return output;
}


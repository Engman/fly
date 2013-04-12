
cbuffer CB_CAMERA
{
  float4x4 mWorld;
  float4x4 mView;
  float4x4 mProj;
};

struct VS_IN
{
  float4 position : POSITION;
  float3 normal : NORMAL;
  //float4 tangent :TANGENT;
  float4 color : COLOR;
};

struct PS_IN
{
  float4 position : SV_POSITION;
  float3 normal : NORMAL;
  //float4 tangent :TANGENT;
  float4 color : COLOR;
};

PS_IN FVertexShader( VS_IN input)
{
  PS_IN output;

  //output.position.w = 1.0f;

  //output.position = mul(input.position,mWorld);
  //output.position = mul( output.position ,mView );
  //output.position = mul( output.position ,mProj );
  output.position = input.position; 
  
  output.normal = mul(input.normal, (float3x3)mWorld );
  //output.normal = mul( output.normal ,mView );
  //output.normal = mul( output.normal ,mProj );
  //output.tangent = mul( input.tangent, mWorld );
  output.color = float4(0,0,1,1);// input.color;

  return output;
}

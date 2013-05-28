

//##########
// Structs #
//##############################################################

struct SpotLight
{
	float4 Ambient;
	float4 Diffuse;
	float4 Specular;
	float4 Position;	//Vector4(Vector3 position, float range)
	float4 Direction;	//Vector4(Vector3 direction, float spotRadius)
};

struct PointLight
{
	float4 Ambient;
	float4 Diffuse;
	float4 Specular;
	float4 Position;	//Vcetor4(Vector3 position, float range)
};

struct DirLight
{
	float4 Ambient;
	float4 Diffuse;
	float4 Specular;
	float4 Direction;
};

struct Material
{
	float4 Ambient;
	float4 Diffuse;
	float4 Specular;
	float3 Reflect;
};

//Help struct for Ambient, Diffuse & Specular out data
struct LightOutData
{
	float4 Ambient;
	float4 Diffuse;
	float4 Specular;
};
//##############################################################




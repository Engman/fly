
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
	float4 Attenuate;	//Vector4(Vector3 attenuate, 0.0f)
};

struct PointLight
{
	float4 Ambient;
	float4 Diffuse;
	float4 Specular;
	float4 Position;	//Vcetor4(Vector3 position, float range)
	float4 Attenuate;	//Vector4(Vector3 Attenuate, 0.0f)
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

//###################################
// Functions for light calculations #
//##############################################################


/**void ComputePointLight(Material mat, PointLight light, float3 vertexPos, float3 vertexNormal, float3 toEye, out LightOutData finalLight)
{
	// Initialize outputs.
	finalLight.Ambient		= float4(0.0f, 0.0f, 0.0f, 0.0f);
	finalLight.Diffuse		= float4(0.0f, 0.0f, 0.0f, 0.0f);
	finalLight.Specular     = float4(0.0f, 0.0f, 0.0f, 0.0f);

	// Vector from surface to light.
	float3 lightVec = light.Position - vertexPos;

	// Length of lightVector.
	float d = length(lightVec);

	// In range?
	if( d > light.Position.w )
		return;

	// Ambient term.
	finalLight.Ambient = light.Ambient * mat.Ambient;

	// Normalize the light vector
	lightVec /= d; 

	vertexNormal = normalize(vertexNormal);

	// Add diffuse and specular term, provided the surface is in 
	// the line of site of the light.
	float diffuseFactor = dot(lightVec, vertexNormal);

	[flatten]
	if( diffuseFactor > 0.0f )
	{
		finalLight.Diffuse		= diffuseFactor	* mat.Diffuse	* light.Diffuse;

		float3 eye = normalize(float3(toEye.x, toEye.y, toEye.z) - vertexPos);
		float3 v         = reflect(-lightVec, vertexNormal);
		float specFactor = pow(max(dot(v, toEye), 0.0f), mat.Specular.w);
					
		if(specFactor > 0.0f)
		{
			finalLight.Specular		= specFactor	* mat.Specular	* light.Specular;
			
		}

		// Attenuate
		float att = light.Position.w / dot(light.Attenuate, float4(1.0f, d, d*d, 1.0f));
		finalLight.Diffuse	*= att;
		finalLight.Specular	*= att;
	}
}*/

/** 
Material is the surface material 
DirLight is the struct for the directional ligt
Surface normal is what it is, used for reflection
eye is the cam position and is used for specular
*/
/**void ComputeDirLight(Material m, DirLight l, float3 surfaceNormal, float3 eye, out LightOutData finalLight)
{
	//Init Data
	finalLight.Ambient  = float4(0.0f, 0.0f, 0.0f, 0.0f);
	finalLight.Diffuse  = float4(0.0f, 0.0f, 0.0f, 0.0f);
	finalLight.Specular = float4(0.0f, 0.0f, 0.0f, 0.0f);

	//Apply ambient light
	finalLight.Ambient = l.Ambient * m.Ambient;

	//Light relative to surface
	float3 opDir = -l.Direction;
	float diffFactor = dot(opDir, surfaceNormal);
	
	[flatten]
	if(diffFactor > 0.0f)
	{
		finalLight.Diffuse = diffFactor * m.Diffuse * l.Diffuse;

		//float3 v         = reflect(l.Direction, surfaceNormal);
		//float specFactor = pow(max(dot(v, eye), 0.0f), m.Specular.w);
		//
		//finalLight.Specular = specFactor * l.Specular * m.Specular;
	}
}*/
//##############################################################

void ComputePointLight( PointLight light, float3 vertexPos, float3 vertexNormal, float3 toEye, out LightOutData finalLight)
{
	// Initialize outputs.
	finalLight.Ambient		= float4(0.0f, 0.0f, 0.0f, 0.0f);
	finalLight.Diffuse		= float4(0.0f, 0.0f, 0.0f, 0.0f);
	finalLight.Specular     = float4(0.0f, 0.0f, 0.0f, 0.0f);

	// Vector from surface to light.
	float3 lightVec = light.Position - vertexPos;

	// Length of lightVector.
	float d = length(lightVec);

	// In range?
	if( d > light.Position.w )
		return;

	// Ambient term.
	finalLight.Ambient = light.Ambient;

	// Normalize the light vector
	lightVec /= d; 

	vertexNormal = normalize(vertexNormal);

	// Add diffuse and specular term, provided the surface is in 
	// the line of site of the light.
	float diffuseFactor = dot(lightVec, vertexNormal);

	[flatten]
	if( diffuseFactor > 0.0f )
	{
		finalLight.Diffuse		= diffuseFactor		* light.Diffuse;

		float3 eye = normalize(float3(toEye.x, toEye.y, toEye.z) - vertexPos);
		float3 v         = reflect(-lightVec, vertexNormal);
		/**float specFactor = pow(max(dot(v, toEye), 0.0f), mat.Specular.w);
					
		if(specFactor > 0.0f)
		{
			finalLight.Specular		= specFactor	* mat.Specular	* light.Specular;
			
		}*/

		// Attenuate
		float att = light.Position.w / dot(light.Attenuate, float4(1.0f, d, d*d, 1.0f));
		finalLight.Diffuse	*= att;
		finalLight.Specular	*= att;
	}
}

void ComputeDirLight(DirLight l, float3 surfaceNormal, float3 eye, out LightOutData finalLight)
{
	//Init Data
	finalLight.Ambient  = float4(0.0f, 0.0f, 0.0f, 0.0f);
	finalLight.Diffuse  = float4(0.0f, 0.0f, 0.0f, 0.0f);
	finalLight.Specular = float4(0.0f, 0.0f, 0.0f, 0.0f);

	//Apply ambient light
	finalLight.Ambient = l.Ambient ;

	//Light relative to surface
	float3 opDir = -l.Direction;
	float diffFactor = dot(opDir, surfaceNormal);
	
	[flatten]
	if(diffFactor > 0.0f)
	{
		finalLight.Diffuse = diffFactor  * l.Diffuse;

		//float3 v         = reflect(l.Direction, surfaceNormal);
		//float specFactor = pow(max(dot(v, eye), 0.0f), m.Specular.w);
		//
		//finalLight.Specular = specFactor * l.Specular * m.Specular;
	}
}
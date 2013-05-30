#ifndef SPHERE_H
#define SPHERE_H

#include <D3DX11\D3D11.h>
#include <D3DX11\D3DX11.h>


#include "..\D3DShell.h"
#include "..\..\Util\misc.h"
#include "..\BaseBuffer.h"
#include "..\IShader.h"
#include "..\..\Util\vertex.h"

#define PI (3.14159265358979323846f)
class Sphere
{
public:
	struct VertexSphere
	{
		VertexSphere()
		{}
		VertexSphere(const D3DXVECTOR3& p, const D3DXVECTOR3& n, const D3DXVECTOR3& t, const D3DXVECTOR2& uv)
			: Position(p), Normal(n), TangentU(t), TexC(uv)
		{}
		VertexSphere( float px, float py, float pz, 
			float nx, float ny, float nz,
			float tx, float ty, float tz,
			float u, float v)
			: Position(px,py,pz), Normal(nx,ny,nz),
			TangentU(tx, ty, tz), TexC(u,v)
		{}

		D3DXVECTOR3 Position;
		D3DXVECTOR3 Normal;
		D3DXVECTOR3 TangentU;
		D3DXVECTOR2 TexC;
	};

	struct MeshData
	{
		std::vector<VertexSphere> Vertices;
		std::vector<UINT> Indices;
	};

private:
	BaseBuffer* vertexBuffer;
	BaseBuffer* indexBuffer;
	int mIndexCount; 
	MeshData sphereObj; 
	void InitBuffers(ID3D11Device *device);
	IShader*				m_shader;
	D3DXMATRIX world;
	float radie; 

public:
	Sphere();
	~Sphere();
	void Initiate(float radius, UINT sliceCount, UINT stackCount, ID3D11Device *device, IShader* shader, D3DXMATRIX translate);
	void CreateSphere(float radius, UINT sliceCount, UINT stackCount, MeshData& meshData);
	Sphere::MeshData GetSphere();
	void Render(BaseBuffer* light, D3DXMATRIX world);
	float getRadie();
};
#endif



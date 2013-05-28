#include "FlyWaterMesh.h"
#include "..\..\Util\CollisionLib.h"

FlyWaterMesh::FlyWaterMesh()
	:Entity(Type::OBJECT)
{
	D3DXMatrixIdentity(&this->world);
	D3DXMatrixIdentity(&this->transformation);
	this->rotation	= vec3(0.0f, 0.0f, 0.0f);
	D3DXMatrixIdentity(&this->world);
}
FlyWaterMesh::~FlyWaterMesh()
{

}

void FlyWaterMesh::Update()
{
	this->boundingSphere->center = this->getPosition();
}
void FlyWaterMesh::Render(ViewFrustum& frustum)
{
	if(this->boundingSphere.IsValid())
	{
		if(this->shader && FrustumVSSphere(frustum, *this->boundingSphere))
		{
			D3DXMATRIX scale, rotation, translation;
			D3DXMatrixScaling(&scale, this->scale.x, this->scale.y, this->scale.z);
			D3DXMatrixRotationYawPitchRoll(&rotation, this->rotation.y, this->rotation.x, this->rotation.z);
			D3DXMatrixTranslation(&translation, this->translation.x, this->translation.y, this->translation.z);

			D3DXMatrixIdentity(&this->world);

			this->world *= scale;
			this->world *= rotation;
			this->world *= translation;

			IShader::DRAW_DATA data;
		
			for(int i = 0; i<(int)this->buffers.size(); i++)
				data.buffers.push_back(this->buffers[i]);
			
			
			data.worldMatrix = &this->world;
			data.material = this->material;
			this->shader->addDrawData(data);
		}
	}
	else
	{
		D3DXMATRIX scale, rotation, translation;
		D3DXMatrixScaling(&scale, this->scale.x, this->scale.y, this->scale.z);
		D3DXMatrixRotationYawPitchRoll(&rotation, this->rotation.y, this->rotation.x, this->rotation.z);
		D3DXMatrixTranslation(&translation, this->translation.x, this->translation.y, this->translation.z);

		D3DXMatrixIdentity(&this->world);

		this->world *= scale;
		this->world *= rotation;
		this->world *= translation;

		IShader::DRAW_DATA data;

		for(int i = 0; i<(int)this->buffers.size(); i++)
			data.buffers.push_back(this->buffers[i]);


		data.worldMatrix = &this->world;
		data.material = this->material;
		this->shader->addDrawData(data);
		
	}
}

bool FlyWaterMesh::Initialize(OBJECT_DESC& data)
{
	if(!data.device)
	{
		DisplayText("ID3D11Device is invalid!");
		return false;
	}

	if(!data.vertecies->size())
	{
		DisplayText("Nothing to initialize!");
		return false;
	}
	if(!data.vCount)
	{
		DisplayText("Vertex count invalid!");
		return false;
	}
	
	vec3 vertex;

	this->vertexList = new vector<vec3>;

	for(unsigned int i = 0; i < data.vertecies->size(); i++)
	{
		vertex.x = data.vertecies->at(i).position.x;
		vertex.y = data.vertecies->at(i).position.y;
		vertex.z = data.vertecies->at(i).position.z;

		this->vertexList->push_back(vertex);
	}

	this->material = MaterialHandler::GetMaterial(data.material_id);
	if(!this->material)
		DisplayText("A material could not be found", "Warning!");

	int vertexNr = 0;
	SmartPtrStd<vector<VERTEX::VertexPNTBT>> vertexBump;
	vertexBump = new vector<VERTEX::VertexPNTBT>;
	(*vertexBump).resize((int)data.vCount);

	D3DXVECTOR4 tangent, binormal; 
	while(vertexNr<(int)data.vCount)
	{
		(*vertexBump)[vertexNr].position	= data.vertecies->at(vertexNr).position;
		(*vertexBump)[vertexNr+1].position	= data.vertecies->at(vertexNr+1).position;
		(*vertexBump)[vertexNr+2].position	= data.vertecies->at(vertexNr+2).position;

		(*vertexBump)[vertexNr].normal		= data.vertecies->at(vertexNr).normal;
		(*vertexBump)[vertexNr+1].normal	= data.vertecies->at(vertexNr+1).normal;
		(*vertexBump)[vertexNr+2].normal	= data.vertecies->at(vertexNr+2).normal;

		(*vertexBump)[vertexNr].texcoord	= data.vertecies->at(vertexNr).texcoord;
		(*vertexBump)[vertexNr+1].texcoord	= data.vertecies->at(vertexNr+1).texcoord;
		(*vertexBump)[vertexNr+2].texcoord	= data.vertecies->at(vertexNr+2).texcoord;

		CalculateTangentBinormal((*data.vertecies)[vertexNr], (*data.vertecies)[vertexNr+1], (*data.vertecies)[vertexNr+2], tangent, binormal);

		(*vertexBump)[vertexNr].tangent		= tangent;
		(*vertexBump)[vertexNr+1].tangent	= tangent;
		(*vertexBump)[vertexNr+2].tangent	= tangent;

		(*vertexBump)[vertexNr].binormal	= binormal;
		(*vertexBump)[vertexNr+1].binormal	= binormal;
		(*vertexBump)[vertexNr+2].binormal	= binormal;
		
		vertexNr +=3; 
	}

	SmartPtrStd<BaseBuffer> b = new BaseBuffer();
	BaseBuffer::BUFFER_INIT_DESC desc;
	desc.device			= data.device;
	desc.dc				= data.deviceContext;
	desc.data			= &(*vertexBump)[0];
	desc.elementSize	= sizeof(VERTEX::VertexPNTBT);
	desc.nrOfElements	= (int)data.vCount;
	desc.type			= BUFFER_FLAG::TYPE_VERTEX_BUFFER;
	desc.usage			= BUFFER_FLAG::USAGE_DEFAULT;
	if( FAILED( b->Initialize(desc) ) )
	{
		DisplayText("Failed to initiate Buffer in FlyBumpMesh");
		return false;
	}
	this->buffers.push_back(b);
	this->shader			= data.shader;
	this->name				= data.name;
	this->boundingSphere	= data.boundingSphere;


	return true;
}

vector<vec3>* FlyWaterMesh::GetTriangles()
{
	return this->vertexList;
}
void FlyWaterMesh::CalculateTangentBinormal(VERTEX::VertexPNT vertex1, VERTEX::VertexPNT vertex2, VERTEX::VertexPNT vertex3,
	D3DXVECTOR4& tangent, D3DXVECTOR4& binormal)
{
	float den;
	D3DXVECTOR3 vector1, vector2;
	D3DXVECTOR2 Uvector, Vvector;

	D3DXVECTOR3 pos1, pos2, pos3; 
	pos1 = D3DXVECTOR3(vertex1.position.x,vertex1.position.y, vertex1.position.z) ;
	pos2 = D3DXVECTOR3(vertex2.position.x,vertex2.position.y, vertex2.position.z) ;
	pos3 = D3DXVECTOR3(vertex3.position.x,vertex3.position.y, vertex3.position.z) ;

	// Calculate the two vectors for this face.
	vector1 = pos2 -pos1;
	vector2 = pos3 -pos1;

	// Calculate the tu and tv texture space vectors.

	Uvector.x = vertex2.texcoord.x - vertex1.texcoord.x;
	Uvector.y = vertex3.texcoord.y - vertex1.texcoord.y;

	Vvector.x = vertex2.texcoord.y - vertex1.texcoord.y;
	Vvector.y = vertex3.texcoord.x - vertex1.texcoord.x;

	// Calculate the denominator of the tangent/binormal equation.
	den = 1.0f / (Uvector.x * Vvector.y - Uvector.y * Vvector.x);

	// Calculate the cross products and multiply by the coefficient to get the tangent and binormal.
	tangent = D3DXVECTOR4(Vvector.y *vector1 - Vvector.y * vector2 *den, 0);
	binormal = D3DXVECTOR4(Uvector.x* vector2 -Uvector.y * vector1 *den, 0);
	D3DXVec4Normalize(&tangent, &tangent);
	D3DXVec4Normalize(&binormal, &binormal);

	return;
}
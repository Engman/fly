#include "ParticleMesh.h"
#include "..\..\Util\CollisionLib.h"

ParticleMesh::ParticleMesh()
	:Entity(Type::OBJECT)
{
	D3DXMatrixIdentity(&this->world);
	D3DXMatrixIdentity(&this->transformation);
	this->rotation	= vec3(0.0f, 0.0f, 0.0f);
	D3DXMatrixIdentity(&this->world);
}
ParticleMesh::~ParticleMesh()
{

}

void ParticleMesh::Update()
{
	this->boundingSphere->center = this->getPosition();
}

void ParticleMesh::Render(ViewFrustum& frustum)
{
	if(this->boundingSphere.IsValid())
	{
		if(this->shader && FrustumVSSphere(frustum, *this->boundingSphere))
		{
			D3DXMATRIX rotation, translation;
			D3DXMatrixRotationYawPitchRoll(&rotation, this->rotation.y, this->rotation.x, this->rotation.z);
			D3DXMatrixTranslation(&translation, this->translation.x, this->translation.y, this->translation.z);

			D3DXMatrixIdentity(&this->world);

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
		D3DXMATRIX rotation, translation;
		D3DXMatrixRotationYawPitchRoll(&rotation, this->rotation.y, this->rotation.x, this->rotation.z);
		D3DXMatrixTranslation(&translation, this->translation.x, this->translation.y, this->translation.z);

		D3DXMatrixIdentity(&this->world);

		this->world *= rotation;
		this->world *= translation;

		IShader::DRAW_DATA data;
		
		for(int i = 0; i<(int)this->buffers.size(); i++)
		{
			data.buffers.push_back(this->buffers[i]);
		}
		
		data.worldMatrix = &this->world;
		data.material = this->material;
		this->shader->addDrawData(data);
		
	}
}

bool ParticleMesh::Initialize(OBJECT_DESC& data)
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

	SmartPtrStd<BaseBuffer> b = new BaseBuffer();
	BaseBuffer::BUFFER_INIT_DESC desc;
	desc.device			= data.device;
	desc.dc				= data.deviceContext;
	desc.data			= &(*data.vertecies)[0];
	desc.elementSize	= sizeof(VERTEX::VertexPNT);
	desc.nrOfElements	= (int)data.vCount;
	desc.type			= BUFFER_FLAG::TYPE_VERTEX_BUFFER;
	desc.usage			= BUFFER_FLAG::USAGE_DYNAMIC_CPU_WRITE_DISCARD;
	if( FAILED( b->Initialize(desc) ) )
	{
		DisplayText("Failed to initiate Buffer in FlyMesh");
		return false;
	}
	this->buffers.push_back(b);
	this->shader			= data.shader;
	this->name				= data.name;
	this->boundingSphere	= data.boundingSphere;


	return true;
}

vector<vec3>* ParticleMesh::GetTriangles()
{
	return this->vertexList;
}

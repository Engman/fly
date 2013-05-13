#include "FlyMesh.h"
#include "..\..\Util\CollisionLib.h"

FlyMesh::FlyMesh()
	:Entity(Type::OBJECT)
{
	D3DXMatrixIdentity(&this->world);
	D3DXMatrixIdentity(&this->transformation);
	this->rotation	= vec3(0.0f, 0.0f, 0.0f);
	D3DXMatrixIdentity(&this->world);
}
FlyMesh::~FlyMesh()
{}

void FlyMesh::Update()
{
	this->boundingSphere->center = this->getPosition();
}
void FlyMesh::Render(ViewFrustum& frustum)
{
	if(this->boundingSphere.IsValid())
	{
		if(this->shader && FrustumVSSphere(frustum, *this->boundingSphere))
		{
			IShader::DRAW_DATA data;
		
			for(int i = 0; i<(int)this->buffers.size(); i++)
				data.buffers.push_back(this->buffers[i]);
			data.worldMatrix = &this->transformation;


			//data.worldMatrix = &this->world;
			data.material = this->material;
			this->shader->addDrawData(data);
		}
	}
	else
	{
		IShader::DRAW_DATA data;
		for(int i = 0; i<(int)this->buffers.size(); i++)
		{
			data.buffers.push_back(this->buffers[i]);
		}
		//get local world
		//rotate
		//scale
		//translate
		D3DXMATRIX rotX, translation, scaleM;
		D3DXVECTOR3 scale = this->scale; 

		D3DXMatrixIdentity(&this->world);
		D3DXMatrixIdentity(&translation);
		D3DXMatrixIdentity(&scaleM);

		D3DXMatrixRotationZ(&rotX, this->rotation.x);

		D3DXMatrixScaling(&scaleM, scale.x, scale.y, scale.z);
		D3DXVECTOR3 pos; 
		pos = getPosition();
		translation._41 = pos.x;
		translation._42 = pos.y;
		translation._43 = pos.z;

		
		this->world *= rotX;

	
		this->world *= scaleM;
		this->world *= translation;
		
		
		data.worldMatrix = &this->world;
		data.material = this->material;
		this->shader->addDrawData(data);
	}
}

bool FlyMesh::Initialize(OBJECT_DESC& data)
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
	desc.usage			= BUFFER_FLAG::USAGE_DEFAULT;
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


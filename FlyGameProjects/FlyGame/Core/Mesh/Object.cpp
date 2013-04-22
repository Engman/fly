#include "Object.h"
//#include "..\..\Util\misc.h"

Object::Object()
	:Entity(Type::OBJECT)
{
	this->position	= vec3(0.0f, 0.0f, 0.0f);
	this->rotation	= vec3(0.0f, 0.0f, 0.0f);
	this->front		= vec3(0.0f, 0.0f, 0.0f);
	this->right		= vec3(0.0f, 0.0f, 0.0f);
	this->up		= vec3(0.0f, 0.0f, 0.0f);
	D3DXMatrixIdentity(&this->world);
}
Object::~Object()
{}

void Object::Update()
{

}
void Object::Render()
{
	if(this->shader)
	{
		IShader::DRAW_DATA data;

		for(int i = 0; i<(int)this->buffers.size(); i++)
			data.buffers.push_back(this->buffers[0]);
		data.worldMatrix = &this->world;
		data.material = this->material;
		data.textures = NULL;
		this->shader->addDrawData(data);
	}
}

bool Object::Initialize(OBJECT_DESC& data)
{
	if(!data.device)
	{
		DisplayText("ID3D11Device is invalid!");
		return false;
	}
	if(!data.vertecies)
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
	desc.data			= data.vertecies;
	desc.elementSize	= sizeof(VERTEX::VertexPNT);
	desc.nrOfElements	= (int)data.vCount;
	desc.type			= BUFFER_FLAG::TYPE_VERTEX_BUFFER;
	desc.usage			= BUFFER_FLAG::USAGE_DEFAULT;
	if( FAILED( b->Initialize(desc) ) )
	{
		DisplayText("Failed to initiate Buffer in Object");
		return false;
	}
	this->buffers.push_back(b);
	this->shader	= data.shader;

	return true;
}


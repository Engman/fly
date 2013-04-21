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
}
Object::~Object()
{}

void Object::Update()
{

}
void Object::Render()
{
	
}

bool Object::Initialize(OBJECT_DESC& data)
{
	if(!data.device)
	{
		DisplayText("ID3D11Device is invalid!");
		return false;
	}
	if(!data.vertecies.size())
	{
		DisplayText("No vertecies in list!");
		return false;
	}
	
	this->material = MaterialHandler::GetMaterial(data.material_id);
	if(!this->material)
		DisplayText("A material could not be found", "Warning!");

	SmartPtrStd<BaseBuffer> b = new BaseBuffer();
	BaseBuffer::BUFFER_INIT_DESC desc;
	desc.device = data.device;
	desc.dc		= data.deviceContext;
	desc.data	= &data.vertecies[0];
	desc.elementSize = sizeof(VERTEX::VertexPNT);
	desc.nrOfElements = (int)data.vertecies.size();
	desc.type			= BUFFER_FLAG::TYPE_VERTEX_BUFFER;
	desc.usage			= BUFFER_FLAG::USAGE_DEFAULT;
	if(!b->Initialize(desc))
	{
		DisplayText("Failed to initiate Buffer in Object");
		return false;
	}
	this->buffers.push_back(b);

	this->position	= data.position;
	this->rotation	= data.rotation;
	this->front		= data.front;
	this->right		= data.right;
	this->up		= data.up;
	this->shader	= data.shader;

	return true;
}


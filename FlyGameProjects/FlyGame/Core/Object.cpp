#include "Object.h"

Object::Object()
	:Entity(Type::I_DRAWABLE_OBJECT, 2)
{

}
Object::~Object()
{

}

void Object::Update()
{

}
void Object::Render()
{
	
}

bool Object::Initialize(/*Model* modelRawData*/)
{
	return true;
}
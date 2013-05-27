#include "FlyPickup.h"

FlyPickup::FlyPickup()
{

}

FlyPickup::~FlyPickup()
{

}

bool FlyPickup::Initialize(FlyGame* entry, wstring modelName, vec3 position, vec3 rotation, int shader)
{
	this->taken = false;

	vector<IShader*> shaders;
	entry->GetCoreInstance()->Gfx_GetShader(shaders);

	BoundingSphere* sphere = new BoundingSphere();
	sphere->center = position;
	sphere->radius = 2.0f;
	
	entry->GetCoreInstance()->Geometry_Load(modelName.c_str(), &this->pickupModel);
	this->pickupModel[0]->setScale(vec3(1.0f, 1.0f, 1.0f));
	this->pickupModel[0]->setPosition(vec3(position));
	this->pickupModel[0]->setRotation(vec3(rotation));
	this->pickupModel[0]->setShader(shaders[shader]);
	this->pickupModel[0]->setBoundingSphere(sphere);

	return true;
}
void FlyPickup::Render(ViewFrustum& f)
{
	this->pickupModel[0]->Render(f);
}

void FlyPickup::Update()
{
	
}

void FlyPickup::SetPickTaken()
{
	this->taken = true;
}

void FlyPickup::SetPosition(vec3 position)
{
	return this->pickupModel[0]->setPosition(position);
}
void FlyPickup::SetRotation(vec3 rotation)
{
	return this->pickupModel[0]->setRotation(rotation);
}
void FlyPickup::SetShader(IShader* shader)
{
	this->pickupModel[0]->setShader(shader);
}

vec3 FlyPickup::GetPosition() const
{
	return this->pickupModel[0]->getPosition();
}
vec3 FlyPickup::GetRotation() const
{
	return this->pickupModel[0]->getRotation();
}
bool FlyPickup::GetTaken() const
{
	return this->taken;
}

BoundingSphere* FlyPickup::GetBoundingSphere()
{
	return this->pickupModel[0]->getBoundingSphere();
}
void FlyPickup::SetBoundingSphere(BoundingSphere* sphere)
{
	this->pickupModel[0]->setBoundingSphere(sphere);
}

void FlyPickup::Release()
{
	for(unsigned int i = 0; i < this->pickupModel.size(); i++)
	{
		delete this->pickupModel[i];
	}
}
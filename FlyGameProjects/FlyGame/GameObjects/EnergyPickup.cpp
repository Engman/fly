#include "EnergyPickup.h"
#include "..\..\FlyGameEngine\Core\Mesh\FlyMeshAnimated.h"

EnergyPickup::EnergyPickup()
{

}

EnergyPickup::~EnergyPickup()
{

}

bool EnergyPickup::Initialize(FlyGame* entry, wstring modelName, vec3 position, vec3 rotation, vec3 scale, int shader)
{
	this->taken = false;

	vector<IShader*> shaders;
	entry->GetCoreInstance()->Gfx_GetShader(shaders);

	BoundingSphere* sphere = new BoundingSphere();
	sphere->center = position;
	sphere->radius = 2.0f;
	
	if(!entry->GetCoreInstance()->Geometry_Load(modelName.c_str(), &this->pickupModel, FlyGeometry_AnimatedMesh, 1, 3))
		return false;
	this->pickupModel[0]->setPosition(position);
	this->pickupModel[0]->setRotation(rotation);
	this->pickupModel[0]->setScale(scale);
	this->pickupModel[0]->setShader(shaders[shader]);
	this->pickupModel[0]->setBoundingSphere(sphere);

	return true;
}
void EnergyPickup::Render(ViewFrustum& f)
{
	this->pickupModel[0]->Render(f);
}

void EnergyPickup::Update(float dt)
{
	if(this->pickupModel.size() > 0)
		((FlyMeshAnimated*)this->pickupModel[0])->LoopAnimation(0, dt); 
}

void EnergyPickup::SetPickTaken(bool taken)
{
	this->taken = taken;
}

void EnergyPickup::SetPosition(vec3 position)
{
	return this->pickupModel[0]->setPosition(position);
}
void EnergyPickup::SetRotation(vec3 rotation)
{
	return this->pickupModel[0]->setRotation(rotation);
}
void EnergyPickup::SetScale(vec3 scale)
{
	this->pickupModel[0]->setScale(scale);
}

void EnergyPickup::SetShader(IShader* shader)
{
	this->pickupModel[0]->setShader(shader);
}

vec3 EnergyPickup::GetPosition() const
{
	return this->pickupModel[0]->getPosition();
}
vec3 EnergyPickup::GetRotation() const
{
	return this->pickupModel[0]->getRotation();
}
vec3 EnergyPickup::GetScale() const
{
	return this->pickupModel[0]->getScale();
}
bool EnergyPickup::GetTaken() const
{
	return this->taken;
}

BoundingSphere* EnergyPickup::GetBoundingSphere()
{
	return this->pickupModel[0]->getBoundingSphere();
}
void EnergyPickup::SetBoundingSphere(BoundingSphere* sphere)
{
	this->pickupModel[0]->setBoundingSphere(sphere);
}

void EnergyPickup::Release()
{
	for(unsigned int i = 0; i < this->pickupModel.size(); i++)
	{
		delete this->pickupModel[i];
	}
}
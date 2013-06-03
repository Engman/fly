#include "FlyPlayer.h"

FlyPlayer::FlyPlayer()
{
	this->velocity = vec3(0.0f, 0.0f, 0.0f);
	this->maxVelocity = vec3(0.4f, 0.4f, 0.8f);
	this->maxEnergy = 10000;
	this->energy = maxEnergy;
	this->closeCargo = false; 
}

FlyPlayer::~FlyPlayer()
{
	
}

void FlyPlayer::Initialize()
{
	
}

void FlyPlayer::Render(ViewFrustum& frustum)
{
	this->playerModel[0]->Render(frustum);
}

void FlyPlayer::Update()
{
	vector<D3DXVECTOR3>* vertexList = ((FlyMeshAnimated*)(this->playerModel[0]))->GetTriangles();
	BoundingBox box;
	box.minPoint = vec3(0.0f, 0.0f, 0.0f);
	box.maxPoint = vec3(0.0f, 0.0f, 0.0f);

	for(unsigned long i = 0; i < vertexList->size(); i++)
	{
		D3DXMATRIX world, rotation, translation;
		D3DXVECTOR4 vertex;
		D3DXMatrixIdentity(&world);
		D3DXMatrixRotationYawPitchRoll(&rotation, this->playerModel[0]->getRotation().y, this->playerModel[0]->getRotation().x, this->playerModel[0]->getRotation().z);

		world *= rotation;

		D3DXVec3Transform(&vertex, &vertexList->at(i), &world);

		if(vertex.x < box.minPoint.x)
		{
			box.minPoint.x = vertex.x;
		}
		if(vertex.y < box.minPoint.y)
		{
			box.minPoint.y = vertex.y;
		}
		if(vertex.z < box.minPoint.z)
		{
			box.minPoint.z = vertex.z;
		}

		if(vertex.x > box.maxPoint.x)
		{
			box.maxPoint.x = vertex.x;
		}
		if(vertex.y > box.maxPoint.y)
		{
			box.maxPoint.y = vertex.y;
		}
		if(vertex.z > box.maxPoint.z)
		{
			box.maxPoint.z = vertex.z;
		}
	}

	this->boundingEllipse.radiusVector = (box.maxPoint - box.minPoint)*0.5f+vec3(0.1f, 0.0f, 0.1f);
	this->boundingEllipse.center = this->GetPosition();

	this->boundingEllipse.center = this->GetPosition();
}

vector<Entity*>* FlyPlayer::GetModel()
{
	return &this->playerModel;
}

void FlyPlayer::SetPosition(vec3 position)
{
	this->playerModel[0]->setPosition(position);
}

void FlyPlayer::SetRotation(vec3 rotation)
{
	this->playerModel[0]->setRotation(rotation);
}

void FlyPlayer::SetScale(vec3 scale)
{
	this->playerModel[0]->setScale(scale);
}

void FlyPlayer::SetVelocity(vec3 velocity)
{
	this->velocity = velocity;
}

void FlyPlayer::SetSmall(bool changed)
{
	this->smallModel = changed;
}

void FlyPlayer::SetEllipseVector(vec3 radius)
{
	this->boundingEllipse.radiusVector = radius;
}

vec3 FlyPlayer::GetPosition() const
{
	return this->playerModel[0]->getPosition();
}

vec3 FlyPlayer::GetRotation() const
{
	return this->playerModel[0]->getRotation();
}

vec3 FlyPlayer::GetVelocity() const
{
	return this->velocity;
}

vec3 FlyPlayer::GetMaxVelocity() const
{
	return this->maxVelocity;
}

bool FlyPlayer::GetSmall() const
{
	return this->smallModel;
}

float FlyPlayer::GetEnergy() const
{
	return this->energy;
}

BoundingEllipse FlyPlayer::GetEllipse() const
{
	return this->boundingEllipse;
}

BoundingSphere* FlyPlayer::GetBoundingSphere()
{
	return this->playerModel[0]->getBoundingSphere();
}

float FlyPlayer::GetMaxEnergy() const
{
	return this->maxEnergy;
}

void FlyPlayer::SetBoundingSphere(BoundingSphere* sphere)
{
	this->playerModel[0]->setBoundingSphere(sphere);
}

void FlyPlayer::UpdateAnimation(int nr, float dt)
{
	((FlyMeshAnimated*)this->playerModel[0])->StartAnimation(nr, dt); 
}

void FlyPlayer::StopAnimation(int nr, float dt)
{
	((FlyMeshAnimated*)this->playerModel[0])->StopAnimation(nr, dt); 
}

void FlyPlayer::setCloseCargo(bool close)
{
	this->closeCargo = close; 
}

bool FlyPlayer::getCloseCargo() const
{
	return this->closeCargo; 
}

void FlyPlayer::Release()
{
	for(unsigned int i = 0; i < this->playerModel.size(); i++)
	{
		delete this->playerModel[i];
	}
}

void FlyPlayer::DeductEnergy(float howMuch)
{
	this->energy -= howMuch;

	if(this->energy > this->maxEnergy)
		this->energy = this->maxEnergy;
}

//Controlls
void FlyPlayer::RollLeft()
{
	this->SetRotation(vec3(this->GetRotation().x, this->GetRotation().y, this->GetRotation().z));
}
void FlyPlayer::RollRight(vec3 forward)
{
	this->SetRotation(vec3(this->GetRotation().x, this->GetRotation().y, this->GetRotation().z));
}
void FlyPlayer::BankLeft()
{

}
void FlyPlayer::BankRight()
{

}
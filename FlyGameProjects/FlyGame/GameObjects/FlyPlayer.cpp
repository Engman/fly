#include "FlyPlayer.h"

FlyPlayer::FlyPlayer()
{
	this->velocity = vec3(0.0f, 0.0f, 0.0f);
	this->maxVelocity = vec3(0.2f, 0.2f, 0.2f);
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

void FlyPlayer::SetVelocity(vec3 velocity)
{
	this->velocity = velocity;
}

void FlyPlayer::SetJustChanged(bool changed)
{
	this->justChangedForm = changed;
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

bool FlyPlayer::GetJustChanged() const
{
	return this->justChangedForm;
}

BoundingSphere* FlyPlayer::GetBoundingSphere()
{
	return this->playerModel[0]->getBoundingSphere();
}

void FlyPlayer::SetBoundingSphere(BoundingSphere* sphere)
{
	this->playerModel[0]->setBoundingSphere(sphere);
}

void FlyPlayer::UpdateAnimation(int nr)
{
	((FlyMeshAnimated*)this->playerModel[0])->UpdateAnimation(nr); 
}

void FlyPlayer::StopAnimation(int nr)
{
	((FlyMeshAnimated*)this->playerModel[0])->StopAnimation(nr); 
}


void FlyPlayer::Release()
{
	for(unsigned int i = 0; i < this->playerModel.size(); i++)
	{
		delete this->playerModel[i];
	}
}
#include "FlyState_Level.h"


void FlyState_Level::_Input()
{
	Input::self()->Frame();

	switch (this->controlScheme)
	{
		case CONTROL_Casual:
			_InputCasual();
		break;
		case CONTROL_Advance:
			_InputAdvance();
		break;
		case CONTROL_Debug:
			_InputDebug();
		break;
	}

	if(Input::self()->IsButtonPressed(DIK_ESCAPE))
	{
		//this->state = 1;
		PostQuitMessage(0);
	}
}


void FlyState_Level::_InputCasual()
{

	int mouseX, mouseY;
	Input::self()->GetMouseRelative(mouseX, mouseY);

	//Casual
	if(mouseY < 0 || mouseY > 0)
	{
		this->mainCamera.RelativePitch(0.1f*mouseY);
	}
	if(mouseX < 0)
	{	
		if(this->player.GetVelocity().x > -this->player.GetMaxVelocity().x)
			this->player.SetVelocity(this->player.GetVelocity() + vec3(mouseX*0.001f, 0.0f, 0.0f));		
	}
	if(mouseX > 0)
	{
		if(this->player.GetVelocity().x < this->player.GetMaxVelocity().x)
			this->player.SetVelocity(this->player.GetVelocity() + vec3(mouseX*0.001f, 0.0f, 0.0f));
	}
	if(mouseX == 0)
	{
		if(this->player.GetVelocity().x <= -0.008f)
			this->player.SetVelocity(this->player.GetVelocity() + vec3(0.008f, 0.0f, 0.0f));
		else if(this->player.GetVelocity().x >= 0.008f)
			this->player.SetVelocity(this->player.GetVelocity() + vec3(-0.008f, 0.0f, 0.0f));
	}

	if(Input::self()->IsButtonPressed(DIK_W))
	{
		if(this->player.GetVelocity().z < this->player.GetMaxVelocity().z)
			this->player.SetVelocity(this->player.GetVelocity() + vec3(0.0f, 0.0f, 0.008f));
	}
	else if(Input::self()->IsButtonPressed(DIK_S))
	{
		if(this->player.GetVelocity().z > -this->player.GetMaxVelocity().z)
			this->player.SetVelocity(this->player.GetVelocity() + vec3(0.0f, 0.0f, -0.008f));
	}
	else
	{
		if(this->player.GetVelocity().z <= -0.004f)
			this->player.SetVelocity(this->player.GetVelocity() + vec3(0.0f, 0.0f, 0.004f));
		else if(this->player.GetVelocity().z >= 0.004f)
			this->player.SetVelocity(this->player.GetVelocity() - vec3(0.0f, 0.0f, 0.004f));
	}

	if(Input::self()->IsButtonPressed(DIK_LSHIFT))
	{
		this->player.SetEllipseVector(vec3(0.5f, 0.5f, 0.5f));
	}
	else if(this->player.GetEllipse().radiusVector.x == 0.5f)
	{
		this->player.SetEllipseVector(vec3(2.0f, 0.5f, 2.0f));
		this->player.SetSmall(true);
	}
}
void FlyState_Level::_InputAdvance()
{
	int mouseX, mouseY;
	Input::self()->GetMouseRelative(mouseX, mouseY);

	//Advanced
	if(Input::self()->IsButtonPressed(DIK_W))
	{
		if(this->player.GetVelocity().y < this->player.GetMaxVelocity().y)
			this->player.SetVelocity(this->player.GetVelocity() + vec3(0.0f, 0.004f, 0.0f));
	}
	else
	{
		if(this->player.GetVelocity().y > 0.0f)
			this->player.SetVelocity(this->player.GetVelocity() - vec3(0.0f, 0.002f, 0.0f));
	}
	if(Input::self()->IsButtonPressed(DIK_S))
	{
		if(this->player.GetVelocity().y > -this->player.GetMaxVelocity().y)
			this->player.SetVelocity(this->player.GetVelocity() - vec3(0.0f, 0.004f, 0.0f));
	}
	else
	{
		if(this->player.GetVelocity().y < 0.0f)
			this->player.SetVelocity(this->player.GetVelocity() + vec3(0.0f, 0.002f, 0.0f));
	}
	
	if(Input::self()->IsButtonPressed(DIK_LSHIFT))
	{
		this->player.SetEllipseVector(vec3(0.5f, 0.5f, 0.5f));
	}
	else if(this->player.GetEllipse().radiusVector.x == 0.5f)
	{
		this->player.SetEllipseVector(vec3(2.0f, 0.5f, 2.0f));
		this->player.SetSmall(true);
	}
	if(Input::self()->IsButtonPressed(DIK_A))
	{
		if(this->player.GetVelocity().x > -this->player.GetMaxVelocity().x)
			this->player.SetVelocity(this->player.GetVelocity() + vec3(-0.002f, 0.0f, 0.0f));
		//this->player.SetRotation(vec3(this->player.GetRotation().x-this->player.GetVelocity().x*0.4f, (this->mainCamera.GetRotation().y-90.0f)*((float)D3DX_PI/180.0f), -this->mainCamera.GetRotation().x*((float)D3DX_PI/180.0f)));
	}
	if(Input::self()->IsMouseButtonPressed(1))
	{
		if(this->player.GetVelocity().z < this->player.GetMaxVelocity().z)
			this->player.SetVelocity(this->player.GetVelocity() + vec3(0.0f, 0.0f, 0.001f));
		if(this->player.GetVelocity().x > -this->player.GetMaxVelocity().x)
			this->player.SetVelocity(this->player.GetVelocity() + vec3(-0.001f, 0.0f, 0.0f));
	}
	else
	{
		if(this->player.GetVelocity().x < -0.001f)
			this->player.SetVelocity(this->player.GetVelocity() + vec3(0.001f, 0.0f, 0.0f));
		if(this->player.GetVelocity().z > 0.0f)
			this->player.SetVelocity(this->player.GetVelocity() - vec3(0.0f, 0.0f, 0.0005f));
		if(this->player.GetVelocity().z < 0.0f)
			this->player.SetVelocity(this->player.GetVelocity() + vec3(0.0f, 0.0f, 0.0005f));
	}
	if(Input::self()->IsButtonPressed(DIK_D))
	{
		if(this->player.GetVelocity().x < this->player.GetMaxVelocity().x)
			this->player.SetVelocity(this->player.GetVelocity() + vec3(0.002f, 0.0f, 0.0f));
		//this->player.SetRotation(vec3(this->player.GetRotation().x-this->player.GetVelocity().x*0.4f, (this->mainCamera.GetRotation().y-90.0f)*((float)D3DX_PI/180.0f), -this->mainCamera.GetRotation().x*((float)D3DX_PI/180.0f)));
	}
	if(Input::self()->IsMouseButtonPressed(0))
	{
		if(this->player.GetVelocity().z < this->player.GetMaxVelocity().z)
			this->player.SetVelocity(this->player.GetVelocity() + vec3(0.0f, 0.0f, 0.001f));
		if(this->player.GetVelocity().x < this->player.GetMaxVelocity().x)
			this->player.SetVelocity(this->player.GetVelocity() + vec3(0.001f, 0.0f, 0.0f));
	}
	else
	{
		if(this->player.GetVelocity().x > 0.001f)
			this->player.SetVelocity(this->player.GetVelocity() + vec3(-0.001f, 0.0f, 0.0f));
		if(this->player.GetVelocity().z > 0.0f)
			this->player.SetVelocity(this->player.GetVelocity() - vec3(0.0f, 0.0f, 0.00005f));
		if(this->player.GetVelocity().z < 0.0f)
			this->player.SetVelocity(this->player.GetVelocity() + vec3(0.0f, 0.0f, 0.00005f));
	}
}
void FlyState_Level::_InputDebug()
{
	int mouseX, mouseY;
	Input::self()->GetMouseRelative(mouseX, mouseY);

	if(Input::self()->IsButtonPressed(DIK_W))
	{
		this->mainCamera.RelativeForward(1.2f);
	}
	if(Input::self()->IsButtonPressed(DIK_S))
	{
		this->mainCamera.RelativeForward(-1.2f);
	}
	if(Input::self()->IsButtonPressed(DIK_A))
	{
		this->mainCamera.RelativeRight(-1.2f);
		//this->player.UpdateAnimation(2);
		//this->entryInstance->GetCoreInstance()->Audio_PlaySound(FlySound_Wings);
	}
	if(Input::self()->IsButtonPressed(DIK_D))
	{
		this->mainCamera.RelativeRight(1.2f);
	}
	if(mouseY < 0)
	{
		this->mainCamera.RelativePitch(0.05f*mouseY);
	}
	if(mouseY > 0)
	{
		this->mainCamera.RelativePitch(0.05f*mouseY);
	}
	if(mouseX < 0)
	{	
		this->mainCamera.RelativeYaw(0.05f*mouseX);
	}
	if(mouseX > 0)
	{
		this->mainCamera.RelativeYaw(0.05f*mouseX);
	}
}
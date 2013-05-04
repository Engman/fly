#include "FlyCamera.h"

FlyCamera::FlyCamera()
{
	this->view		= D3DXMATRIX(0, 0, 0, 0, 
								 0, 0, 0, 0,
								 0, 0, 0, 0,
								 0, 0, 0, 1);

	this->proj		= D3DXMATRIX(0, 0, 0, 0, 
								 0, 0, 0, 0,
								 0, 0, 0, 0,
								 0, 0, 0, 0);

	this->worldUp	= vec3(0.0f, 1.0f, 0.0f);
	this->position	= vec3(0.0f, 0.0f, 0.0f);
	this->right		= vec3(1.0f, 0.0f, 0.0f);
	this->up		= vec3(0.0f, 1.0f, 0.0f);
	this->front		= vec3(0.0f, 0.0f, 1.0f);

	this-> nearZ	= 0.0f;
	this-> farZ		= 0.0f;
	this->fov		= 0.0f;
	this->aspect	= 0.0f;
}
FlyCamera::~FlyCamera()
{

}


void FlyCamera::setWorldUp(vec3 up)
{
	this->worldUp = up;
}
void FlyCamera::setPos(const vec3 pos)
{
	this->position = pos;
}
void FlyCamera::setPos(const float x, const float y, const float z)
{
	this->position.x = x;
	this->position.y = y;
	this->position.z = z;
}
void FlyCamera::setHeight(const float y)
{
	this->position.y = y;
}
void FlyCamera::setPerspective(float fov, float aspect, float zNear, float zFar)
{
	this->fov		= fov;
	this->aspect	= aspect;

	if(zNear == 0.0f)
		zNear = 0.1f;
	this->nearZ		= zNear;

	if(zFar <= zNear)
		zFar = 10.f;
	this->farZ	= zFar;

	D3DXMatrixPerspectiveFovLH(&this->proj, this->fov, this->aspect, this->nearZ, this->farZ);
}
void FlyCamera::setLookAt(vec3 pos, vec3 at, vec3 up)
{
	this->position	= pos;
	this->front		= at - pos;
	D3DXVec3Cross(&this->right, &up, &this->front);
	D3DXVec3Cross(&this->up, &this->front, &this->right);
}

void FlyCamera::modifyFarZByVal(float z)
{
	this->farZ += z;
	D3DXMatrixPerspectiveFovLH(&this->proj, this->fov, this->aspect, this->nearZ, this->farZ);
}
void FlyCamera::modifyNearZByVal(float z)
{
	this->nearZ += z;
	D3DXMatrixPerspectiveFovLH(&this->proj, this->fov, this->aspect, this->nearZ, this->farZ);
}


void FlyCamera::Strafe(float dist)
{
	this->position += this->right * dist;
}
void FlyCamera::Move(float dist)
{
	this->position += this->front * dist;
}
void FlyCamera::Yaw(float angle)	//Rotate around [Y]
{
	D3DXMATRIX R;
	D3DXMatrixRotationAxis(&R, &this->worldUp, angle);
	
	D3DXVec3TransformNormal(&this->right, &this->right, &R);
	D3DXVec3TransformNormal(&this->front, &this->front, &R);
}
void FlyCamera::Pitch(float angle)	//Rotate around [X]
{
	D3DXMATRIX R;
	D3DXMatrixRotationAxis(&R, &this->right, angle);

	D3DXVec3TransformNormal(&this->up, &this->up, &R);
	D3DXVec3TransformNormal(&this->front, &this->front, &R);
}
void FlyCamera::Ascend(float dist)
{
	this->position +=  this->worldUp * dist;
}


void FlyCamera::updateView()
{
	//Normalize the forwar vector, we only need to know the direction
	D3DXVec3Normalize(&this->front, &this->front);
	
	//get up vector and normalize it
	D3DXVec3Normalize(&this->up, D3DXVec3Cross(&this->up, &this->front, &this->right));

	//get right vector, no need to normalize. (Orthonormal basis)
	D3DXVec3Cross(&this->right, &this->up, &this->front);
	
	//Get view 4th row data
	float rX4 = -D3DXVec3Dot(&this->position, &this->right);
	float uY4 = -D3DXVec3Dot(&this->position, &this->up);
	float fZ4 = -D3DXVec3Dot(&this->position, &this->front);


	/********************
		View-Matrix
	┌					┐
	│r.x   u.x   f.x   0│
	│r.y   u.y   f.y   0│
	│r.z   u.z   f.z   0│
	│-pr   -pu   -pf   1│
	└					┘
	*********************/
	this->view.m[0][0] = this->right.x;
	this->view.m[1][0] = this->right.y;
	this->view.m[2][0] = this->right.z;
	this->view.m[3][0] = rX4;

	this->view.m[0][1] = this->up.x;
	this->view.m[1][1] = this->up.y;
	this->view.m[2][1] = this->up.z;
	this->view.m[3][1] = uY4;

	this->view.m[0][2] = this->front.x;
	this->view.m[1][2] = this->front.y;
	this->view.m[2][2] = this->front.z;
	this->view.m[3][2] = fZ4;

	this->view.m[0][3] = 0;
	this->view.m[1][3] = 0;
	this->view.m[2][3] = 0;
	this->view.m[3][3] = 1;
}


//############# Get funktions ###############
vec3 FlyCamera::getLookAt() const
{
	return this->position + this->front;
	//return lAt;
}
vec3 FlyCamera::getRight() const
{
	return this->right;
}
vec3 FlyCamera::getUp() const
{
	return this->up;
}
vec3 FlyCamera::getfront() const
{
	return this->front;
}
vec3 FlyCamera::getPosition() const
{
	return this->position;
}
D3DXMATRIX	FlyCamera::getView() const
{
	return this->view;
}
D3DXMATRIX	FlyCamera::getInvView() const
{
	Matrix m;
	float det = D3DXMatrixDeterminant(&this->view);
	if(det)
	{
		D3DXMatrixInverse(&m, &det, &this->view);
	}
	return m;
}
D3DXMATRIX	FlyCamera::getProj() const
{
	return this->proj;
}
D3DXMATRIX	FlyCamera::getViewProj() const
{
	return this->view * this->proj;
}
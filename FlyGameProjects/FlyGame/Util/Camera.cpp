#include "Camera.h"

Camera::Camera()
{
	this->positionX = 0.0f;
	this->positionY = 0.0f;
	this->positionZ = 0.0f;

	this->rotationX = 0.0f;
	this->rotationY = 0.0f;
	this->rotationZ = 0.0f;

	this->lookAt = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
}

Camera::Camera(const Camera& other)
{
	this->positionX = other.positionX;
	this->positionY = other.positionY;
	this->positionZ = other.positionZ;

	this->rotationX = other.rotationX;
	this->rotationY = other.rotationY;
	this->rotationZ = other.rotationZ;

	this->lookAt = other.lookAt;

	this->viewMatrix = other.viewMatrix;
	this->projectionMatrix = other.projectionMatrix;
	this->orthogonalMatrix = other.orthogonalMatrix;
}

Camera::~Camera()
{

}

void Camera::SetPosition(float x, float y, float z)
{
	this->positionX = x;
	this->positionY = y;
	this->positionZ = z;
}

void Camera::SetRotation(float x, float y, float z)
{
	this->rotationX = x;
	this->rotationY = y;
	this->rotationZ = z;
}

void Camera::SetProjectionMatrix(float fieldOfView, float aspectRatio, float nearPlane, float farPlane)
{
	D3DXMatrixPerspectiveFovLH(&this->projectionMatrix, fieldOfView, aspectRatio, nearPlane, farPlane);
}

void Camera::SetOrthogonalMatrix(float width, float height, float nearPlane, float farPlane)
{
	D3DXMatrixOrthoLH(&this->orthogonalMatrix, width, height, nearPlane, farPlane);
}

D3DXVECTOR3 Camera::GetPosition() const
{
	return D3DXVECTOR3(this->positionX, this->positionY, this->positionZ);
}

D3DXVECTOR3 Camera::GetRotation() const
{
	return D3DXVECTOR3(this->rotationX, this->rotationY, this->rotationZ);
}

D3DXVECTOR3 Camera::GetLookAt() const
{
	return D3DXVECTOR3(this->lookAt.x, this->lookAt.y, this->lookAt.z);
}

void Camera::Render()
{
	D3DXVECTOR3 up, position, xAxis, yAxis, zAxis;

	D3DXMATRIX rotationMatrix;

	float yaw, pitch, roll;
	
	//Point "up"
	up.x = 0.0f;
	up.y = 1.0f;
	up.z = 0.0f;

	//Set position
	position.x = this->positionX;
	position.y = this->positionY;
	position.z = this->positionZ;

	//Point to 1 z(default)
	this->lookAt.x = 0.0f;
	this->lookAt.y = 0.0f;
	this->lookAt.z = 1.0f;

	//Set rotations
	pitch = this->rotationX*0.0174532925f;
	yaw   = this->rotationY*0.0174532925f;
	roll  = this->rotationZ*0.0174532925f;


	//Create rotation matrix
	D3DXMatrixRotationYawPitchRoll(&rotationMatrix, yaw, pitch, roll);

	//Transform lookAt and up so they are rotated accordingly
	D3DXVec3TransformCoord(&this->lookAt, &this->lookAt, &rotationMatrix);
	D3DXVec3TransformCoord(&up, &up, &rotationMatrix);

	//Translate the rotated camera position to the location of the viewer.
	this->lookAt = position + this->lookAt;

	//Create the view matrix from the three updated vectors
	D3DXVec3Normalize(&zAxis, &(this->lookAt - position));
	D3DXVec3Normalize(&xAxis, D3DXVec3Cross(&xAxis, &up, &zAxis));
	D3DXVec3Cross(&yAxis, &zAxis, &xAxis);

	this->viewMatrix._11 = xAxis.x;
	this->viewMatrix._21 = xAxis.y;
	this->viewMatrix._31 = xAxis.z;
	this->viewMatrix._41 = -D3DXVec3Dot(&xAxis, &position);

	this->viewMatrix._12 = yAxis.x;
	this->viewMatrix._22 = yAxis.y;
	this->viewMatrix._32 = yAxis.z;
	this->viewMatrix._42 = -D3DXVec3Dot(&yAxis, &position);

	this->viewMatrix._13 = zAxis.x;
	this->viewMatrix._23 = zAxis.y;
	this->viewMatrix._33 = zAxis.z;
	this->viewMatrix._43 = -D3DXVec3Dot(&zAxis, &position);

	this->viewMatrix._14 = 0.0f;
	this->viewMatrix._24 = 0.0f;
	this->viewMatrix._34 = 0.0f;
	this->viewMatrix._44 = 1.0f;


}

D3DXMATRIX Camera::GetViewMatrix() const
{
	return this->viewMatrix;
}

D3DXMATRIX Camera::GetProjectionMatrix() const
{
	return this->projectionMatrix;
}

D3DXMATRIX	Camera::GetOrthogonalMatrix() const
{
	return this->orthogonalMatrix;
}

D3DXVECTOR3	Camera::GetForward() const
{
	return D3DXVECTOR3(this->viewMatrix._11, this->viewMatrix._21, this->viewMatrix._31);
}

D3DXVECTOR3 Camera::GetRight() const
{
	return D3DXVECTOR3(this->viewMatrix._13, this->viewMatrix._23, this->viewMatrix._33);
}

D3DXVECTOR3	Camera::GetParallelForward() const
{
	D3DXVECTOR3 up, returnedValue;

	//Point "up"
	up.x = 0.0f;
	up.y = 1.0f;
	up.z = 0.0f;

	D3DXVec3Cross(&returnedValue, &up, &GetRight());

	return returnedValue;
}

D3DXVECTOR3 Camera::GetParallelRight() const
{
	D3DXVECTOR3 up, returnedValue;

	//Point "up"
	up.x = 0.0f;
	up.y = 1.0f;
	up.z = 0.0f;

	D3DXVec3Cross(&returnedValue, &GetForward(), &up);

	return returnedValue;
}

void Camera::SetPositionX(float x)
{
	this->positionX = x;
}
void Camera::SetPositionY(float y)
{
	this->positionY = y;
}
void Camera::SetPositionZ(float z)
{
	this->positionZ = z;
}

void Camera::SetRotationX(float x)
{
	this->rotationX = x;
}
void Camera::SetRotationY(float y)
{
	this->rotationY = y;
}
void Camera::SetRotationZ(float z)
{
	this->rotationZ = z;
}

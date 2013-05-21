#include "Camera.h"

Camera::Camera()
{
	this->positionX = 0.0f;
	this->positionY = 0.0f;
	this->positionZ = 0.0f;

	this->rotationX = 0.0f;
	this->rotationY = 0.0f;
	this->rotationZ = 0.0f;

	this->projNear = 0.0f;
	this->projFar = 0.0f;
	

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

	this->projNear = other.projNear;
	this->projFar = other.projFar;

	this->lookAt = other.lookAt;

	this->viewMatrix = other.viewMatrix;
	this->projectionMatrix = other.projectionMatrix;
	this->orthogonalMatrix = other.orthogonalMatrix;
}
Camera::~Camera()
{

}

int Camera::GetID() const
{
	return this->id;
}

void Camera::SetPosition(float x, float y, float z)
{
	this->positionX = x;
	this->positionY = y;
	this->positionZ = z;
}
void Camera::SetPosition(D3DXVECTOR3 position)
{
	this->positionX = position.x;
	this->positionY = position.y;
	this->positionZ = position.z;
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
void Camera::RelativeForward(float speed)
{
	D3DXVECTOR3 norm;
	D3DXVec3Normalize(&norm, &(this->GetForward()));

	this->positionX += norm.x*speed;
	this->positionY += norm.y*speed;
	this->positionZ += norm.z*speed;

	//D3DXMatrixOrthoLH(&this->orthogonalMatrix, width, height, nearPlane, farPlane);
}
void Camera::RelativeRight(float speed)
{
	D3DXVECTOR3 norm;
	D3DXVec3Normalize(&norm, &(this->GetRight()));

	this->positionX += norm.x*speed;
	this->positionY += norm.y*speed;
	this->positionZ += norm.z*speed;
}
void Camera::RelativeUp(float speed)
{
	D3DXVECTOR3 norm;
	D3DXVec3Normalize(&norm, &(this->GetUp()));

	this->positionX += norm.x*speed;
	this->positionY += norm.y*speed;
	this->positionZ += norm.z*speed;
}
void Camera::RelativeParallelForward(float speed)
{
	D3DXVECTOR3 norm;
	D3DXVec3Normalize(&norm, &(this->GetParallelForward()));

	this->positionX += norm.x*speed;
	this->positionY += norm.y*speed;
	this->positionZ += norm.z*speed;
}
void Camera::RelativeParallelRight(float speed)
{
	D3DXVECTOR3 norm;
	D3DXVec3Normalize(&norm, &(this->GetParallelRight()));

	this->positionX += norm.x*speed;
	this->positionY += norm.y*speed;
	this->positionZ += norm.z*speed;
}

void Camera::SetRotation(float x, float y, float z)
{
	this->rotationX = x;
	this->rotationY = y;
	this->rotationZ = z;
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
void Camera::RelativePitch(float degrees)
{
	this->rotationX += degrees;
}
void Camera::RelativeYaw(float degrees)
{
	this->rotationY += degrees;
}
void Camera::RelativeRoll(float degrees)
{
	this->rotationZ += degrees;
}

void Camera::SetViewMatrix(D3DXMATRIX view)
{
	this->viewMatrix = view;
}
void Camera::SetProjectionMatrix(D3DXMATRIX projection)
{
	this->projectionMatrix = projection;
}
void Camera::SetProjectionMatrix(float fieldOfView, float aspectRatio, float nearPlane, float farPlane)
{
	this->projFar = farPlane;
	this->projNear = nearPlane;
	D3DXMatrixPerspectiveFovLH(&this->projectionMatrix, fieldOfView, aspectRatio, nearPlane, farPlane);
}
void Camera::SetOrthogonalMatrix(float width, float height, float nearPlane, float farPlane)
{
	D3DXMatrixOrthoLH(&this->orthogonalMatrix, width, height, nearPlane, farPlane);
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

D3DXVECTOR3	Camera::GetRight() const
{
	return D3DXVECTOR3(this->viewMatrix._11, this->viewMatrix._21, this->viewMatrix._31);
}
D3DXVECTOR3 Camera::GetForward() const
{
	return D3DXVECTOR3(this->viewMatrix._13, this->viewMatrix._23, this->viewMatrix._33);
}
D3DXVECTOR3 Camera::GetUp() const
{
	return D3DXVECTOR3(this->viewMatrix._12, this->viewMatrix._22, this->viewMatrix._32);
}
D3DXVECTOR3	Camera::GetParallelForward() const
{
	D3DXVECTOR3 up, returnedValue;

	//Point "up"
	up.x = 0.0f;
	up.y = 1.0f;
	up.z = 0.0f;

	D3DXVec3Cross(&returnedValue, &GetRight(), &up);

	return returnedValue;
}
D3DXVECTOR3 Camera::GetParallelRight() const
{
	D3DXVECTOR3 up, returnedValue;

	//Point "up"
	up.x = 0.0f;
	up.y = 1.0f;
	up.z = 0.0f;

	D3DXVec3Cross(&returnedValue, &up, &GetForward());

	return returnedValue;
}


void Camera::ConstructViewFrustum(ViewFrustum& frustum)
{
	float zMinimum, r;
	D3DXMATRIX matrix;
		
	// Calculate the minimum Z distance in the frustum.
	zMinimum = -this->projectionMatrix._43 / this->projectionMatrix._33;
	r = this->projFar / (this->projFar - zMinimum);
	this->projectionMatrix._33 = r;
	this->projectionMatrix._43 = -r * zMinimum;

	// Create the frustum matrix from the view matrix and updated projection matrix.
	D3DXMatrixMultiply(&matrix, &this->viewMatrix, &this->projectionMatrix);

	// Calculate near plane of frustum.
	frustum.planes[0].a = matrix._14 + matrix._13;
	frustum.planes[0].b = matrix._24 + matrix._23;
	frustum.planes[0].c = matrix._34 + matrix._33;
	frustum.planes[0].d = matrix._44 + matrix._43;
	D3DXPlaneNormalize(&frustum.planes[0], &frustum.planes[0]);

	// Calculate far plane of frustum.
	frustum.planes[1].a = matrix._14 - matrix._13; 
	frustum.planes[1].b = matrix._24 - matrix._23;
	frustum.planes[1].c = matrix._34 - matrix._33;
	frustum.planes[1].d = matrix._44 - matrix._43;
	D3DXPlaneNormalize(&frustum.planes[1], &frustum.planes[1]);

	// Calculate left plane of frustum.
	frustum.planes[2].a = matrix._14 + matrix._11; 
	frustum.planes[2].b = matrix._24 + matrix._21;
	frustum.planes[2].c = matrix._34 + matrix._31;
	frustum.planes[2].d = matrix._44 + matrix._41;
	D3DXPlaneNormalize(&frustum.planes[2], &frustum.planes[2]);

	// Calculate right plane of frustum.
	frustum.planes[3].a = matrix._14 - matrix._11; 
	frustum.planes[3].b = matrix._24 - matrix._21;
	frustum.planes[3].c = matrix._34 - matrix._31;
	frustum.planes[3].d = matrix._44 - matrix._41;
	D3DXPlaneNormalize(&frustum.planes[3], &frustum.planes[3]);

	// Calculate top plane of frustum.
	frustum.planes[4].a = matrix._14 - matrix._12; 
	frustum.planes[4].b = matrix._24 - matrix._22;
	frustum.planes[4].c = matrix._34 - matrix._32;
	frustum.planes[4].d = matrix._44 - matrix._42;
	D3DXPlaneNormalize(&frustum.planes[4], &frustum.planes[4]);

	// Calculate bottom plane of frustum.
	frustum.planes[5].a = matrix._14 + matrix._12;
	frustum.planes[5].b = matrix._24 + matrix._22;
	frustum.planes[5].c = matrix._34 + matrix._32;
	frustum.planes[5].d = matrix._44 + matrix._42;
	D3DXPlaneNormalize(&frustum.planes[5], &frustum.planes[5]);

	//Calculate sphere around frustum for faster culling
	float length = this->projFar - this->projNear;
	float height = length*tan((float)D3DX_PI*0.5f*0.5f);
	float width = height*(800/600);

	D3DXVECTOR3 p(0.0f,0.0f, 1+length*0.5f);
	D3DXVECTOR3 q(width, height, length);

	D3DXVECTOR3 vDiff(p-q);

	frustum.sphere.radius = D3DXVec3Length(&vDiff);
	frustum.sphere.center = D3DXVECTOR3(this->positionX, this->positionY, this->positionZ) + (D3DXVECTOR3(this->GetLookAt().x - this->positionX, this->GetLookAt().y - this->positionY, this->GetLookAt().z - this->positionZ)*(length*0.5f));  
}
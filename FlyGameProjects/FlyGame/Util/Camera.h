#ifndef CAMERA_H
#define CAMERA_H

#include <D3DX11\d3dx10math.h>
#include "Proxy.h"

class Camera
{
	private:
		float positionX, positionY, positionZ;
		float rotationX, rotationY, rotationZ;
		float projNear, projFar;

		D3DXMATRIX viewMatrix;
		D3DXMATRIX projectionMatrix;
		D3DXMATRIX orthogonalMatrix;
		D3DXVECTOR3 lookAt;


	public:
		Camera();
		Camera(const Camera& other);
		~Camera();

		void SetPosition(float x, float y, float z);
		void SetPositionX(float x);
		void SetPositionY(float y);
		void SetPositionZ(float z);
		/**Angles are set in degrees*/
		void SetRotation(float x, float y, float z);
		void SetRotationX(float x);
		void SetRotationY(float y);
		void SetRotationZ(float z);

		void SetProjectionMatrix(float fieldOfView, float aspectRatio, float nearPlane, float farPlane);
		void SetOrthogonalMatrix(float width, float height, float nearPlane, float farPlane);

		D3DXVECTOR3 GetPosition() const;
		D3DXVECTOR3 GetRotation() const;
		D3DXVECTOR3 GetLookAt() const;

		void Render();
		void UpdatePosition();

		D3DXMATRIX	GetViewMatrix() const;
		D3DXMATRIX	GetProjectionMatrix() const;
		D3DXMATRIX	GetOrthogonalMatrix() const;
		D3DXVECTOR3	GetForward() const;
		D3DXVECTOR3 GetRight() const;
		/**Use these two functions to get vectors that are parallel to the X and Z-axes (useful for ground movement*/
		D3DXVECTOR3	GetParallelForward() const;
		D3DXVECTOR3 GetParallelRight() const;

		ViewFrustum GetViewFrustum();

		void DennisTemporaryMoveFunction(D3DXVECTOR3 relative);
};

#endif
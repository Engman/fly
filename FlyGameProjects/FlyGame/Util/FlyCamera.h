#ifndef FLY_CAMERA_H
#define FLY_CAMERA_H

#include "misc.h"

class FlyCamera
{
	private:
		D3DXMATRIX	view;
		D3DXMATRIX	proj;

		vec3 position;
		vec3 right;
		vec3 up;
		vec3 front;
		vec3 worldUp;

		float nearZ;
		float farZ;
		float fov;
		float aspect;
	public:
		FlyCamera();
		virtual~FlyCamera();

		void updateView();

		vec3 getLookAt()			const;
		vec3 getRight()				const;
		vec3 getUp()				const;
		vec3 getfront()				const;
		vec3 getPosition()			const;
		D3DXMATRIX  getView()		const;
		D3DXMATRIX  getProj()		const;
		D3DXMATRIX  getViewProj()	const;

		void setWorldUp(vec3);

		void setPos(const vec3);
		void setPos(const float x, const float y, const float z);
		void setHeight(const float y);
		void setPerspective(float fov, float aspect, float zNear, float zFar);
		void setLookAt(vec3 pos, vec3 at, vec3 up);

		void modifyFarZByVal(float z);
		void modifyNearZByVal(float z);

		void Strafe(float dist);
		void Move(float dist);
		void Yaw(float angle);
		void Pitch(float angle);
		void Ascend(float dist);
};

#endif
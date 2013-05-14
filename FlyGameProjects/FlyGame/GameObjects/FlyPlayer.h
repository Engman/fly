#ifndef FLY_PLAYER_H
#define FLY_PLAYER_H

#include "..\..\FlyGameEngine\Core\Mesh\FlyMesh.h"
#include "..\..\FlyGameEngine\Core\Entity.h"
#include "..\..\FlyGameEngine\Util\BoundingVolumes.h"

class FlyPlayer
{
	private:
		vector<Entity*> playerModel;

		vec3 velocity;
		vec3 maxVelocity;

		bool justChangedForm;


	public:
		FlyPlayer();
		virtual~FlyPlayer();

		void Initialize();

		void Render(ViewFrustum& frustum);

		vector<Entity*>* GetModel();

		void SetPosition(vec3 position);
		void SetRotation(vec3 rotation);
		void SetVelocity(vec3 velocity);
		void SetJustChanged(bool changed);

		vec3 GetPosition() const;
		vec3 GetRotation() const;
		vec3 GetVelocity() const;
		vec3 GetMaxVelocity() const;
		bool GetJustChanged() const;

		BoundingSphere* GetBoundingSphere();
		void SetBoundingSphere(BoundingSphere* sphere);

		void Release();
};


#endif


#ifndef FLY_PICKUP_H
#define FLY_PICKUP_H

#include "..\..\FlyGameEngine\Core\Mesh\FlyMesh.h"
#include "..\..\FlyGameEngine\Core\Entity.h"
#include "..\..\FlyGameEngine\Util\BoundingVolumes.h"

class FlyPickup
{
	private:
		vector<Entity*> pickupModel;

		bool taken;

	public:
		FlyPickup();
		virtual~FlyPickup();

		void Initialize();

		void Render(ViewFrustum& frustum);
		void Update();		

		vector<Entity*>* GetModel();

		void SetPosition(vec3 position);
		void SetRotation(vec3 rotation);

		vec3 GetPosition() const;
		vec3 GetRotation() const;

		BoundingSphere* GetBoundingSphere();
		void SetBoundingSphere(BoundingSphere* sphere);

		void Release();
};


#endif


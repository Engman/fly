#ifndef FLY_PICKUP_H
#define FLY_PICKUP_H

#include "..\..\FlyGameEngine\Core\Mesh\FlyMesh.h"
#include "..\..\FlyGameEngine\Core\Entity.h"
#include "..\..\FlyGameEngine\Util\BoundingVolumes.h"
#include "../FlyGame.h"
#include "../../FlyGameEngine/FlyEngine/FlyEngine.h"

class FlyPickup
{
	private:
		vector<Entity*> pickupModel;

		bool taken;

	public:
		FlyPickup();
		virtual~FlyPickup();

		bool Initialize(FlyGame* entry, wstring modelName, vec3 position, vec3 rotation, int shader);

		void Render(ViewFrustum& frustum);
		void Update();		

		void SetPosition(vec3 position);
		void SetRotation(vec3 rotation);
		void SetPickTaken();
		void SetShader(IShader* shader);

		vec3 GetPosition() const;
		vec3 GetRotation() const;
		bool GetTaken() const;

		BoundingSphere* GetBoundingSphere();
		void SetBoundingSphere(BoundingSphere* sphere);

		void Release();
};


#endif


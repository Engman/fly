#ifndef ENERGYPICKUP_H
#define ENERGYPICKUP_H

#include "..\..\FlyGameEngine\Core\Mesh\FlyMesh.h"
#include "..\..\FlyGameEngine\Core\Entity.h"
#include "..\..\FlyGameEngine\Util\BoundingVolumes.h"
#include "../FlyGame.h"
#include "../../FlyGameEngine/FlyEngine/FlyEngine.h"

class EnergyPickup
{
	private:
		vector<Entity*> pickupModel;

		bool taken;

	public:
		EnergyPickup();
		virtual~EnergyPickup();

		bool Initialize(FlyGame* entry, wstring modelName, vec3 position, vec3 rotation, vec3 scale, int shader);

		void Render(ViewFrustum& frustum);
		void Update(float deltaTime);		

		void SetPosition(vec3 position);
		void SetRotation(vec3 rotation);
		void SetScale(vec3 rotation);
		void SetPickTaken(bool taken);
		void SetShader(IShader* shader);

		vec3 GetPosition() const;
		vec3 GetRotation() const;
		vec3 GetScale() const;
		bool GetTaken() const;

		BoundingSphere* GetBoundingSphere();
		void SetBoundingSphere(BoundingSphere* sphere);

		void Release();
};


#endif


#ifndef FLY_PLAYER_H
#define FLY_PLAYER_H

#include "..\..\FlyGameEngine\Core\Mesh\FlyMesh.h"
#include "..\..\FlyGameEngine\Core\Mesh\FlyMeshAnimated.h"
#include "..\..\FlyGameEngine\Core\Entity.h"
#include "..\..\FlyGameEngine\Util\BoundingVolumes.h"

class FlyPlayer
{
	private:
		vector<Entity*> playerModel;

		vec3 initialPos;
		vec3 initialRot;
		vec3 velocity;
		vec3 maxVelocity;

		bool smallModel;

		BoundingEllipse boundingEllipse;

		float energy;
		float maxEnergy;

		bool closeCargo; 
	public:
		FlyPlayer();
		virtual~FlyPlayer();

		void Initialize();

		void Render(ViewFrustum& frustum);
		void Update();		

		vector<Entity*>* GetModel();

		void SetPosition(vec3 position);
		void SetRotation(vec3 rotation);
		void SetScale(vec3 scale);
		void SetVelocity(vec3 velocity);
		void SetEllipseVector(vec3 radius);
		void SetSmall(bool changed);

		vec3 GetPosition() const;
		vec3 GetRotation() const;
		vec3 GetVelocity() const;
		vec3 GetMaxVelocity() const;
		bool GetSmall() const;
		void setInitialPos(vec3 p);
		vec3 getInitialPos();
		void setInitialRot(vec3 r);
		vec3 getInitialRot();
		BoundingEllipse GetEllipse() const;
		float GetEnergy() const;
		float GetMaxEnergy() const;

		BoundingSphere* GetBoundingSphere();
		void SetBoundingSphere(BoundingSphere* sphere);

		void UpdateAnimation(int nr, float deltaTime);
		void StopAnimation(int nr, float deltaTime);
	
		void setCloseCargo(bool close); 
		bool getCloseCargo() const;

		void Release();

		void DeductEnergy(float howMuch);

		//Controls
		void RollLeft();
		void RollRight(vec3 forward);
		void BankLeft();
		void BankRight();
};


#endif


#ifndef PARTICLECOLLISIONSYSTEM_H
#define PARTICLECOLLISIONSYSTEM_H

#include <D3DX11\D3D11.h>
#include <D3DX11\d3dx10math.h>
#include "Entity.h"
#include "Mesh\ParticleMesh.h"
#include "D3DShell.h"
#include "../Util/SmartPtrs.h"
#include "../Util/vertex.h"


////////////////////////////////////////////////////////////////////////////////
// Class name: ParticleSystemClass
////////////////////////////////////////////////////////////////////////////////
class ParticleCollisionSystem
{
	private:
		struct Particle
		{
			float positionX, positionY, positionZ;
			vec3 velocity;
			bool active;
			float life;
			SmartPtrStd<Entity> model;
		};

	public:
		ParticleCollisionSystem();
		ParticleCollisionSystem(const ParticleCollisionSystem&);
		virtual ~ParticleCollisionSystem();

		bool Initialize();
		void Shutdown();
		bool Frame(vec3 forward, float deltaTime);
		void Render(ViewFrustum f);

		void EmitParticles(float, vec3 forward);

		void SetRotation(vec3 rotation);
		void SetPosition(vec3 position);
		void SetParticleShader(IShader* shader);
	private:
		
		void UpdateParticles(float);
		void KillParticles(vec3 position);

		bool UpdateBuffers();

		void RenderBuffers(ID3D11DeviceContext*);

	private:

		float particleDeviationX, particleDeviationY, particleDeviationZ;
		float particleVelocity, particleVelocityVariation;
		float particleSize, particlesPerSecond;
		int maxParticles;

		int currentParticleCount;
		float accumulatedTime;

		vector<Particle> particleList;
		SmartPtrStd<vector<VERTEX::VertexPNT>> vertexList;

		int vertexCount;
};

#endif

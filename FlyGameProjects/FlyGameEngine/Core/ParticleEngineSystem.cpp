////////////////////////////////////////////////////////////////////////////////
// Filename: particlesystemclass.cpp
////////////////////////////////////////////////////////////////////////////////
#include "ParticleEngineSystem.h"
#include "../Core/Mesh/MaterialHandler.h"

ParticleEngineSystem::ParticleEngineSystem()
{
	
}


ParticleEngineSystem::ParticleEngineSystem(const ParticleEngineSystem& other)
{
}


ParticleEngineSystem::~ParticleEngineSystem()
{
}

bool ParticleEngineSystem::Initialize()
{
	// Set the random deviation of where the particles can be located when emitted.
	this->particleDeviationX = 20.0f;
	this->particleDeviationY = 20.0f;
	this->particleDeviationZ = 10.0f;

	// Set the speed and speed variation of particles.
	this->particleVelocity = 8.0f;
	this->particleVelocityVariation = 0.2f;

	// Set the physical size of the particles.
	this->particleSize = 0.05f;

	// Set the number of particles to emit per second.
	this->particlesPerSecond = 5.0f;

	// Set the maximum number of particles allowed in the particle system.
	this->maxParticles = 250;

	// Initialize the current particle count to zero since none are emitted yet.
	this->currentParticleCount = 0;

	// Clear the initial accumulated time for the particle per second emission rate.
	this->accumulatedTime = 0.0f;


	this->vertexList = new vector<VERTEX::VertexPNT>;
	this->vertexCount = 6;

	ObjectMaterial::OBJECT_MATERIAL_DESC materialDesc;

	materialDesc.dc = D3DShell::self()->getDeviceContext();
	materialDesc.ambient = vec4(0.5f, 0.5f, 0.5f, 1.0f);
	materialDesc.diffuse = vec4(1.0f, 1.0f, 1.0f, 1.0f);
	materialDesc.ambient = vec4(0.1f, 0.8f, 0.2f, 1.0f);
	materialDesc.specualarPow = 0.0f;
	materialDesc.device = D3DShell::self()->getDevice();
	materialDesc.ambientTexture = L"";
	materialDesc.diffuseTexture = L"..\\Resources\\Textures\\smoke_particle.png";
	materialDesc.specularTexture = L"..\\Resources\\Textures\\noGlow.png";
	materialDesc.glowTexture = L"..\\Resources\\Textures\\noGlow.png";
	materialDesc.normalTexture = L"..\\Resources\\Textures\\noGlow.png";
	materialDesc.name = L"engine_particle";

	MaterialHandler::AddMaterial(materialDesc);

	ParticleMesh::OBJECT_DESC desc;
	desc.device = D3DShell::self()->getDevice();
	desc.deviceContext = D3DShell::self()->getDeviceContext();
	desc.material_id = MaterialHandler::GetMaterial(L"engine_particle")->GetID();
	desc.name = L"particles";
	desc.vCount = 6;

	this->vertexList->push_back(VERTEX::VertexPNT());
	this->vertexList->push_back(VERTEX::VertexPNT());
	this->vertexList->push_back(VERTEX::VertexPNT());
	this->vertexList->push_back(VERTEX::VertexPNT());
	this->vertexList->push_back(VERTEX::VertexPNT());
	this->vertexList->push_back(VERTEX::VertexPNT());

	for(int i = 0; i < this->maxParticles; i++)
	{
		this->particleList.push_back(Particle());
		this->particleList[i].active = false;
		this->particleList[i].positionX = 0.0f;
		this->particleList[i].positionY = 0.0f;
		this->particleList[i].positionZ = 0.0f;
		this->particleList[i].velocity = vec3(0.0f, 0.0f, 0.0f);
		this->particleList[i].model = new ParticleMesh();
		this->particleList[i].life = 10000.0f;

		float texCoordX = (rand()%3)*0.3333333f;
		float texCoordY = (rand()%3)*0.3333333f;
		this->vertexList->at(0).position = vec4(-this->particleSize, -this->particleSize, 0.0f, 1.0f);
		this->vertexList->at(0).normal = vec4(0.0f, 0.0f, 1.0f, 0.0f);
		this->vertexList->at(0).texcoord = vec2(texCoordX, texCoordY+0.3333333f);

		this->vertexList->at(1).position = vec4(-this->particleSize, this->particleSize, 0.0f, 1.0f);
		this->vertexList->at(1).normal = vec4(0.0f, 0.0f, 1.0f, 0.0f);
		this->vertexList->at(1).texcoord = vec2(texCoordX, texCoordY);

		this->vertexList->at(2).position = vec4(this->particleSize, this->particleSize, 0.0f, 1.0f);
		this->vertexList->at(2).normal = vec4(0.0f, 0.0f, 1.0f, 0.0f);
		this->vertexList->at(2).texcoord = vec2(texCoordX+0.3333333f, texCoordY);

		this->vertexList->at(3).position = vec4(this->particleSize, this->particleSize, 0.0f, 1.0f);
		this->vertexList->at(3).normal = vec4(0.0f, 0.0f, 1.0f, 0.0f);
		this->vertexList->at(3).texcoord = vec2(texCoordX+0.3333333f, texCoordY);

		this->vertexList->at(4).position = vec4(this->particleSize, -this->particleSize, 0.0f, 1.0f);
		this->vertexList->at(4).normal = vec4(0.0f, 0.0f, 1.0f, 0.0f);
		this->vertexList->at(4).texcoord = vec2(texCoordX+0.3333333f, texCoordY+0.3333333f);

		this->vertexList->at(5).position = vec4(-this->particleSize, -this->particleSize, 0.0f, 1.0f);
		this->vertexList->at(5).normal = vec4(0.0f, 0.0f, 1.0f, 0.0f);
		this->vertexList->at(5).texcoord = vec2(texCoordX, texCoordY+0.3333333f);

		desc.vertecies = this->vertexList;

		Entity* tempPoint = this->particleList[i].model;
		dynamic_cast<ParticleMesh*>(tempPoint)->Initialize(desc);
	}

	

	return true;
}

void ParticleEngineSystem::Shutdown()
{
	// Release the texture used for the particles.

	return;
}


bool ParticleEngineSystem::Frame(vec3 forward, float deltaTime)
{
	bool result;


	// Release old particles.
	KillParticles(forward);

	// Emit new particles.
	//EmitParticles(16.0f,forward);
	
	// Update the position of the particles.
	UpdateParticles(deltaTime);


	return true;
}

void ParticleEngineSystem::Render(ViewFrustum f)
{
	// Put the vertex and index buffers on the graphics pipeline to prepare them for drawing.
	//RenderBuffers(deviceContext);

	for(int i = 0; i < this->currentParticleCount; i++)
	{
		this->particleList[i].model->Render(f);
	}

	return;
}


void ParticleEngineSystem::EmitParticles(float frameTime, vec3 forward)
{
	bool emitParticle;
	float positionX, positionY, positionZ;
	vec3 velocity;


	// Increment the frame time.
	this->accumulatedTime += frameTime;

	// Set emit particle to false for now.
	emitParticle = false;
	
	// Check if it is time to emit a new particle or not.
	if(this->accumulatedTime > (1000.0f / this->particlesPerSecond))
	{
		this->accumulatedTime = 0.0f;
		emitParticle = true;
	}

	// If there are particles to emit then emit one per frame.
	if((emitParticle == true) && (this->currentParticleCount < (this->maxParticles - 1)))
	{
		// Now generate the randomized particle properties.
		positionX = this->particleDeviationX;
		positionY = this->particleDeviationY;
		positionZ = this->particleDeviationZ;

		for(int i = 0; i < 10; i++)
		{
			this->currentParticleCount++;
			D3DXMATRIX rotation;
			D3DXMatrixRotationYawPitchRoll(&rotation, i*0.628f, 0.0f, 0.0f);
			vec4 vectorFour;

			vec3 velocityThree = *D3DXVec4Transform(&vectorFour, &vec4(-forward, 0.0f), &rotation);

		

			// Now insert it into the particle array in the correct depth order.
			this->particleList[this->currentParticleCount-1].model->setPosition(vec3(positionX, positionY, positionZ));
			this->particleList[this->currentParticleCount-1].velocity = velocityThree;
			this->particleList[this->currentParticleCount-1].active = true;
			this->particleList[this->currentParticleCount-1].life = 2000.0f;
		}
	}

	return;
}

void ParticleEngineSystem::UpdateParticles(float frameTime)
{
	int i;

	// Each frame we update all the particles by making them move downwards using their position, velocity, and the frame time.
	for(i=0; i<this->currentParticleCount; i++)
	{
		this->particleList[i].model->setPosition(this->particleList[i].model->getPosition() + this->particleList[i].velocity*frameTime);
		this->particleList[i].life -= frameTime*1000.0f;
	}

	return;
}

void ParticleEngineSystem::KillParticles(vec3 position)
{
	int i;


	// Kill all the particles that have gone below a certain height range.
	for(i=0; i<this->currentParticleCount; i++)
	{
		if((this->particleList[i].active == true) && this->particleList[i].life < 0.0f)
		{	
			this->currentParticleCount--;
			this->particleList[i].active = false;
			this->particleList[i].model->setPosition(this->particleList[this->currentParticleCount].model->getPosition());
			this->particleList[i].velocity  = this->particleList[this->currentParticleCount].velocity;
			this->particleList[i].active    = this->particleList[this->currentParticleCount].active;
			this->particleList[i].life    = this->particleList[this->currentParticleCount].life;
			i--;
		}
	}

	return;
}

void ParticleEngineSystem::SetRotation(vec3 rotation)
{
	for(int i = 0; i < this->currentParticleCount; i++)
	{
		this->particleList[i].model->setRotation(rotation);
	}
}

void ParticleEngineSystem::SetPosition(vec3 position)
{
	this->particleDeviationX = position.x;
	this->particleDeviationY = position.y;
	this->particleDeviationZ = position.z;
}

void ParticleEngineSystem::SetParticleShader(IShader* shader)
{
	for(int i = 0; i < this->currentParticleCount; i++)
	{
		this->particleList[i].model->setShader(shader);
	}
}
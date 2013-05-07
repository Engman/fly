#include "Terrain.h"

Terrain::Terrain()
	: FlyMesh()
{
	
}

Terrain::~Terrain()
{

}

bool Terrain::Initialize(OBJECT_DESC& data)
{
	this->vertices = data.vertecies;

	this->material = MaterialHandler::GetMaterial(data.material_id);
	if(!this->material)
		DisplayText("A material could not be found", "Warning!");

	this->shader = data.shader;

	this->octTree.Initialize(this->vertices, data.vCount, 2);
	
	return true;
}

void Terrain::Update()
{

}

void Terrain::Render(ViewFrustum& frustum)
{
	vector<OctTree::RenderBufferType> nodeBuffers = this->octTree.Render(frustum);

	if(this->shader)
	{
		IShader::DRAW_DATA data;

		for(int i = 0; i<(int)nodeBuffers.size(); i++)
			data.buffers.push_back(nodeBuffers[i].vertexBuffer);

		data.worldMatrix = &this->world;
		data.material = this->material;
		this->shader->addDrawData(data);
	}
}


void Terrain::Release()
{
	this->octTree.Release();
}
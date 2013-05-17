#include "Terrain.h"

Terrain::Terrain()
	: FlyMesh()
{
	
}

Terrain::~Terrain()
{
	this->octTree.Release();
}

bool Terrain::Initialize(OBJECT_DESC& data)
{
	this->vertices = data.vertecies;

	this->material = MaterialHandler::GetMaterial(data.material_id);
	if(!this->material)
		DisplayText("A material could not be found", "Warning!");

	this->shader = data.shader;

	this->octTree.Initialize(this->vertices, data.vCount, 1);
	
	return true;
}

void Terrain::Update()
{

}

void Terrain::Render(ViewFrustum& frustum)
{
	if(this->shader)
	{
		IShader::DRAW_DATA data;
		data.worldMatrix = &this->world;
		data.material = this->material;

		this->octTree.Render(frustum, this->shader, data);

		
		//this->shader->addDrawData(data);
	}
}


void Terrain::Release()
{
	this->octTree.Release();
}


vector<vector<D3DXVECTOR3>*> Terrain::GetCollidedBoxes(BoundingSphere sphere)
{
	return this->octTree.GetCollidedBoxes(sphere);
}
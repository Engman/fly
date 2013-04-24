#include "Terrain.h"

Terrain::Terrain()
	: Object()
{
	
}

bool Terrain::Initialize(OBJECT_DESC& data)
{
	
	//this->vertices = new vector<VERTEX::VertexPNT>;
	this->vertices = data.vertecies;



	//this->shader = data.shader;

	this->octTree.Initialize(this->vertices, data.vCount, 2);
	
	return true;
}

void Terrain::Update()
{

}

void Terrain::Render()
{
}

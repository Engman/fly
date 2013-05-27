#include "Terrain.h"

Terrain::Terrain()
	: FlyMesh()
{
	
}

Terrain::~Terrain()
{
	this->octTree.Release();
}

bool Terrain::Initialize(OBJECT_DESC& data, int renderBoxes, int collisionBoxes)
{
	this->vertices = data.vertecies;

	this->material = MaterialHandler::GetMaterial(data.material_id);
	if(!this->material)
		DisplayText("A material could not be found", "Warning!");

	this->shader = data.shader;

	this->octTree.Initialize(this->vertices, data.vCount, renderBoxes, collisionBoxes);
	
	return true;
}

void Terrain::Update()
{

}

void Terrain::Render(ViewFrustum& frustum)
{
	if(this->shader)
	{
		D3DXMATRIX scale, rotation, translation;
		D3DXMatrixScaling(&scale, this->scale.x, this->scale.y, this->scale.z);
		D3DXMatrixRotationYawPitchRoll(&rotation, this->rotation.y, this->rotation.x, this->rotation.z);
		D3DXMatrixTranslation(&translation, this->translation.x, this->translation.y, this->translation.z);

		D3DXMatrixIdentity(&this->world);

		this->world *= scale;
		this->world *= rotation;
		this->world *= translation;

		IShader::DRAW_DATA data;
		data.worldMatrix = &this->world;
		data.material = this->material;

		this->octTree.Render(frustum, this->shader, data);
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

BoundingBox Terrain::GetBoundingBox() const
{
	return this->octTree.GetTopBox();
}

void Terrain::TransformBoxes()
{
	D3DXMATRIX scale, rotation, translation;
	D3DXMatrixScaling(&scale, this->scale.x, this->scale.y, this->scale.z);
	D3DXMatrixRotationYawPitchRoll(&rotation, this->rotation.y, this->rotation.x, this->rotation.z);
	D3DXMatrixTranslation(&translation, this->translation.x, this->translation.y, this->translation.z);

	D3DXMatrixIdentity(&this->world);

	this->world *= scale;
	this->world *= rotation;
	this->world *= translation;

	this->octTree.TransformBoxes(this->world);
}
#include "FlyMesh.h"
#include "..\..\Util\CollisionLib.h"
#include "..\WindowShell.h"

FlyMesh::FlyMesh()
	:Entity(Type::OBJECT)
{
	D3DXMatrixIdentity(&this->world);
	this->translation = vec3(0.0f, 0.0f, 0.0f);
	this->rotation	= vec3(0.0f, 0.0f, 0.0f);
	this->scale = vec3(1.0f, 1.0f, 1.0f);
	D3DXMatrixIdentity(&this->world);
}
FlyMesh::~FlyMesh()
{}

void FlyMesh::Update()
{
	D3DXMATRIX rotation;
	D3DXMATRIX scaling;
	D3DXMATRIX translation;
	D3DXMatrixIdentity(&this->world);

	D3DXMatrixScaling(&scaling, this->scale.x, this->scale.y, this->scale.z);
	D3DXMatrixRotationYawPitchRoll(&rotation, this->rotation.y, this->rotation.x, this->rotation.z);
	D3DXMatrixTranslation(&translation, this->translation.x, this->translation.y, this->translation.z);

	this->world *= scaling;
	this->world *= rotation;
	this->world *= translation;
}
void FlyMesh::Render(ViewFrustum& frustum)
{
	if(!this->shader)
		return;

	if(this->boundingSphere.IsValid() && 1 == 2)
	{
		if(this->shader && FrustumVSSphere(frustum, *this->boundingSphere))
		{
			IShader::DRAW_DATA data;

			data.worldMatrix = &this->world;
		
			for(int i = 0; i<(int)this->buffers.size(); i++)
				data.buffers.push_back(this->buffers[i]);

			data.material = this->material;
			this->shader->addDrawData(data);
		}
	}
	else
	{
		IShader::DRAW_DATA data;
		for(int i = 0; i<(int)this->buffers.size(); i++)
			data.buffers.push_back(this->buffers[i]);

		data.worldMatrix = &this->world;
		data.material = this->material;
		this->shader->addDrawData(data);
	}

	static int cc = 0;
	cc++;
	if(cc > 3000)
	{
		cc = 0;
		//wchar_t buffX[10];
		//wchar_t buffY[10];
		//wchar_t buffZ[10];
		//
		//_itow(this->boundingSphere->center.x * 100, buffX, 10);
		//_itow(this->boundingSphere->center.y * 100, buffY, 10);
		//_itow(this->boundingSphere->center.z * 100, buffZ, 10);
		//wstring text1 = L"BS (";
		//text1.append(buffX);
		//text1.append(L", ");
		//text1.append(buffY);
		//text1.append(L", ");
		//text1.append(buffZ);
		//text1.append(L")");
		//
		//
		//_itow(this->translation.x * 100, buffX, 10);
		//_itow(this->translation.y * 100, buffY, 10);
		//_itow(this->translation.z * 100, buffZ, 10);
		//wstring text2 = L"MESH (";
		//text2.append(buffX);
		//text2.append(L", ");
		//text2.append(buffY);
		//text2.append(L", ");
		//text2.append(buffZ);
		//text2.append(L")\n");
		//text2.append(text1);
		//
		//MessageBox(WindowShell::self()->getHWND(), text2.c_str(), L"", 0);
	}
}

bool FlyMesh::Initialize(OBJECT_DESC& data)
{
	if(!data.device)
	{
		DisplayText("ID3D11Device is invalid!");
		return false;
	}
	if(!data.vertecies->size())
	{
		DisplayText("Nothing to initialize!");
		return false;
	}
	if(!data.vCount)
	{
		DisplayText("Vertex count invalid!");
		return false;
	}
	if(data.filename == L"" || data.filename.size() == 0)
	{
		DisplayText("No filename specified!");
		return false;
	}

	vec3 vertex;

	this->vertexList = new vector<vec3>;

	for(unsigned int i = 0; i < data.vertecies->size(); i++)
	{
		vertex.x = data.vertecies->at(i).position.x;
		vertex.y = data.vertecies->at(i).position.y;
		vertex.z = data.vertecies->at(i).position.z;

		this->vertexList->push_back(vertex);
	}
	
	this->material = MaterialHandler::GetMaterial(data.material_id);
	if(!this->material)
		DisplayText("A material could not be found", "Warning!");

	SmartPtrStd<BaseBuffer> b = new BaseBuffer();
	BaseBuffer::BUFFER_INIT_DESC desc;
	desc.device			= data.device;
	desc.dc				= data.deviceContext;
	desc.data			= &(*data.vertecies)[0];
	desc.elementSize	= sizeof(VERTEX::VertexPNT);
	desc.nrOfElements	= (int)data.vCount;
	desc.type			= BUFFER_FLAG::TYPE_VERTEX_BUFFER;
	desc.usage			= BUFFER_FLAG::USAGE_DEFAULT;
	if( FAILED( b->Initialize(desc) ) )
	{
		DisplayText("Failed to initiate Buffer in FlyMesh");
		return false;
	}
	this->buffers.push_back(b);
	this->shader			= data.shader;
	this->name				= data.name;
	this->_filename			= data.filename;
	this->boundingSphere	= data.boundingSphere;


	return true;
}

vector<vec3>* FlyMesh::GetTriangles()
{
	return this->vertexList;
}


#include "FlyMeshAnimated.h"
#include "..\..\Util\CollisionLib.h"
#include "..\Input.h"

FlyMeshAnimated::FlyMeshAnimated()
	:Entity(Type::OBJECT)
{
	D3DXMatrixIdentity(&this->world);
	D3DXMatrixIdentity(&this->transformation);
	this->rotation	= vec3(0.0f, 0.0f, 0.0f);
	D3DXMatrixIdentity(&this->world);
}
FlyMeshAnimated::~FlyMeshAnimated()
{

}

void FlyMeshAnimated::Update()
{

	this->boundingSphere->center = this->getPosition();
	//for(int i=0; i<this->animationCount; i++)
	//{
	//	this->time.at(i) += 0.01f;
	//	if( this->time.at(i) > this->frameData.at(i).at(this->currFrame.at(i)).frameTime)
	//	{
	//		this->time.at(i) = 0.0f;
	//		this->currFrame.at(i) ++; 

	//		if(this->currFrame.at(i)>=(int) this->frameData.at(i).size())
	//		{
	//			this->currFrame.at(i) = 1; 
	//		}
	//	}
	//}
}
void FlyMeshAnimated::UpdateAnimation(int nr)
{
	this->time.at(nr) += 0.01f;
	if( this->time.at(nr) > this->frameData->at(nr).at(this->currFrame.at(nr)).frameTime)
	{
		this->time.at(nr) = 0.0f;
		this->currFrame.at(nr) ++; 

		if(this->currFrame.at(nr)>=(int) this->frameData->at(nr).size())
		{
			this->currFrame.at(nr) = 1; 
		}
	}
}
void FlyMeshAnimated::StopAnimation(int nr)
{
	if( this->time.at(nr)> 0.0f)
	{

		this->time.at(nr) += 0.01f;
		if( this->time.at(nr) > this->frameData->at(nr).at(this->currFrame.at(nr)).frameTime)
		{
			this->time.at(nr) = 0.0f;
		
			this->currFrame.at(nr) = 1; 
			
		}
	}
}
void FlyMeshAnimated::Render(ViewFrustum& frustum)
{
	if(animationCount==2)
	{
		MorphAmination2();
	}
	else
		MorphAmination();

	if(this->boundingSphere.IsValid())
	{
		if(this->shader && FrustumVSSphere(frustum, *this->boundingSphere))
		{
			D3DXMATRIX rotation, translation;
			D3DXMatrixRotationYawPitchRoll(&rotation, this->rotation.y, this->rotation.x, this->rotation.z);
			D3DXMatrixTranslation(&translation, this->translation.x, this->translation.y, this->translation.z);

			D3DXMatrixIdentity(&this->world);

			this->world *= rotation;
			this->world *= translation;

			IShader::DRAW_DATA data;
		
			//for(int i = 0; i<(int)this->buffers.size(); i++)
			//	data.buffers.push_back(this->buffers[i]);
			
			data.buffers.push_back(this->buffers[0]);

			data.worldMatrix = &this->world;
			data.material = this->material;
			this->shader->addDrawData(data);
		}
	}
	else
	{
		D3DXMATRIX rotation, translation;
		D3DXMatrixRotationYawPitchRoll(&rotation, this->rotation.y, this->rotation.x, this->rotation.z);
		D3DXMatrixTranslation(&translation, this->translation.x, this->translation.y, this->translation.z);

		D3DXMatrixIdentity(&this->world);

		this->world *= rotation;
		this->world *= translation;

		IShader::DRAW_DATA data;
		
		for(int i = 0; i<(int)this->buffers.size(); i++)
		{
			data.buffers.push_back(this->buffers[i]);
		}
		
		data.worldMatrix = &this->world;
		data.material = this->material;
		this->shader->addDrawData(data);
		
	}
}

bool FlyMeshAnimated::Initialize(ANIM_OBJECT_DESC& data)
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
	
	vec3 vertex;

	
	std::vector<vec3> tempVector; 

	
	for(unsigned int i = 0; i < (*data.vertecies).size(); i++)
	{
		for(int k =0; k<(int)(*data.vertecies).at(i).size(); k++)
		{
			vertex.x = (*data.vertecies).at(i).at(k).position.x;
			vertex.y = (*data.vertecies).at(i).at(k).position.y;
			vertex.z = (*data.vertecies).at(i).at(k).position.z;
			tempVector.push_back(vertex);
		}
		this->vertexList.push_back(tempVector);
		tempVector.clear();
	}

	this->material = MaterialHandler::GetMaterial(data.material_id);
	if(!this->material)
		DisplayText("A material could not be found", "Warning!");

	
	BaseBuffer::BUFFER_INIT_DESC desc;
	desc.device			= data.device;
	desc.dc				= data.deviceContext;
	desc.elementSize	= sizeof(VERTEX::VertexPNT);
	desc.nrOfElements	= (int)data.vCount;
	desc.type			= BUFFER_FLAG::TYPE_VERTEX_BUFFER;
	desc.usage			= BUFFER_FLAG::USAGE_DYNAMIC_CPU_WRITE_DISCARD;
	
	SmartPtrStd<BaseBuffer> b = new BaseBuffer();
	if( FAILED( b->Initialize(desc) ) )
	{
		DisplayText("Failed to initiate Buffer in FlyMesh");
		return false;
	}
	this->buffers.push_back(b);

	/*for( int i=0; i<(int)(*data.vertecies).size(); i++)
	{	
		this->buffers.push_back(b);
	}*/

	keyFramesVertices = new vector<vector<VERTEX::VertexPNT>>; 
	//keyFramesVertices = data.vertecies;

	for(int i= 0; i<data.vertecies->size(); i++)
	{
		keyFramesVertices->push_back(data.vertecies->at(i));
	}

	
	float time= 0.0f;
	float lastTime = 0.0f;
	frameData = new vector<vector<FrameData>>; 
	for( int i=0; i<(int)(*data.frames).size(); i++)
	{
		 
		vector<FrameData> tempFrame; 
		for(int k= 0; k<(int)(*data.frames).at(i).size(); k++)
		{
			FrameData fd = (*data.frames).at(i).at(k);
			time =  (*data.frames).at(i).at(k).frameTime - lastTime; 
			fd.frameTime = time;
			lastTime = (*data.frames).at(i).at(k).frameTime; 
			tempFrame.push_back(fd);
		}
		


		(*frameData).push_back(tempFrame);
	}

	this->shader			= data.shader;
	this->name				= data.name;
	
	this->boundingSpheres  = new vector<BoundingSphere>;
	for(int i= 0; i<(int)data.boundingSphere->size();i++)
	{
		this->boundingSpheres->push_back((*data.boundingSphere).at(i));
	}
	
	for(int i=0; i<(int)(*data.frames).size(); i++)
	{
		this->time.push_back(0.0f);
		this->currFrame.push_back(0); 
	}
	this->vertexCount = data.vCount; 
	this->animationCount = (*data.frames).size();

	triangleList = new vector<vec3>; 
	triangleList->resize(vertexCount);
	//delete data.vertecies
	return true;
}

vector<vec3>* FlyMeshAnimated::GetTriangles()
{
	
	return this->triangleList;
}

void FlyMeshAnimated::MorphAmination()
{
	VERTEX::VertexPNT* morphedMesh = (VERTEX::VertexPNT*)this->buffers[0]->Map();

	int* frameNr =  new int[this->animationCount];
	float* weights = new float[this->animationCount]; 
	int frameCount = 0; 

	for(int i = 0; i<this->animationCount; i++)
	{
		int frame = this->currFrame.at(i);
		frameNr[i] = frameData->at(i).at(frame).frameNumber + frameCount;
		frameCount += frameData->at(i).size();

		if(this->frameData->at(i).at(this->currFrame.at(i)).frameTime!=0)
		{
			weights[i] = time.at(i)/ this->frameData->at(i).at(this->currFrame.at(i)).frameTime; 
		}
		else
		{
			weights[i] = 0.0f; 
		}
		
	}

	for(int i=0; i<this->vertexCount; i++)
	{
		D3DXVECTOR4 position	= keyFramesVertices->at(frameNr[0]).at(i).position; 
		D3DXVECTOR4 normal		= keyFramesVertices->at(frameNr[0]).at(i).normal; 
		D3DXVECTOR2 texture		= keyFramesVertices->at(frameNr[0]).at(i).texcoord; 

		for(int k =1; k<this->animationCount; k++)
		{
			position	+= (keyFramesVertices->at(frameNr[k]).at(i).position - keyFramesVertices->at(frameNr[k]-1).at(i).position) * weights[k];
			normal		+= (keyFramesVertices->at(frameNr[k]).at(i).normal	- keyFramesVertices->at(frameNr[k]-1).at(i).normal) * weights[k];
		}
		
		morphedMesh[i].position = position;
		morphedMesh[i].normal	= normal;
		morphedMesh[i].texcoord = texture;

	}
	
	vec3 pos; 
	for(int i=0; i<this->vertexCount; i++)
	{
		pos.x  = morphedMesh[i].position.x;
		pos.y  = morphedMesh[i].position.y;
		pos.z  = morphedMesh[i].position.z;
		this->triangleList->at(i) =  pos; 
	}

	this->buffers[0]->Unmap();
	delete [] frameNr; 
	delete [] weights;
}

void FlyMeshAnimated::MorphAmination2()
{
	VERTEX::VertexPNT* morphedMesh = (VERTEX::VertexPNT*)this->buffers[0]->Map();

	int* frameNr =  new int[this->animationCount];
	float* weights = new float[this->animationCount]; 
	int frameCount = 0; 

	for(int i = 0; i<this->animationCount; i++)
	{
		int frame = this->currFrame.at(i);
		frameNr[i] = frameData->at(i).at(frame).frameNumber + frameCount;
		frameCount += frameData->at(i).size();

		if(this->frameData->at(i).at(this->currFrame.at(i)).frameTime!=0)
		{
			weights[i] = time.at(i)/ this->frameData->at(i).at(this->currFrame.at(i)).frameTime; 
		}
		else
		{
			weights[i] = 0.0f; 
		}

	}

	for(int i=0; i<this->vertexCount; i++)
	{
		D3DXVECTOR4 position	= keyFramesVertices->at(frameNr[1]-1).at(i).position;
		D3DXVECTOR4 normal		= keyFramesVertices->at(frameNr[1]-1).at(i).normal;
		D3DXVECTOR2 texture		= keyFramesVertices->at(frameNr[1]-1).at(i).texcoord; 

		position	+= (keyFramesVertices->at(frameNr[1]).at(i).position - keyFramesVertices->at(frameNr[1]-1).at(i).position) * weights[1];
		normal		+= (keyFramesVertices->at(frameNr[1]).at(i).normal	- keyFramesVertices->at(frameNr[1]-1).at(i).normal) * weights[1];
		

		morphedMesh[i].position = position;
		morphedMesh[i].normal	= normal;
		morphedMesh[i].texcoord = texture;

	}

	vec3 pos; 
	for(int i=0; i<this->vertexCount; i++)
	{
		pos.x  = morphedMesh[i].position.x;
		pos.y  = morphedMesh[i].position.y;
		pos.z  = morphedMesh[i].position.z;
		this->triangleList->at(i) =  pos; 
	}

	this->buffers[0]->Unmap();
	delete [] frameNr; 
	delete [] weights;
}
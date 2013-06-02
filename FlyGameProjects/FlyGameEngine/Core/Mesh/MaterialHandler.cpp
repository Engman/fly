#include "MaterialHandler.h"
#include "..\..\Util\MutexHandler.h"


static HANDLE MaterialResourceMutex = CreateMutex(0, FALSE, 0);
static std::vector<SmartPtrStd<ObjectMaterial>> MaterialHandlerMaterialList = std::vector<SmartPtrStd<ObjectMaterial>>();

int findExistingMaterial(std::wstring name)
{
	for (int i = 0; i < (int)MaterialHandlerMaterialList.size(); i++)
	{
		if(MaterialHandlerMaterialList[i]->GetName() == name)
			return i;
	}
	return -1;
}



int MaterialHandler::AddMaterial(ObjectMaterial::OBJECT_MATERIAL_DESC& desc)
{
	
	if(!MutexHandler::SetMutex(MATERIAL_HANDLER, true))
		return -1;

	int k = findExistingMaterial(desc.name);
	//if(k != -1)
	//	return MaterialHandlerMaterialList[k]->GetID();

	SmartPtrStd<ObjectMaterial> m = new ObjectMaterial();
	if(!m->CreateMaterial(desc))
	{
		MutexHandler::SetMutex(MATERIAL_HANDLER);
		return -1;
	}
	bool done = false;
	int insertIndex = 0;
	MaterialHandlerMaterialList.resize(MaterialHandlerMaterialList.size()+1);

	for (int i = 0; !done && i < (int)MaterialHandlerMaterialList.size()-1; i++)
	{
		if(MaterialHandlerMaterialList[i]->GetID() > m->GetID())
		{
			for (int k = (int)(MaterialHandlerMaterialList.size() - 1); !done && (k < i); k++)
			{
				MaterialHandlerMaterialList[k] = MaterialHandlerMaterialList[k-1];
			}
			insertIndex = i;
			done = true;
		}
		else
		{
			insertIndex = i + 1;
		}
	}

	MaterialHandlerMaterialList[insertIndex] = m;

	MutexHandler::SetMutex(MATERIAL_HANDLER);

	return MaterialHandlerMaterialList[insertIndex]->GetID();
}

// G�r en klass som hanterar singleton resurserna f�r inladdning osv

bool MaterialHandler::RemoveMaterial(int GID)
{
	if(!MutexHandler::SetMutex(MATERIAL_HANDLER, true))
		return false;

	bool done = false;
	for (int i = 0; i < (int)MaterialHandlerMaterialList.size(); i++)
	{
		if(MaterialHandlerMaterialList[i]->GetID() == GID)
		{
			MaterialHandlerMaterialList.erase(MaterialHandlerMaterialList.begin() + i);
			done = true;
			break;
		}
	}
	MutexHandler::SetMutex(MATERIAL_HANDLER);
	return done;
}

ObjectMaterial* MaterialHandler::GetMaterial(int GID)
{
	if(!MutexHandler::SetMutex(MATERIAL_HANDLER, true))
		return 0;

	int first = 0;
	int last = (int)MaterialHandlerMaterialList.size()-1;
	int mid = (int)last/2;

	//If only one material
	if(first == last)
	{
		MutexHandler::SetMutex(MATERIAL_HANDLER);
		return (GID == MaterialHandlerMaterialList[mid]->GetID()) ? MaterialHandlerMaterialList[mid] : NULL;
	}
	while (first <= last)
	{
		if(MaterialHandlerMaterialList[mid]->GetID() > GID)
		{
			last = mid-1;
			mid = (int)last/2;
		}
		else if(MaterialHandlerMaterialList[mid]->GetID() < GID)
		{
			first = mid+1;
			mid = first + ((int)(last-first)/2);
		}
		else
		{
			MutexHandler::SetMutex(MATERIAL_HANDLER);
			return MaterialHandlerMaterialList[mid];
		}
	}
	MutexHandler::SetMutex(MATERIAL_HANDLER);
	return NULL;
}
ObjectMaterial* MaterialHandler::GetMaterial(std::wstring materialName)
{
	for (int i = 0; i < (int)MaterialHandlerMaterialList.size(); i++)
	{
		if(MaterialHandlerMaterialList[i]->GetName() == materialName)
			return MaterialHandlerMaterialList[i];
	}

	return NULL;
}


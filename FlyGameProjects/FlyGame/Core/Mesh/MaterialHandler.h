#ifndef MATERIAL_HANDLER_H
#define MATERIAL_HANDLER_H

#include "ObjectMaterial.h"
#include "..\..\Util\SmartPtrs.h"
#include "..\..\Util\GID.h"
#include <vector>

/** 
*	Handles all materials in core.
*	Adding a new material will giv ownership to the handler
*/
class MaterialHandler
{
	public:
		/** If success, this will return the unuiqe identifier that can be used to fetch the material
		*	If it fails it returnes -1*/
		static int AddMaterial(ObjectMaterial::OBJECT_MATERIAL_DESC& material);

		/** Returns the specific material or NULL if fail */
		static ObjectMaterial* GetMaterial(int GID);
		/** Returns the specific material or NULL if fail 
		*	It's recommended to not use this function in realtime,
		*	use this to extract the material identifier in initializing phase
		*	an use the identifier to get the material. */
		static ObjectMaterial* GetMaterial(std::wstring materialName);
};


#endif

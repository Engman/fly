#ifndef OBJECT_MATERIAL_H
#define OBJECT_MATERIAL_H

#include "SmartPtrs.h"
#include "misc.h"


class ObjectMaterial
{
	private:
		vec4 ambient;
		vec4 diffuse;
		vec4 specular;

		

		float specualrPow;



	public:
		ObjectMaterial();
		virtual~ObjectMaterial();

};


#endif


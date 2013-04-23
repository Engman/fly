#ifndef ENTITY_H
#define ENTITY_H

#include "..\Util\GID.h"
#include "..\Util\misc.h"
#include "..\Util\SmartPtrs.h"

#include "IShader.h"
#include "BaseBuffer.h"

#include <vector>

namespace Type
{
	const int LIGHT				=  0;
	const int OBJECT			=  1;
	const int TYPELESS			= -1;
}


class Entity abstract
{
	private:
		int			type;
		GID			id;

	protected:
		Matrix		world;
		Matrix		transformation;
		vec3		rotation;

		IShader		*shader;
		std::vector<SmartPtrStd<BaseBuffer>> buffers;

	public:
		Entity					(int _type)
			:type(_type)
		{}
		Entity					(const Entity& origObj)
		{
			this->world				= origObj.world;
			this->transformation	= origObj.transformation;
			this->rotation			= origObj.rotation;
			this->buffers			= origObj.buffers;
		}
		virtual~Entity			()
		{}
		Entity& operator=		(const Entity& origObj)
		{
			this->world				= origObj.world;
			this->transformation	= origObj.transformation;
			this->rotation			= origObj.rotation;
			this->buffers			= origObj.buffers;

			return *this;
		}
		bool operator ==		(const Entity& object)
		{
			return this->id == object.id;
		}
		bool operator ==		(int id)
		{
			return this->id == id;
		}
		bool isOfType			(int type)
		{
			return this->type == type;
		}
		bool isOfType			(const Entity& object)
		{
			return this->type == object.type;
		}
		
		int getType				()	const
		{ 
			return this->type; 
		}
		int getID				()	const
		{
			return this->id;
		}
		vec3 getPosition		()	const
		{
			return vec3(this->transformation.m[3]);
		}
		vec3 getRotation		()	const  
		{
			return this->rotation;
		}
		vec3 getFront			()	const  
		{
			return vec3(this->transformation.m[2]);
		}
		vec3 getRight			()	const  
		{
			vec3(this->transformation.m[0]);
		}
		vec3 getUp				()	const  
		{
			vec3(this->transformation.m[1]);
		}
		IShader* getShader		()
		{
			return this->shader;
		}

		void setPosition		(vec3 _position)
		{
			this->transformation._41 = _position.x;
			this->transformation._42 = _position.y;
			this->transformation._43 = _position.z;
		}
		void setRotation		(vec3 _rotation)
		{
			this->rotation = _rotation;
		}
		void setFront			(vec3 _front)
		{
			this->transformation._31 = _front.x;
			this->transformation._32 = _front.y;
			this->transformation._33 = _front.z;
		}
		void setRight			(vec3 _right)
		{
			this->transformation._11 = _right.x;
			this->transformation._12 = _right.y;
			this->transformation._13 = _right.z;
		}
		void setUp				(vec3 _up)
		{
			this->transformation._21 = _up.x;
			this->transformation._22 = _up.y;
			this->transformation._23 = _up.z;
		}
		void setShader			(IShader* _shader)
		{
			this->shader = _shader;
		}
		std::vector<SmartPtrStd<BaseBuffer>>* getBuffers()
		{
			return &this->buffers;
		}

		virtual void Update	(){}
		virtual void Render	(){}

		
};


#endif
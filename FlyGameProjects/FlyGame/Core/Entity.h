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
		vec3		position;
		vec3		rotation;
		vec3		front;
		vec3		right;
		vec3		up;
		IShader		*shader;
		std::vector<SmartPtrStd<BaseBuffer>> buffers;

	public:
		Entity					(int _type)
			:type(_type)
		{}
		Entity					(const Entity& origObj)
		{
			this->position	= origObj.position;
			this->rotation	= origObj.rotation;
			this->front		= origObj.front;
			this->right		= origObj.right;
			this->up		= origObj.up;
			this->buffers	= origObj.buffers;
		}
		virtual~Entity			()
		{}
		Entity& operator=		(const Entity& origObj)
		{
			this->position	= origObj.position;
			this->rotation	= origObj.rotation;
			this->front		= origObj.front;
			this->right		= origObj.right;
			this->up		= origObj.up;
			this->buffers	= origObj.buffers;

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
			return this->position;
		}
		vec3 getRotation		()	const  
		{
			return this->rotation;
		}
		vec3 getFront			()	const  
		{
			return this->front;
		}
		vec3 getRight			()	const  
		{
			return this->right;
		}
		vec3 getUp				()	const  
		{
			return this->up;
		}
		IShader* getShader		()
		{
			return this->shader;
		}

		void setPosition		(vec3 _position)
		{
			this->position = _position;
		}
		void setRotation		(vec3 _rotation)
		{
			this->rotation = _rotation;
		}
		void setFront			(vec3 _front)
		{
			this->front = _front;
		}
		void setRight			(vec3 _right)
		{
			this->right = _right;
		}
		void setUp				(vec3 _up)
		{
			this->up = _up;
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
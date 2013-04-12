#ifndef ENTITY_H
#define ENTITY_H

#include "..\Util\GID.h"
#include "BaseBuffer.h"
#include "..\Util\misc.h"
#include "..\Util\SmartPtrs.h"
#include <vector>

namespace Type
{
	const int LIGHT				=  0;
	const int I_DRAWABLE_OBJECT	=  1;
	const int TYPELESS			= -1;
}


class Entity abstract
{
	private:
		int type;
		GID id;

	protected:
		vec3 position;
		vec3 rotation;
		vec3 front;
		vec3 right;
		vec3 up;
		std::vector<BaseBuffer> buffers;

	public:
		Entity(int _type)
			:type(_type)
		{}
		Entity(const Entity& origObj)
		{
			this->position	= origObj.position;
			this->rotation	= origObj.rotation;
			this->front		= origObj.front;
			this->right		= origObj.right;
			this->up		= origObj.up;
			this->buffers	= origObj.buffers;
		}
		virtual~Entity()
		{
			
		}
		Entity& operator=(const Entity& origObj)
		{
			this->position	= origObj.position;
			this->rotation	= origObj.rotation;
			this->front		= origObj.front;
			this->right		= origObj.right;
			this->up		= origObj.up;
			this->buffers	= origObj.buffers;

			return *this;
		}
		bool operator ==(const Entity& object)
		{
			return this->id == object.id;
		}
		bool operator ==(int id)
		{
			return this->id == id;
		}
		bool isOfType(int type)
		{
			return this->type == type;
		}
		bool isOfType(const Entity& object)
		{
			return this->type == object.type;
		}
		
		int getType() const
		{ 
			return this->type; 
		}
		int getID() const
		{
			return this->id;
		}

		vec3 getPosition() const
		{
			return this->position;
		}
		vec3 getRotation()	const  
		{
			return this->rotation;
		}
		vec3 getFront()		const  
		{
			return this->front;
		}
		vec3 getRight()		const  
		{
			return this->right;
		}
		vec3 getUp()		const  
		{
			return this->up;
		}
		std::vector<BaseBuffer>* getBuffers()
		{
			return &this->buffers;
		}

		virtual void Update(){}
		virtual void Render(){}

		
};


#endif
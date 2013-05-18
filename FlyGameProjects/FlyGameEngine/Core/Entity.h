#ifndef ENTITY_H
#define ENTITY_H

#include "..\Util\GID.h"
#include "..\Util\misc.h"
#include "..\Util\SmartPtrs.h"

#include "Mesh\MaterialHandler.h"
#include "IShader.h"
#include "BaseBuffer.h"
#include "..\Util\BoundingVolumes.h"

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
		int				type;
		GID				id;

	protected:

		std::wstring	name;

		Matrix		world;
		Matrix		transformation;
		vec3		rotation;
		vec3		scale;
		vec3		translation;


		ObjectMaterial	*material;
		IShader			*shader;
		SmartPtrStd<BoundingSphere> boundingSphere;
		std::vector<SmartPtrStd<BaseBuffer>> buffers;

	public:
		Entity									(int _type)
			:type(_type), name(L"Entity")
		{}
		Entity									(const Entity& origObj)
		{
			this->world				= origObj.world;
			this->transformation	= origObj.transformation;
			this->rotation			= origObj.rotation;
			this->scale				= origObj.scale;
			this->translation		= origObj.translation;
			this->buffers			= origObj.buffers;
			this->name				= origObj.name;
			this->material			= origObj.material;
			this->shader			= origObj.shader;
			this->boundingSphere	= origObj.boundingSphere;
		}
		virtual~Entity							()
		{}

		Entity&			operator=				(const Entity& origObj)
		{
			this->world				= origObj.world;
			this->transformation	= origObj.transformation;
			this->rotation			= origObj.rotation;
			this->scale				= origObj.scale;
			this->buffers			= origObj.buffers;

			return *this;
		}
		bool			operator ==				(const Entity& object)
		{
			return this->id == object.id;
		}
		bool			operator ==				(int id)
		{
			return this->id == id;
		}
		bool			isOfType				(int type)
		{
			return this->type == type;
		}
		bool			isOfType				(const Entity& object)
		{
			return this->type == object.type;
		}
		
		int				getType					()	const
		{ 
			return this->type; 
		}
		int				getID					()	const
		{
			return this->id;
		}
		vec3			getPosition				()	const
		{
			return this->translation;
		}
		vec3			getRotation				()	const  
		{
			return this->rotation;
		}

		vec3 getScale			() const
		{
			return this->scale;
		}
		vec3 getFront			()	const  

		{
			return vec3(this->transformation.m[2]);
		}
		vec3			getRight				()	const  
		{
			return vec3(this->transformation.m[0]);
		}
		vec3			getUp					()	const  
		{
			return vec3(this->transformation.m[1]);
		}
		Matrix			getWorld				()	const
		{
			return this->world;
		}
		Matrix			getTransformation		()	const
		{
			return this->transformation;
		}
		IShader*		getShader				()
		{
			return this->shader;
		}
		std::wstring	getName					() const
		{
			return this->name;
		}
		BoundingSphere* getBoundingSphere		()
		{
			return this->boundingSphere;
		}
		std::vector<SmartPtrStd<BaseBuffer>>* getBuffers()
		{
			return &this->buffers;
		}

		void			setPosition				(vec3 _position)
		{
			this->translation = _position;
		}
		void			setRotation				(vec3 _rotation)
		{
			this->rotation = _rotation;

			
		}

		void setScale			(vec3 _scale)
		{
			this->scale = _scale;
		}	
		void setFront			(vec3 _front)
		{
			this->transformation._31 = _front.x;
			this->transformation._32 = _front.y;
			this->transformation._33 = _front.z;
		}
		void			setRight				(vec3 _right)
		{
			this->transformation._11 = _right.x;
			this->transformation._12 = _right.y;
			this->transformation._13 = _right.z;
		}
		void			setUp					(vec3 _up)
		{
			this->transformation._21 = _up.x;
			this->transformation._22 = _up.y;
			this->transformation._23 = _up.z;
		}
		void			setShader				(IShader* _shader)
		{
			this->shader = _shader;
		}
		void			setBoundingSphere				(BoundingSphere* _sphere)
		{
			this->boundingSphere = _sphere;
		}
		

		virtual void	Update					()						{  }
		virtual void	Render					(ViewFrustum& frustum)	{  }
		virtual void	Release					()						{  }
		
};


#endif


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
		Matrix			world;

		vec3			translation;
		vec3			rotation;
		vec3			scale;

		ObjectMaterial	*material;
		IShader			*shader;
		SmartPtrStd<BoundingSphere> boundingSphere;
		std::vector<SmartPtrStd<BaseBuffer>> buffers;

	public:
		Entity									(int _type)
			:type(_type), name(L"Entity"), translation(0.0f, 0.0f, 0.0f), scale(1.0f, 1.0f, 1.0f), rotation(0.0f, 0.0f, 0.0f)
		{
			D3DXMatrixIdentity(&world);
		}
		Entity									(const Entity& origObj)
		{
			this->world				= origObj.world;
			this->translation		= origObj.translation;
			this->rotation			= origObj.rotation;
			this->scale				= origObj.scale;
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
			this->translation		= origObj.translation;
			this->scale				= origObj.scale;
			this->rotation			= origObj.rotation;
			this->scale				= origObj.scale;
			this->buffers			= origObj.buffers;
			this->material			= origObj.material;
			this->shader			= origObj.shader;
			this->boundingSphere	= origObj.boundingSphere;
			this->name				= origObj.name;
			this->type				= origObj.type;

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
		vec3			getScale			() const
		{
			return this->scale;
		}
		Matrix			getWorld				()	const
		{
			return this->world;
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
		void			setScale				(vec3 _scale)
		{
			this->scale = _scale;
		}	
		void			setName					(std::wstring _name)
		{
			this->name = _name;
		}
		void			setShader				(IShader* _shader)
		{
			this->shader = _shader;
		}
		void			setBoundingSphere		(BoundingSphere* _sphere)
		{
			this->boundingSphere = _sphere;
		}
		

		virtual void	Update					()						{  }
		virtual void	Render					(ViewFrustum& frustum)	{  }
		virtual void	Release					()						{  }
		
};


#endif


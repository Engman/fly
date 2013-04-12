#ifndef SHADER_HANDLER_H
#define SHADER_HANDLER_H

#include "IShader.h"

/** 
*	Handles all shaders 
*	Implemented as a singleton
*/
class ShaderHandler
{
	private:
		struct _Data;
		_Data* _data;
		std::vector<IShader*> _shaders;

	private:
		ShaderHandler();
		virtual~ShaderHandler();

	public:
		/** Returns an instance of the ShaderHandler */
		static ShaderHandler* self();
		/** Destroys the only instance of the ShaderHandler */
		static void destroy();

		//int addShader(BaseShader::BASE_SHADER_DESC&);
		void drawShaders(IShader::SHADER_PARAMETER_DATA&);
		std::vector<IShader*>* getShaders();
		int getShaderCount() const;

		IShader* getShader(int GID);

		/** The shader description and the shader type to initiate 
		Returns GID if success, else -1 */
		template<typename T>
		IShader* addShader(BaseShader::BASE_SHADER_DESC& desc)  
		{
			IShader* s = new T();
			if(!s->init(desc))
			{
				delete s;
				return NULL;
			}
			 this->_shaders.push_back(s);

			return s;
		}
};


#endif


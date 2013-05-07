#include "ShaderHandler.h"



struct ShaderHandler::_Data
{
	static ShaderHandler* instance;

	_Data()
	{}
	~_Data()
	{}
};

ShaderHandler* ShaderHandler::_Data::instance = NULL;



ShaderHandler::ShaderHandler()
{ 
	this->_data = new _Data(); 
}
ShaderHandler::~ShaderHandler()
{
	delete this->_data;
}

ShaderHandler*  ShaderHandler::self()
{
	if(!ShaderHandler::_Data::instance)
		ShaderHandler::_Data::instance = new ShaderHandler();

	return ShaderHandler::_Data::instance;
}
void ShaderHandler::destroy()			  
{
	delete ShaderHandler::_Data::instance;
	ShaderHandler::_Data::instance = NULL;
}

void ShaderHandler::drawShaders(IShader::PER_FRAME_DATA& data)
{
	for (int i = 0; i < (int)this->_shaders.size(); i++)
		this->_shaders[i]->draw(data);
}

std::vector<IShader*>* ShaderHandler::getShaders()
{
	return &this->_shaders;
}
IShader* ShaderHandler::getShader(int gid)
{
	for (int i = 0; i < (int)this->_shaders.size(); i++)
		if(this->_shaders[i]->getId() == gid)
			return this->_shaders[i];

	return NULL;
}
int ShaderHandler::getShaderCount() const
{
	return (int)this->_shaders.size();
}
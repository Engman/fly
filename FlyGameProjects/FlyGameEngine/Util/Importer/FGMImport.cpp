#include "FGMImport.h"

#include <fstream>
#include <iostream>
#include <string>
#include <ctime>
#include "..\..\Core\Mesh\MaterialHandler.h"
#include "..\..\Core\Mesh\FlyMesh.h"
#include "..\..\Core\D3DShell.h"



#pragma region FROWARD DELERATION

size_t fileSize = 0;
size_t iChar = 0;

bool ParseAnimationFile			(std::string& in, ImportedObjectData* d);
bool ParseStandardFile			(std::string& in, ImportedObjectData* d);

int  ParseMaterial				(std::string& in);

bool ParseV						(std::string& in, ObjectData& o);
bool ParseVT					(std::string& in, ObjectData& o);
bool ParseVN					(std::string& in, ObjectData& o);

bool ParseVector4				(std::string& in, vec4& v);
bool ParseVector3				(std::string& in, vec4& v);
bool ParseVector3				(std::string& in, vec3& v);
bool ParseVector2				(std::string& in, vec2& v);
bool ParseInteger				(std::string& in, int& v);
bool ParseFloat					(std::string& in, float& v);
std::string& ParseLine			(std::string& in, std::string& out, bool trash = false);   


namespace ObjImpFormat
{
	const std::string		comment				= "#";		//# Comment
	const std::string		animationCount		= "ac";		//#  The number of frames in this file
	const std::string		vertexCount			= "vc";		//# The number of frames in this file
	const std::string		material			= "m";		//# material name, meaning a new material
	const std::string		ambient				= "a";		//# ambient
	const std::string		diffuse				= "d";		//# diffuse
	const std::string		specular			= "s";		//# specular
	const std::string		specularPower		= "sp";		//# specular power
	const std::string		ambientTexture		= "ta";		//# texture for ambient
	const std::string		diffuseTexture		= "td";		//# texture for diffuse
	const std::string		specularTexture		= "ts";		//# texture for specular
	const std::string		glowTexture			= "tg";		//# texture for glow
	const std::string		normalTexture		= "tn";		//# texture for normal map
	const std::string		animation			= "an";		//# The animation number (since there will be more than one in a file)
	const std::string		frame				= "fr";		//# Sequence for animation / FlyMesh number / Vertecie count
	const std::string		v					= "v";		//# Position flag
	const std::string		vt					= "vt";		//# UV-coord flag
	const std::string		vn					= "vn";		//# Normal flag
	const std::string		endMaterial			= "endmtl";	//# Material ends
};

#pragma endregion



bool prepearFile	(std::wstring& file, std::string& outData, ImportedObjectData& data)
{
	//Get size of the file
	ifstream fl(file);
	if(!fl.is_open())
		return false;

	fl.seekg( 0, ios::end );  
	size_t len = (size_t)fl.tellg();   
	fl.seekg(0, ios::beg);
	fl.close();
	
	// Convert to a char*
    size_t origsize = wcslen(file.c_str()) + 1;
    const size_t newsize = 100;
    size_t convertedChars = 0;
    char path[newsize];
    wcstombs_s(&convertedChars, path, origsize, file.c_str(), _TRUNCATE);


	size_t first = file.find_last_of('\\') + 1;
	size_t last = file.find_last_of('.');
	data.name = file.substr(first, last-first);

	char *buff = new char[len];

	FILE *stream;
	if( fopen_s( &stream, path, "r+t" ) == 0 )
	{
		fileSize = fread(buff, sizeof( char ), len, stream );
		fclose( stream );
		//buff[fileSize] = '\0';
	}
	else
	{
		std::string msg = "Failed to open file: \n";
		msg.append(path);
		MessageBoxA(0, msg.c_str(), "Import error", 0);
		delete [] buff;
		return false;
	}

	outData = buff;
	outData.resize(fileSize);
	delete [] buff;
	return true;
}
const wchar_t* toW	(const char* c, std::wstring& dummy)
{
	size_t origsize = strlen(c) + 1;
    const size_t newsize = 100;
    size_t convertedChars = 0;
    wchar_t wcstring[newsize];
    mbstowcs_s(&convertedChars, wcstring, origsize, c, _TRUNCATE);
	dummy = wcstring;
	return dummy.c_str();
}
string& read		(const std::string& file, std::string& out)
{
	out = "";
	while (iChar < fileSize&& file[iChar] != ' ' && file[iChar] != '\n' && iChar < fileSize)
	{
		out += file[iChar++];
	}

	//Set at right index
	while (iChar < fileSize&& file[iChar] == ' ' || file[iChar] == '\n')
		iChar++;

	return out;
}
string& readToendl	(const std::string& file, std::string& out)
{
	out = "";
	while (iChar < fileSize && file[iChar] != '\n')
	{
		out += file[iChar++];
	}

	//Set at right index
	while (iChar < fileSize && file[iChar] == '\n')
		iChar++;

	return out;
}
string& readToBlank	(const std::string& file, std::string& out)
{
	out = "";
	while (iChar < fileSize && file[iChar] != ' ')
	{
		out += file[iChar++];
	}
	//Set at right index
	while (iChar < fileSize && file[iChar] == ' ')
		iChar++;

	return out;
}
string& readFlag	(const std::string& file, std::string& out)
{
	out = "";
	while (iChar < fileSize && file[iChar] != ' ' && file[iChar] != '\n')
	{
		out += file[iChar++];
	}
	if(file[iChar] == ' ')
		iChar++;

	return out;
}



bool FGMImport::Import			(std::wstring file, ImportedObjectData* rawData)
{
	fileSize = 0;
	iChar = 0;
	std::string in;
	bool parsing = true;
	std::string part = "";

	if(!prepearFile(file, in, *rawData))
		return false;


	while (iChar != fileSize)
	{
		read(in, part);

		if(part == ObjImpFormat::animationCount) 
		{ 
			int count = 0;
			if(!ParseInteger(in, count))
				return false;
		
			if(count)
			{
				rawData->animations.resize(count);
				if(!ParseAnimationFile(in, rawData))
					return false;
			}
			else		
				if(!ParseStandardFile(in, rawData))
					return false;
		}
		else { ParseLine(in, part, true); }
		
	}

	return true;
}
bool FGMImport::Import			(std::wstring file, __out std::vector<int>* identifiers)
{
	MessageBox(0, L"This method is not implemented!", L"", 0);
	return false;
}

bool ParseAnimationFile			(std::string& in, ImportedObjectData* d)
{
	std::string flag;
	bool result = true;
	int vCount = 0;
	int currFrame = -1;
	int currObject = -1;
	int currAnimation = -1;
	int currMaterial = -1;

	while(iChar != fileSize)
	{
		read(in, flag);

		if(iChar != fileSize)
		{
			if(flag == ObjImpFormat::vertexCount)	
			{
				if(!ParseInteger(in, vCount))	return false;
				if(!vCount)						return false;
			}

			else if(flag == ObjImpFormat::animation)	
			{
				currAnimation++;
				int frameCount = 0;
				if(!ParseInteger(in, d->animations[currAnimation].id))
					return false;
				if(!ParseInteger(in, frameCount))
					return false;

				if(!frameCount)
					return false;

				d->animations[currAnimation].frames.resize(frameCount);
				currFrame = -1;
			}
			else if(flag == ObjImpFormat::frame)
			{
				float frameTime = 0.0f;
				int frameNum = -1;
				if(!ParseInteger(in, frameNum))	return false;
				if(frameNum < 0)				return false;
				if(!ParseFloat(in, frameTime))	return false;
				if(frameTime < 0.0f)			return false;

				d->objects.push_back(ObjectData());
				currObject = (int)d->objects.size()-1;
				currFrame++;

				d->animations[currAnimation].frames[currFrame].frameNumber = frameNum;
				d->animations[currAnimation].frames[currFrame].frameTime = frameTime;
				d->animations[currAnimation].frames[currFrame].objectIndex = currFrame;

				d->objects[currObject].vertex = new std::vector<VERTEX::VertexPNT>(vCount);
				d->objects[currObject].material = currMaterial;
			}
			else if(flag == ObjImpFormat::material)		{ currMaterial = ParseMaterial(in);	}
			else if(flag == ObjImpFormat::v)			{ result = ParseV(in,d->objects[currFrame]); 	}
			else if(flag == ObjImpFormat::vt)			{ result = ParseVT(in,d->objects[currFrame]);	}
			else if(flag == ObjImpFormat::vn)			{ result = ParseVN(in,d->objects[currFrame]);	}
			
			if(!result)
				return result;
		}
	}

	return result;
}
bool ParseStandardFile			(std::string& in, ImportedObjectData* d)
{
	std::string flag;
	bool result = true;
	int safteyCounter = iChar;
	int safteyMax = (int)(fileSize + ((int)fileSize*0.5f));

	while(iChar != fileSize && safteyCounter < (int)fileSize)
	{
		read(in, flag);

		if(iChar != fileSize)
		{
			if(flag == ObjImpFormat::vertexCount)	
			{
				int count = 0;
				if(!ParseInteger(in, count))	return false;
				if(!count)						return false;
				
				//No animations, then only one mesh is valid
				d->objects.resize(1);
				d->objects[0].vertex = new std::vector<VERTEX::VertexPNT>(count);
				d->objects[0].material = 0;
			}
			else if(flag == ObjImpFormat::material)		{ d->objects[0].material = ParseMaterial(in); }
			else if(flag == ObjImpFormat::v)			{ result = ParseV(in,d->objects[0]);  }
			else if(flag == ObjImpFormat::vt)			{ result = ParseVT(in,d->objects[0]); }
			else if(flag == ObjImpFormat::vn)			{ result = ParseVN(in,d->objects[0]); }
			
			if(!result)
				return result;
		}
		safteyCounter++;
	}

	return result;
}

int ParseMaterial				(std::string& in)			
{
	ObjectMaterial::OBJECT_MATERIAL_DESC md;
	md.device = D3DShell::self()->getDevice();
	md.dc = D3DShell::self()->getDeviceContext();
	std::string buff = ""; 
	std::wstring wdummy = L"";
	md.name = toW(ParseLine(in, buff).c_str(), wdummy);

	bool done = false;
	int safteyCounter = iChar;

	do
	{
		//Get flag
		//readToBlank(in, buff);
		readFlag(in, buff);
		if(buff.size())
		{
				 if(buff		== ObjImpFormat::ambient)			{ ParseVector4(in, md.ambient); }
			else if(buff		== ObjImpFormat::diffuse)			{ ParseVector4(in, md.diffuse); }
			else if(buff		== ObjImpFormat::specular)			{ ParseVector4(in, md.specular); }
			else if(buff		== ObjImpFormat::specularPower)		{ ParseInteger(in, md.specualarPow); }
			else if(buff		== ObjImpFormat::ambientTexture)	{ md.ambientTexture		= toW(readToendl(in, buff).c_str(), wdummy);  }
			else if(buff		== ObjImpFormat::diffuseTexture)	{ md.diffuseTexture		= toW(readToendl(in, buff).c_str(), wdummy);  }
			else if(buff		== ObjImpFormat::specularTexture)	{ md.specularTexture	= toW(readToendl(in, buff).c_str(), wdummy);  }
			else if(buff		== ObjImpFormat::glowTexture)		{ md.glowTexture		= toW(readToendl(in, buff).c_str(), wdummy);  }
			else if(buff		== ObjImpFormat::normalTexture)		{ md.normalTexture		= toW(readToendl(in, buff).c_str(), wdummy);  }
			else if(buff		== ObjImpFormat::comment)			{ ParseLine(in, buff, true); }
			else if(buff		== ObjImpFormat::endMaterial)
			{
				done = true;

				//Return newly created material id
				return MaterialHandler::AddMaterial(md);
			}
		}
		safteyCounter++;
	} while (iChar < fileSize && !done && safteyCounter < (int)fileSize);

	return -1;
}	


bool ParseV						(std::string& in, ObjectData& o)			
{
	int total = (int)o.vertex->size();
	int i = 0;

	while (i < total)
	{
		if(!ParseVector3(in, (*o.vertex)[i++].position)) 
			return false;
		(*o.vertex)[i-1].position.w = 1.0f;
	}

	return true;
}
bool ParseVT					(std::string& in, ObjectData& o)	
{
	int total = (int)o.vertex->size();
	int i = 0;

	while (i < total)
	{
		if(!ParseVector2(in, (*o.vertex)[i++].texcoord))
			return false;
	}
	
	return true;
}
bool ParseVN					(std::string& in, ObjectData& o)				
{
	int total = (int)o.vertex->size();
	int i = 0;

	while (i < total)
	{
		if(!ParseVector3(in, (*o.vertex)[i++].normal))	
			return false;
		(*o.vertex)[i-1].normal.w = 0.0f;
	}
	
	return true;
}


bool ParseVector4				(std::string& in, vec4& v)				
{
	if(!ParseFloat(in, v.x)) 
		return false;
	if(!ParseFloat(in, v.y)) 
		return false;
	if(!ParseFloat(in, v.z)) 
		return false;
	if(!ParseFloat(in, v.w)) 
		return false;
	return true;
}
bool ParseVector3				(std::string& in, vec3& v)				
{
	if(!ParseFloat(in, v.x)) 
		return false;
	if(!ParseFloat(in, v.y)) 
		return false;
	if(!ParseFloat(in, v.z)) 
		return false;
	return true;
}
bool ParseVector3				(std::string& in, vec4& v)				
{
	if(!ParseFloat(in, v.x)) 
		return false;
	if(!ParseFloat(in, v.y)) 
		return false;
	if(!ParseFloat(in, v.z)) 
		return false;
	v.w = 1.0f;
	return true;
}
bool ParseVector2				(std::string& in, vec2& v)				
{
	if(!ParseFloat(in, v.x)) 
		return false;
	if(!ParseFloat(in, v.y)) 
		return false;
	return true;
}
bool ParseInteger				(std::string& in, int& v)
{
	std::string temp = "";
	read(in, temp).c_str();
	if(!isdigit(temp[0]) && temp[0] == '-')	
		return false;

	v = atoi(temp.c_str());

	return true;
}
bool ParseFloat					(std::string& in, float& v)
{
	std::string temp = "";
	read(in, temp).c_str();
	if(!isdigit(temp[0]) && temp[0] != '-')	
		return false;

	v = (float)atof(temp.c_str());

	return true;
}
std::string& ParseLine			(std::string& in, std::string& out, bool trash)		   
{
	out = "";
	if(trash)
	{
		while (in[iChar] != '\n')
		{
			iChar++;
		}
		iChar++;
	}
	else
	{
		int first = iChar;
		while (in[iChar] != '\n')
		{
			iChar++;
		}
		out = in.substr(first, iChar-first);
		iChar++;
	}
	return out;
}



#include "ObjectImporter.h"

#include <fstream>
#include <iostream>
#include <string>
#include "..\..\Core\Mesh\MaterialHandler.h"
#include "..\..\Core\Mesh\FlyMesh.h"
#include "..\..\Core\D3DShell.h"



#pragma region FROWARD DELERATION

bool ParseAnimationFile			(std::wifstream& in, ImportedObjectData* d);
bool ParseStandardFile			(std::wifstream& in, ImportedObjectData* d);

int  ParseMaterial				(std::wifstream& in);

bool ParseV						(std::wifstream& in, ObjectData& o);
bool ParseVT					(std::wifstream& in, ObjectData& o);
bool ParseVN					(std::wifstream& in, ObjectData& o);

bool ParseVector4				(std::wifstream& in, vec4& v);
bool ParseVector3				(std::wifstream& in, vec4& v);
bool ParseVector3				(std::wifstream& in, vec3& v);
bool ParseVector2				(std::wifstream& in, vec2& v);
bool ParseInteger				(std::wifstream& in, int& v);
bool ParseFloat					(std::wifstream& in, float& v);
std::wstring ParseLine			(std::wifstream& in, bool trash = false);


namespace ObjImpFormat
{
	const std::wstring		comment				= L"#";			//Comment
	const std::wstring		animationCount		= L"ac";		//#The number of frames in this file
	const std::wstring		vertexCount			= L"vc";		//#The number of frames in this file
	const std::wstring		material			= L"m";			//#material name, meaning a new material
	const std::wstring		ambient				= L"a";			//#ambient
	const std::wstring		diffuse				= L"d";			//#diffuse
	const std::wstring		specular			= L"s";			//#specular
	const std::wstring		specularPower		= L"sp";		//#specular power
	const std::wstring		ambientTexture		= L"ta";		//#texture for ambient
	const std::wstring		diffuseTexture		= L"td";		//#texture for diffuse
	const std::wstring		specularTexture		= L"ts";		//#texture for specular
	const std::wstring		glowTexture			= L"tg";		//#texture for glow
	const std::wstring		normalTexture		= L"tn";		//#texture for normal map
	const std::wstring		animation			= L"an";		//#The animation number (since there will be more than one in a file)
	const std::wstring		frame				= L"fr";		//#Sequence for animation / FlyMesh number / Vertecie count
	const std::wstring		v					= L"v";			//#Position flag
	const std::wstring		vt					= L"vt";		//#UV-coord flag
	const std::wstring		vn					= L"vn";		//#Normal flag
	const std::wstring		endMaterial			= L"endmtl";	//Material ends
};

#pragma endregion



bool ObjectImporter::Import(std::wstring file, ImportedObjectData* rawData)
{
	size_t first = file.find_last_of('\\') + 1;
	size_t last = file.find_last_of('.');
	rawData->name = file.substr(first, last-first);
	

	std::wifstream in (file);
	if(!in.is_open())
	{
		std::wstring msg = L"Failed to open file: \n";
		msg.append(file);
		MessageBox(0, msg.c_str(), L"Import error", 0);
		return false;
	}
	else
	{
		std::wstring buff;

		while (!in.eof())
		{
			in >> buff;

			if(buff.size())
			{
				if(buff == ObjImpFormat::animationCount) 
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
				else { ParseLine(in, true); }
			}
		}

		in.close();
	}

	return true;
}
bool ObjectImporter::Import		(std::wstring file, __out std::vector<int>* identifiers)
{
	if(file == L"")
		return false;

	std::wifstream in (file);
	
	if(!in.is_open())
		return false;

	std::wstring buff;
	int mid = -1;

	while (!in.eof())
	{
		in >> buff;

		if(buff == ObjImpFormat::material)
		{
			mid = ParseMaterial(in);
			if(mid != -1)
			{
				identifiers->push_back(mid);
			}
		}
		in.close();
	}

	return true;
}

bool ParseAnimationFile			(std::wifstream& in, ImportedObjectData* d)
{
	std::wstring flag;
	bool result = true;
	int vCount = 0;
	int currFrame = -1;
	int currObject = -1;
	int currAnimation = -1;
	int currMaterial = -1;
	int totMeshCount = -1;

	while(!in.eof())
	{
		in >> flag;

		if(in.eof())
			break;

		if(flag == ObjImpFormat::vertexCount)	
		{
			if(!ParseInteger(in, vCount))
			{
				DisplayText("File:\n" + in.getloc().name() + "\nDoes not contain any vertex information");
				return false;
			}
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
			if(!ParseInteger(in, frameNum))
				return false;
			if(!ParseFloat(in, frameTime))
			{
				char temp1[2];
				_itoa_s(frameNum, temp1, 10);
				DisplayText("File:\n" + in.getloc().name() + "\nFrame: " + temp1 + "\nInvalid frame time");
				return false;
			}

			if(frameTime < 0.0f)
				return false;

			d->objects.push_back(ObjectData());
			currObject = (int)d->objects.size()-1;
			currFrame++;
			totMeshCount++;

			d->animations[currAnimation].frames[currFrame].frameNumber = frameNum;
			d->animations[currAnimation].frames[currFrame].frameTime = frameTime;
			d->animations[currAnimation].frames[currFrame].objectIndex = currFrame;

			d->objects[totMeshCount].vertex = new std::vector<VERTEX::VertexPNT>(vCount);
			d->objects[totMeshCount].material = currMaterial;
		}
		else if(flag == ObjImpFormat::material)		{ currMaterial = ParseMaterial(in);	}
		else if(flag == ObjImpFormat::v)			{ result = ParseV(in,d->objects[totMeshCount]); 	}
		else if(flag == ObjImpFormat::vt)			{ result = ParseVT(in,d->objects[totMeshCount]);	}
		else if(flag == ObjImpFormat::vn)			
		{ result = ParseVN(in,d->objects[totMeshCount]);	}
		else if(flag == ObjImpFormat::comment)		{ ParseLine(in, true); }
		
		if(!result)
			return result;
	}

	return result;
}
bool ParseStandardFile			(std::wifstream& in, ImportedObjectData* d)
{
	std::wstring flag;
	bool result = true;
	while(!in.eof())
	{
		in >> flag;
		if(!in.eof())
		{
			if(flag == ObjImpFormat::vertexCount)	
			{
				int count = 0;
				if(!ParseInteger(in, count))
				{
					DisplayText("File:\n" + in.getloc().name() + "\nDoes not contain any vertex information");
					return false;
				}
				else if(!count)
				{
					DisplayText("File:\n" + in.getloc().name() + "\nHas invalid vertex count (0)");
					return false;
				}

				//No animations, then only one mesh is valid
				d->objects.resize(1);
				d->objects[0].vertex = new std::vector<VERTEX::VertexPNT>(count);
				d->objects[0].material = 0;
			}
			else if(flag == ObjImpFormat::material)		{ d->objects[0].material = ParseMaterial(in); }
			else if(flag == ObjImpFormat::v)			{ result = ParseV(in,d->objects[0]);  }
			else if(flag == ObjImpFormat::vt)			{ result = ParseVT(in,d->objects[0]); }
			else if(flag == ObjImpFormat::vn)			{ result = ParseVN(in,d->objects[0]); }
			else if(flag == ObjImpFormat::comment)		{ ParseLine(in, true); }
			
			if(!result)
				return result;
		}
	}

	return result;
}

int ParseMaterial				(std::wifstream& in)			
{
	
	ObjectMaterial::OBJECT_MATERIAL_DESC md;
	md.device = D3DShell::self()->getDevice();
	md.dc = D3DShell::self()->getDeviceContext();
	md.name = ParseLine(in);
	std::wstring buffer = L"";
	bool done = false;
	std::vector<std::streamoff> val;
	do
	{
		in >> buffer;
		if(buffer.size())
		{
				 if(buffer		== ObjImpFormat::ambient)			{ ParseVector4(in, md.ambient); }
			else if(buffer		== ObjImpFormat::diffuse)			{ ParseVector4(in, md.diffuse); }
			else if(buffer		== ObjImpFormat::specular)			{ ParseVector4(in, md.specular); }
			else if(buffer		== ObjImpFormat::specularPower)		{ ParseInteger(in, md.specualarPow); }
			else if(buffer		== ObjImpFormat::ambientTexture)	{ md.ambientTexture		= ParseLine(in); }
			else if(buffer		== ObjImpFormat::diffuseTexture)	{ md.diffuseTexture		= ParseLine(in);  }
			else if(buffer		== ObjImpFormat::specularTexture)	{ md.specularTexture	= ParseLine(in);  }
			else if(buffer		== ObjImpFormat::glowTexture)		{ md.glowTexture		= ParseLine(in);  }
			else if(buffer		== ObjImpFormat::normalTexture)		{ md.normalTexture		= ParseLine(in);  }
			else if(buffer		== ObjImpFormat::comment)			{ ParseLine(in, true); }
			else if(buffer		== ObjImpFormat::endMaterial)
			{
				done = true;

				//Return newly created material id
				return MaterialHandler::AddMaterial(md);
			}
		}
	
	} while (!done || !in.eof());

	return -1;
}	


bool ParseV						(std::wifstream& in, ObjectData& o)			
{
	int total = (int)o.vertex->size();
	int i = 0;

	while (i < total)
	{
		(*o.vertex)[i].position.w = 1.0f;
		if(!ParseVector3(in, (*o.vertex)[i++].position))
			return false;
	}

	return true;
}
bool ParseVT					(std::wifstream& in, ObjectData& o)	
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
bool ParseVN					(std::wifstream& in, ObjectData& o)				
{
	int total = (int)o.vertex->size();
	int i = 0;

	while (i < total)
	{
		(*o.vertex)[i].normal.w = 0.0f;
		if(!ParseVector3(in, (*o.vertex)[i++].normal))
			return false;
	}
	
	return true;
}


bool ParseVector4				(std::wifstream& in, vec4& v)				
{
	std::wstring buff;
	in >> buff;
	if(!isdigit(buff[0]) && buff[0] != '-')	return false;
	v.x = (FLOAT)_wtof(buff.c_str());
	in >> buff;
	if(!isdigit(buff[0]) && buff[0] != '-')	return false;
	v.y = (FLOAT)_wtof(buff.c_str());
	in >> buff;
	if(!isdigit(buff[0]) && buff[0] != '-')	return false;
	v.z = (FLOAT)_wtof(buff.c_str());
	in >> buff;
	if(!isdigit(buff[0]) && buff[0] != '-')	return false;
	v.w = (FLOAT)_wtof(buff.c_str());

	return true;
}
bool ParseVector3				(std::wifstream& in, vec3& v)				
{
	std::wstring buff;

	in >> buff;
	if(!isdigit(buff[0]) && buff[0] != '-')	return false;
	v.x = (FLOAT)_wtof(buff.c_str());
	in >> buff;
	if(!isdigit(buff[0]) && buff[0] != '-')	return false;
	v.y = (FLOAT)_wtof(buff.c_str());
	in >> buff;
	if(!isdigit(buff[0]) && buff[0] != '-')	return false;
	v.z = (FLOAT)_wtof(buff.c_str());

	return true;
}
bool ParseVector3				(std::wifstream& in, vec4& v)				
{
	std::wstring buff;

	in >> buff;
	if(buff.size())
		if(!isdigit(buff[0]) && buff[0] != '-')	return false;
		v.x = (FLOAT)_wtof(buff.c_str());
	in >> buff;
	if(buff.size())
		if(!isdigit(buff[0]) && buff[0] != '-')	return false;
		v.y = (FLOAT)_wtof(buff.c_str());
	in >> buff;
	if(buff.size())
		if(!isdigit(buff[0]) && buff[0] != '-')	return false;
		v.z = (FLOAT)_wtof(buff.c_str());

	return true;
}
bool ParseVector2				(std::wifstream& in, vec2& v)				
{
	std::wstring buff;
	in >> buff;
	if(!isdigit(buff[0]) && buff[0] != '-')	return false;
	v.x = (FLOAT)_wtof(buff.c_str());
	in >> buff;
	if(!isdigit(buff[0]) && buff[0] != '-')	return false;
	v.y = (FLOAT)_wtof(buff.c_str());

	return true;
}
bool ParseInteger				(std::wifstream& in, int& v)
{
	std::wstring buff;
	in >> buff;

	if(!isdigit(buff[0]) && buff[0] == '-')	return false;
	v = _wtoi(buff.c_str());

	return true;
}
bool ParseFloat					(std::wifstream& in, float& v)
{
	std::wstring buff;
	in >> buff;

	if(!isdigit(buff[0]) && buff[0] == '-')	return false;
	v = (float)_wtof(buff.c_str());

	return true;
}
std::wstring ParseLine			(std::wifstream& in, bool trash)		   
{
	wchar_t line[255];
	in.getline(line, 255, '\n');
	std::wstring text = line;
	if(text == L" ")
		return L"";
	if(!trash)
	{
		//Find junk stuff, we are only interested in "real" data
		int first = 0;
		int tab = (int)text.find_first_of('\t');
		int comment = (int)text.find_first_of(ObjImpFormat::comment);

		if(tab != -1)
		{
			if(tab < comment || comment == -1)
				text.erase(tab, text.back());
			else
				text.erase(comment, text.back());
		}
		else if(comment != -1)
			text.erase(comment, text.back());
		

		int left = 0;
		int right = (int)text.length()-1;
		while(left < right && text[left] == ' ')
			left++;
		while(right > left && text[right] == ' ')
			right--;
			
		
		if(left > 0 && left < (int)text.length() - 1)
			text.erase(0, left);
		if(right > 0 && right < (int)text.length() - 1)
			text.erase(right+1, text.length() - 1);
	}

	return text;
}

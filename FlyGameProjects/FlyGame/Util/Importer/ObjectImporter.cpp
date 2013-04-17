#include "ObjectImporter.h"

#include <fstream>
#include <iostream>
#include <string>



namespace ObjImpFormat
{
	const std::wstring	name			= L"n";					//Name of the mesh collection
	const wchar_t		comment			= '#';					//A simple comment
	const std::wstring	count			= L"c";					//The number of frames in this file
	const std::wstring	material		= L"m";					//material name
	const std::wstring	ambient			= L"a";					//ambient
	const std::wstring	diffuse			= L"d";					//diffuse
	const std::wstring	specluar		= L"s";					//specular
	const std::wstring	specularPower	= L"sp";				//specular power
	const std::wstring	textureAmbient	= L"ta";				//texture for ambient
	const std::wstring	textureDiffuse	= L"td";				//texture for diffuse
	const std::wstring	textureSpecular	= L"ts";				//texture for specular
	const std::wstring	textureGlow		= L"tg";				//texture for glow
	const std::wstring	textureNormal	= L"tn";				//texture for normal map
	const std::wstring	frame			= L"fr";				//Sequence for animation / Object number
	const std::wstring	v				= L"v";					//Position flag and number of positions below
	const std::wstring	vt				= L"vt";				//UV-coord flag and number of uv coordinates below
	const std::wstring	vn				= L"vn";				//Normal flag and number of normals below
	const std::wstring	f				= L"f";					//Face flag and number of faces below
	const std::wstring	mend			= L"mend";				//The end of materials
};


bool ObjectImporter::Import(wchar_t* file, SmartPtrStd<ImportedObjectData>& rawData)
{
	if(!rawData.IsValid())
		rawData = new ImportedObjectData();


	std::wifstream in (file);
	if(!in.is_open())
	{
#if defined(_DEBUG) || defined(DEBUG)
		std::wstring msg = L"Failed to open file: \n";
		msg.append(file);
		msg.append(L"\nIn function [bool ObjectImporter::Import(wchar_t* file, SmartPtrStd<RawObjectData> rawData)]");
		MessageBox(0, msg.c_str(), L"Import error", 0);
		return false;
#else
		throw std::exception("Error");
		return false;
#endif
	}
	else
	{
		std::wstring buffer;
		int frame = 0;
		RawVertecieData rawVert;
		RawMaterialData rawMat;
		while (!in.eof())
		{
			in >> buffer;

				 if(buffer		== ObjImpFormat::count)			{ ParseMeshCount(in, rawData); }
			else if(buffer		== ObjImpFormat::frame)			{ in >> frame; }
			else if(buffer[0]	== ObjImpFormat::comment)		{ ParseText			(in, true);		}
			else if(buffer		== ObjImpFormat::material)		{ ParseMaterial		(rawMat, in);  }
			else if(buffer		== ObjImpFormat::v)				{ ParsePositions	(rawVert, in, frame);  }
			else if(buffer		== ObjImpFormat::vt)			{ ParseUVs			(rawVert, in, frame);  }
			else if(buffer		== ObjImpFormat::vn)			{ ParseNormals		(rawVert, in, frame);  }
			else if(buffer		== ObjImpFormat::f)				{ ParseFace			(rawVert, in, frame, rawData);  }
			else if(buffer		== ObjImpFormat::name)			{ rawData->name = ParseText(in);	}

		}
	}

	return true;
}


void ObjectImporter::ParseUVs(RawVertecieData& rawVert, std::wifstream& in, int frame)
{
	int count = 0;
	in >> count;

	if(count > 0)
	{
		rawVert.textCoord.resize(count);
		vec2 uv;
		std::wstring buff;
		int i = 0;
		while (i < count)
		{
			in >> buff;
			if( buff[0] == ObjImpFormat::comment)
				ParseText(in, true);
			else
			{
				uv.x = (float)_wtof(buff.c_str());
				in >> buff;
				uv.y = 1.0f - (float)_wtof(buff.c_str());

				rawVert.textCoord[i++] = uv;
			}
		}
	}
}
void ObjectImporter::ParsePositions(RawVertecieData& rawVert, std::wifstream& in, int frame)
{
	int count = 0;
	in >> count;

	if(count > 0)
	{
		rawVert.position.resize(count);
		vec4 p;
		std::wstring buff;
		p.w = 1.0f;
		int i = 0;
		while (i < count)
		{
			in >> buff;
			if( buff[0] == ObjImpFormat::comment)
				ParseText(in, true);
			else
			{
				p.x = (float)_wtof(buff.c_str());
				in >> buff;
				p.y = (float)_wtof(buff.c_str());
				in >> buff;
				p.z = (float)_wtof(buff.c_str());

				rawVert.position[i++] = p;
			}
		}
	}
}
void ObjectImporter::ParseNormals(RawVertecieData& rawVert, std::wifstream& in, int frame)
{
	int count = 0;
	in >> count;

	if(count > 0)
	{
		rawVert.normal.resize(count);
		vec4 p;
		std::wstring buff;
		p.w = 0.0f;
		int i = 0;
		while (i < count)
		{
			in >> buff;
			if( buff[0] == ObjImpFormat::comment)
				ParseText(in, true);
			else
			{
				p.x = (float)_wtof(buff.c_str());
				in >> buff;
				p.y = (float)_wtof(buff.c_str());
				in >> buff;
				p.z = (float)_wtof(buff.c_str());

				rawVert.normal[i++] = p;
			}
		}
	}
}
void ObjectImporter::ParseFace(RawVertecieData& rawVert, std::wifstream& in, int frame, ImportedObjectData* rawData)		
{
	int count = 0;
	in >> count;

	if(count > 0)
	{
		rawData->MeshData[frame].indecies.resize(count*3);
		rawData->MeshData[frame].vertex.resize(rawVert.position.size());

		int i = 0, c = 0;
		std::wstring buff;
		bool comment = true;
		do
		{
			buff = ParseText(in);
			if(buff.size())
				if(buff[0] != ObjImpFormat::comment)
					comment = false;

		} while (comment);

		//For all faces
		for(int q = 0; q<count; q++)
		{
			//For each face
			std::vector<std::wstring> f = stringSplit(buff,' ');

			//For each vertex
			for (int k = 0; k < (int)f.size(); k++)
			{
				std::vector<std::wstring> v = stringSplit(f[k], '/');

				if(rawVert.position.size())
					i = (int)_wtoi(v[0].c_str());
					rawData->MeshData[frame].vertex[i].position = rawVert.position[i];

				if(rawVert.textCoord.size())
				{
					i = (int)_wtoi(v[1].c_str());
					rawData->MeshData[frame].vertex[i].texcoord = rawVert.textCoord[i];

					if(rawVert.normal.size())
						i = (int)_wtoi(v[2].c_str());
						rawData->MeshData[frame].vertex[i].normal = rawVert.normal[i];
				}	
				else
					if(rawVert.normal.size())
						i = (int)_wtoi(v[1].c_str());
						rawData->MeshData[frame].vertex[i].normal = rawVert.normal[i];

				rawData->MeshData[frame].indecies[c++] = i;
			}
			
			buff = ParseText(in);
		}
	}
}
void ObjectImporter::ParseMaterial(RawMaterialData& rawData, std::wifstream& in)
{
	//static int materialCount = 0;
	//rawData->materialData.push_back(RawMaterialData());
	//materialCount ++;
	//
	//in >> rawData->materialData[materialCount-1] .name;
	//
	//std::wstring buffer = L"";
	//bool done = false;
	//do
	//{
	//	in >> buffer;
	//	if(buffer.size())
	//	{
	//			 if(buffer		== ObjImpFormat::ambient)			{ ParseVector4(in, rawData->materialData[materialCount-1].ambient); }
	//		else if(buffer[0]	== ObjImpFormat::comment)			{ ParseText(in, true); }
	//		else if(buffer		== ObjImpFormat::diffuse)			{ ParseVector4(in, rawData->materialData[materialCount-1].diffuse); }
	//		else if(buffer		== ObjImpFormat::specluar)			{ ParseVector4(in, rawData->materialData[materialCount-1].specular); }
	//		else if(buffer		== ObjImpFormat::specularPower)		{ rawData->materialData[materialCount-1].specualrPow = _wtoi(ParseText(in).c_str()); }
	//		else if(buffer		== ObjImpFormat::textureAmbient)	{ rawData->materialData[materialCount-1].ambientTexture = ParseText(in); }
	//		else if(buffer		== ObjImpFormat::textureDiffuse)	{ rawData->materialData[materialCount-1].diffuseTexture = ParseText(in);  }
	//		else if(buffer		== ObjImpFormat::textureSpecular)	{ rawData->materialData[materialCount-1].specularTexture = ParseText(in);  }
	//		else if(buffer		== ObjImpFormat::textureGlow)		{ rawData->materialData[materialCount-1].glowTexture = ParseText(in);  }
	//		else if(buffer		== ObjImpFormat::textureNormal)		{ rawData->materialData[materialCount-1].normalTexture = ParseText(in);  }
	//		else 
	//		{
	//			//Move stream pointer back
				//in.unget();
	//			done = true;
	//		}
	//	}
	//
	//} while (!done);
}
std::wstring ObjectImporter::ParseText(std::wifstream& in, bool trash)		   
{
	wchar_t line[255];
	in.getline(line, 255, '\n');
	std::wstring text = line;
	
	if(!trash)
	{
		//Find junk stuff, we are only interested in "real" data
		int a = 0;
		int tab = text.find_first_of('\t');
		int comment = text.find_first_of(ObjImpFormat::comment);

		if(tab > comment)
		{
			//Comment
			text.erase(comment, text.back());
		}
		else if(comment > tab)
		{
			text.erase(tab, text.back());
		}

		//Remove junk, aka spacing
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
void ObjectImporter::ParseVector4(std::wifstream& in, vec4& v)
{
	std::wstring buff;
	in >> buff;
	v.x = (FLOAT)_wtof(buff.c_str());
	in >> buff;
	v.y = (FLOAT)_wtof(buff.c_str());
	in >> buff;
	v.z = (FLOAT)_wtof(buff.c_str());
	in >> buff;
	v.w = (FLOAT)_wtof(buff.c_str());
}
int ObjectImporter::ParseMeshCount(std::wifstream& in, ImportedObjectData* rawData)
{
	int count = 0;
	in >> count;

	if(count <= 0)
	{
#if defined(_DEBUG) || defined(DEBUG)
		MessageBox(0, L"Failed to parse mesh count in function:\n[void ObjectImporter::ParseMeshCount(std::wifstream& in, int &count)]", L"Size error", 0);
		return 0;
#else
		return 0;
#endif
	}

	rawData->MeshData.resize(count);

	return true;
}
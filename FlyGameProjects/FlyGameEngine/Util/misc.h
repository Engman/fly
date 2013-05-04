#ifndef MISC_H
#define MISC_H

#include <D3DX11\D3D10_1.h>
#include <D3DX11\D3DX10math.h>
#include <vector>
#include <string>


typedef D3DXVECTOR2 vec2;
typedef D3DXVECTOR3 vec3;
typedef D3DXVECTOR4 vec4;
typedef D3DXMATRIX Matrix;


struct Point
{
	int x;
	int y;

	Point()
		:x(0), y(0)
	{}
	Point(int w)
		:x(w), y(w)
	{}
	Point(int _x, int _y)
		:x(_x), y(_y)
	{}
	Point(const Point& obj)
	{
		this->x = obj.x;
		this->y = obj.y;
	}
	Point& operator=(const Point& obj)
	{
		this->x = obj.x;
		this->y = obj.y;

		return *this;
	}
};

static void DisplayText(std::wstring text, std::wstring message = L"Error")
{
	MessageBoxW(0, text.c_str(), message.c_str(), 0);
}
static void DisplayText(std::string text, std::string message = "Error")
{
	MessageBoxA(0, text.c_str(), message.c_str(), 0);
}

static std::vector<std::wstring> stringSplit(std::wstring& p_sstring, char p_cDel, bool ignoreEmpty = true) 
{
	std::vector<std::wstring> parts;

	int start = 0;
	int endIndex = (int)p_sstring.length()-1;
  	int charCount = 0;
	std::wstring temp = L"";

	int k = 0;
	for(int i = 0; i <= endIndex; i)
	{
		for(k = i; k <= endIndex && p_sstring[k] != p_cDel; k++)
		{
			temp += p_sstring[k];
		}
		if(temp != L"" || !ignoreEmpty)
		{
			parts.push_back(temp);
			temp = L"";
		}
		i = k+1;
	}

	return parts;
}
static std::vector<std::string> stringSplit(std::string& p_sstring, char p_cDel, bool ignoreEmpty = true) 
{
	std::vector<std::string> parts;

	int start = 0;
	int endIndex = (int)p_sstring.length()-1;
  	int charCount = 0;
	std::string temp = "";

	int k = 0;
	for(int i = 0; i <= endIndex; i)
	{
		for(k = i; k <= endIndex && p_sstring[k] != p_cDel; k++)
		{
			temp += p_sstring[k];
		}
		if(temp != "" || !ignoreEmpty)
		{
			parts.push_back(temp);
			temp = "";
		}
		i = k+1;
	}

	return parts;
}


#endif
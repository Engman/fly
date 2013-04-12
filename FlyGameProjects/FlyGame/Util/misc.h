#ifndef MISC_H
#define MISC_H

#include <D3DX11\D3D10_1.h>
#include <D3DX11\D3DX10math.h>

#define DLLExport	__declspec(dllexport)
#define DLLImport	__declspec(dllimport)


#define FIND_MEMORY_LEAKS _CrtSetDbgFlag( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF)
#define DELETE_COM(x) if( x ) { (x)->Release(); (x) = NULL; }
#define DELETE_PTR(x) if( x ) { delete(x); (x) = NULL; }
#define DELETE_ARRAY(x) if( x ) { delete[](x); (x) = NULL; }


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


#endif
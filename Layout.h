#pragma once
#pragma comment ( lib, "D3D11.lib")
#pragma comment ( lib,"D3DX11.lib")

#include<D3D11.h>
#include<D3DX11.h>
#include<xnamath.h>

struct Position
{
	float x, y, z, w;
};

struct Normal
{
	float x, y, z;
};

struct Texcoord
{
	float u, v;
};



struct IndexType
{
	int vertex;
	int normal;
	int tex;
};


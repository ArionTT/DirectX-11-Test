#pragma once

#pragma comment ( lib,"D3DX11.lib")
#pragma comment ( lib, "D3D11.lib")

#include<D3D11.h>
#include<d3dx11.h>
#include<xnamath.h>

class Light
{
public:
	Light() {}
	~Light() {}
	virtual void SetSpecularColor(XMFLOAT4 color) { this->Specular = color; }
	virtual void SetLightPos(XMFLOAT3 pos) { this->Pos = pos; }

	virtual XMFLOAT4 GetSpecular() { return Specular; }
	virtual XMFLOAT4 GetAmbient() { return Ambient; }
	virtual XMFLOAT4 GetDiffuse() { return Diffuse; }

public:
	XMFLOAT4  Ambient;
	XMFLOAT4 Diffuse;
	XMFLOAT4 Specular;

	XMFLOAT3 Pos;
};

class PointLight :public Light
{
public :
	PointLight() {}
	~PointLight() {}
	void SetSpecularColor(XMFLOAT4 color) { this->Specular = color; }
	void SetLightPos(XMFLOAT3 pos) { this->Pos = pos; }
	void SetRange(float range) { this->range = range; }
	void SetAttention(XMFLOAT3 att) { this->att = att; }

	FLOAT GetRange() { return range; }
	XMFLOAT3 GetAttention() { return att; }

public:
	FLOAT range;
	XMFLOAT3 att;
	float unused;     //用于与HLSL中"4D向量"对齐规则匹配  
};

class DirLight :public Light
{
public:
	DirLight() {}
	~DirLight() {}
	void SetSpecularColor(XMFLOAT4 color) { this->Specular = color; }
	void SetLightPos(XMFLOAT3 pos) { this->Pos = pos; }
	void SetDirection(XMFLOAT3 dir) { this->dir = dir; }

	XMFLOAT3 GetDirection() { return dir; }

public:
	XMFLOAT3 dir;
	float unused;     //用于与HLSL中"4D向量"对齐规则匹配  
};


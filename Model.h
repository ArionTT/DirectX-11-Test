#pragma once
#include"Mesh2.h"

//Éºº÷ºì
const float defaultMatColor[4] = { 1.0f, 0.498f, 0.3137f, 1.0f };
//°×¹â
const float defaultLightColor[4] = { 0.9f,0.9f,0.9f,1.0f };
const float defaultLigtPos[3] = { 100.0f,100.0f,100.0f };

struct Material
{
	FLOAT Glossness;
	float order;
	float order1;
	float order2;
	XMFLOAT4 MatColor;
	//Texture2D tex;
};

struct ConstanceBuffer
{
	XMMATRIX world;
	XMMATRIX view;
	XMMATRIX projection;
};

struct LightConst
{
	XMFLOAT4 LightColor;
	XMFLOAT3 LightPos;
	float order;
	XMFLOAT3 LightDir;
	float order2;
	XMFLOAT3 ViewPos;
	float order3;
};

class Model
{
public:
	Model();
	~Model();
	HRESULT Initialize(std::string filename,HWND hwnd,ID3D11Device*device,ID3D11DeviceContext*deviceContext,XMMATRIX &view,XMMATRIX &projection,XMFLOAT3 viewPos);
	HRESULT InitializeShader(ID3D11Device*device, ID3D11DeviceContext*deviceContext, WCHAR* VSFileName, WCHAR* PSFileName, LPCSTR VSEntryPoint, LPCSTR PSEntryPoint, LPCSTR VSShaderModel, PCSTR PSShaderModel);
	void Update(ID3D11DeviceContext* deviceContext);
	void Render(ID3D11DeviceContext*deviceContext);

public:
	void SetPosition(FLOAT x, FLOAT y, FLOAT z);
	void SetScaler(FLOAT sx, FLOAT sy, FLOAT sz);
	void SetRotation(FLOAT angelX, FLOAT angelY, FLOAT angelZ);
	void SetLightConst(LightConst &light);
	void SetMaterial(Material &mat);
	void SetView(XMMATRIX &view);
	void SetProjection(XMMATRIX &projection);

public:
	virtual void AddInitialize(ID3D11Device*device, ID3D11DeviceContext*deviceContext);
	virtual void AddUpdate(ID3D11DeviceContext* deviceContext);
	virtual void AddRender(ID3D11DeviceContext* deviceContext);

protected:
	HRESULT CompileShaderFromFile(WCHAR* szFileName, LPCSTR szEntryPoint, LPCSTR szShaderModel, ID3DBlob** ppBlobOut);

public:
	//Window
	HWND hwnd;
	//D3D
	ID3DBlob*vertexBlob=NULL;
	ID3DBlob*pixelBlob=NULL;
	ID3D11Buffer*pVertexBuffer=NULL;
	ID3D11Buffer*pIndexBuffer=NULL;
	ID3D11Buffer*m_pConstantBuffer = NULL;
	ID3D11Buffer*m_pLightConstBuffer = NULL;
	ID3D11Buffer*m_pMaterialBuffer = NULL;
	ID3D11VertexShader*m_pVertexShader = NULL;
	ID3D11PixelShader*m_pPixelShader = NULL;
	ID3D11InputLayout*m_pVertexLayout = NULL;
	//Matrix
	ConstanceBuffer matrix;
	//Material
	Material mat;
	//Light
	LightConst light;
	//Mesh
	Mesh2*mesh = NULL;
	UINT indicesCount = 0;
	//Numbers
	int VSConstStartNum = 1;
	int PSConstStartNum = 2;
};


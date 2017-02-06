#pragma once

#pragma comment ( lib, "D3D11.lib")
#pragma comment ( lib, "d3dcompiler.lib")
#pragma comment ( lib,"D3DX11.lib")

#include<D3D11.h>
#include<D3DX11.h>
#include<D3Dcompiler.h>
#include<xnamath.h>
#include"Model.h"

const XMFLOAT4 Grid_C = XMFLOAT4(0.4156f, 0.4156f, 0.4156f, 1.0f);
const XMFLOAT4 TestColor = XMFLOAT4(0.8f, 0.0f, 0.0f, 1.0f);

struct GridParameter
{
	XMFLOAT4 gridColor;
	XMFLOAT4 planeColor;
	FLOAT LineWidth;
	FLOAT LineDistance;
	float order1;
	float order2;
};


class Grids
{
public:
	Grids();
	~Grids();
	HRESULT Initialize(HWND hwnd, ID3D11Device*device, ID3D11DeviceContext*deviceContext);
	void SetView(XMMATRIX &view);
	void SetProjection(XMMATRIX &proj);
	HRESULT Update(ID3D11DeviceContext*deviceContext);
	HRESULT Render(ID3D11DeviceContext*deviceContext);

protected:
	HRESULT CompileShaderFromFile(WCHAR* szFileName, LPCSTR szEntryPoint, LPCSTR szShaderModel, ID3DBlob** ppBlobOut);

protected:
	//Windows
	HWND m_hwnd;
	//D3D
	ID3DBlob*vertexBlob = NULL;
	ID3DBlob*pixelBlob = NULL;
	ID3D11VertexShader*m_pVertexShader=NULL;
	ID3D11PixelShader*m_pPixelShader = NULL;
	ID3D11InputLayout*m_pInputLayout = NULL;
	ID3D11Buffer*m_pVertexBuffer = NULL;
	ID3D11Buffer*m_pIndexBuffer = NULL;
	ID3D11Buffer*m_pConstBuffer = NULL;
	ID3D11Buffer*m_pGridParam = NULL;
	//Matrix
	XMMATRIX world;
	XMMATRIX view;
	XMMATRIX proj;
	//Constant
	GridParameter gridParam;
	ConstanceBuffer constBuffer;
	//Model
	Mesh2 *m_grid=NULL;
	//Counts
	UINT verticesCounts = 0;
	UINT indicesCounts = 0;
};


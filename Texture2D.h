#pragma once

#pragma comment ( lib, "D3D11.lib")
#pragma comment ( lib, "d3dcompiler.lib")
#pragma comment ( lib,"D3DX11.lib")

#include<D3D11.h>
#include<D3DX11.h>
#include<D3Dcompiler.h>
#include<xnamath.h>


struct Texture2DMatrix
{
	XMMATRIX world;
	XMMATRIX view;
	XMMATRIX projection;
};


struct Texture2DVertex
{
	XMFLOAT3 pos;
	float order;
	XMFLOAT3 normal;
	float order1;
};



class Texture2D
{
public:
	Texture2D();
	~Texture2D();
	HRESULT Initialize(HWND hwnd,ID3D11Device*device, ID3D11DeviceContext*deviceContext);
	HRESULT InitializeShader(WCHAR* VSFileName, WCHAR* PSFileName, LPCSTR VSEntryPoint, LPCSTR PSEntryPoint, LPCSTR VSShaderModel, PCSTR PSShaderModel);
	HRESULT Render(ID3D11DeviceContext*deviceContext);
	ID3D11RenderTargetView* GetRenderView();
	void  SetRenderView(ID3D11RenderTargetView*pRenderTargetView);
	ID3D11ShaderResourceView* GetShaderResourceView();
	void  SetShaderResourceView(ID3D11ShaderResourceView*pShaderResourceView);

protected:
	HRESULT CompileShaderFromFile(WCHAR* szFileName, LPCSTR szEntryPoint, LPCSTR szShaderModel, ID3DBlob** ppBlobOut);

protected:
	//Windows
	HWND m_hwnd;
	//D3D
	ID3D11RenderTargetView*m_pRenderTargetView = NULL;
	ID3D11DepthStencilView*m_pDepthStencilView = NULL;
	ID3D11ShaderResourceView*m_pShaderResourceView = NULL;
	ID3D11VertexShader*m_pVertexShader = NULL;
	ID3D11PixelShader*m_pPixelShader = NULL;
	ID3D11InputLayout*m_pInputLayout = NULL;
	ID3D11Buffer*m_pConstantBuffer = NULL;
	ID3D10Texture2D*m_pRenderTexture = NULL;
	//Constant Buffer
	Texture2DMatrix matrix;
	//Vertex
	Texture2DVertex*vertices=NULL;
	WORD*indices = NULL;
	//Counts
	UINT verticesCounts = 0;
	UINT indicesCount = 0;
};


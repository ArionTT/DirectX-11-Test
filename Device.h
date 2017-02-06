#pragma once

#pragma comment ( lib, "D3D11.lib")
#pragma comment ( lib, "d3dcompiler.lib")
#pragma comment ( lib,"D3DX11.lib")

#include<D3D11.h>
#include<D3DX11.h>
#include<D3Dcompiler.h>
#include<xnamath.h>
#include"objLoader.h"
#include"Camera.h"
#include"Time.h"
#include"Model.h"
#include"Grids.h"

#define PI 3.141592654f

//Clear Color
const float ClearColor[4] = { 0.2784f,0.2784f,0.2784f,1.0f };


class Device
{
public:
	Device();
	~Device();

public:
	HRESULT Initialize(HINSTANCE hinstance, HWND hwnd,int width,int height);
	HRESULT Update();
	void Render();

protected:
	HRESULT InitializeSwapChain();
	HRESULT InitializeRenderTarget(bool NoDepth=false);
	HRESULT InitializeShaderView();
	HRESULT InitializeDepthView();
	HRESULT InitializeZBuffer();
	HRESULT InitializeBlend();
	HRESULT InitializeViewPort(int width,int height,int TopX=0,int TopY=0);
	HRESULT InitializeModel();

	HRESULT CompileShaderFromFile(WCHAR* szFileName, LPCSTR szEntryPoint, LPCSTR szShaderModel, ID3DBlob** ppBlobOut);

	void TurnBlend(bool on);
	void TureZBuffer(bool on);

	void RenderBegin();
	void RenderEnd();

protected:
	//Window
	HWND  m_hwnd = NULL;
	int width = 0;
	int height = 0;
	//D3D 
	IDXGISwapChain*m_pSwapChain = NULL;
	ID3D11Device*m_pDevice = NULL;
	ID3D11DeviceContext*m_pImmediateContext = NULL;
	D3D_FEATURE_LEVEL m_featureLevel = D3D_FEATURE_LEVEL_11_0;
	ID3D11RenderTargetView* m_pRenderTargetView = NULL;
	ID3D11ShaderResourceView*m_pShaderResourceView = NULL;
	ID3D11Texture2D*m_pRenderTexture = NULL;
	ID3D11DepthStencilView* m_pDepthStencilView = NULL;
	ID3D11Texture2D*m_pDepthStencil = NULL;
	ID3D11BlendState*m_alphaEnableBlendingState = NULL;
	ID3D11BlendState*m_alphaDisableBlendingState = NULL;
	ID3D11DepthStencilState*m_pDepthEnableState = NULL;
	ID3D11DepthStencilState*m_pDepthDisableState = NULL;
	//Matrix
	XMMATRIX  m_World;
	XMMATRIX  m_View;
	XMMATRIX  m_Projection;
	XMMATRIX m_Ortho;
	//Buffer
	UINT verticesCount = 0;
	UINT indicesCount = 0;
	//Camera
	Camera*m_camera=NULL;
	//Mesh
	Model*looper = NULL;
	Model*cube = NULL;
	Grids*grid = NULL;
	//Timer
	Time*timer=NULL;
	float deltaTime;
	float FPS;
	//Angel
	float Angel = 0;
	//Bounding Sphere
	float boundingRadius = 0.0f;

protected:
	//A parameter to Turn on or Ture off  Blend /Z Buffer
	const bool ON = true;
	//A parameter to Turn on or Ture off  Blend /Z Buffer
	const bool OFF = false;

	//A Parameter for InitializeRenderTarget(bool)
	//if parameter is false, Depth view has bean used
	const bool AddDepth = false;
	const bool NoDepth = true;
};


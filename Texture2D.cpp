#include "Texture2D.h"

HRESULT Texture2D::CompileShaderFromFile(WCHAR* szFileName, LPCSTR szEntryPoint, LPCSTR szShaderModel, ID3DBlob** ppBlobOut)
{
	HRESULT hr = S_OK;

	DWORD dwShaderFlags = D3DCOMPILE_ENABLE_STRICTNESS;
#if defined( DEBUG ) || defined( _DEBUG )
	// Set the D3DCOMPILE_DEBUG flag to embed debug information in the shaders.
	// Setting this flag improves the shader debugging experience, but still allows 
	// the shaders to be optimized and to run exactly the way they will run in 
	// the release configuration of this program.
	dwShaderFlags |= D3DCOMPILE_DEBUG;
#endif

	ID3DBlob* pErrorBlob;
	hr = D3DX11CompileFromFile(szFileName, NULL, NULL, szEntryPoint, szShaderModel,
		dwShaderFlags, 0, NULL, ppBlobOut, &pErrorBlob, NULL);
	if (FAILED(hr))
	{
		MessageBox(m_hwnd, L"Complie 1 failed", L"ERROR", MB_OK);
		if (pErrorBlob != NULL)
		{
			MessageBox(m_hwnd, L"Shader Compiler Debugger Info Has Out", L"Error", MB_OK);
			OutputDebugStringA((char*)pErrorBlob->GetBufferPointer());
		}
		if (pErrorBlob)
			pErrorBlob->Release();
		return hr;
	}
	if (pErrorBlob)
		pErrorBlob->Release();

	return S_OK;
}

Texture2D::Texture2D()
{
}

Texture2D::~Texture2D()
{
}

HRESULT Texture2D::Initialize(HWND hwnd, ID3D11Device*device, ID3D11DeviceContext*deviceContext)
{
	HRESULT hr = S_OK;

	return hr;
}

HRESULT Texture2D::InitializeShader(WCHAR* VSFileName, WCHAR* PSFileName, LPCSTR VSEntryPoint, LPCSTR PSEntryPoint, LPCSTR VSShaderModel, PCSTR PSShaderModel)
{
	HRESULT hr = S_OK;

	return hr;
}

HRESULT Texture2D::Render(ID3D11DeviceContext*deviceContext)
{
	HRESULT hr = S_OK;

	return hr;
}

ID3D11RenderTargetView* Texture2D::GetRenderView()
{
	return this->m_pRenderTargetView;
}

void  Texture2D::SetRenderView(ID3D11RenderTargetView*pRenderTargetView)
{

}

ID3D11ShaderResourceView* Texture2D::GetShaderResourceView()
{
	return this->m_pShaderResourceView;
}

void  Texture2D::SetShaderResourceView(ID3D11ShaderResourceView*pShaderResourceView)
{

}

#include "Grids.h"


HRESULT Grids::CompileShaderFromFile(WCHAR* szFileName, LPCSTR szEntryPoint, LPCSTR szShaderModel, ID3DBlob** ppBlobOut)
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

Grids::Grids()
{
	m_grid = new Mesh2();
}

Grids::~Grids()
{

}

HRESULT Grids::Initialize(HWND hwnd, ID3D11Device*device, ID3D11DeviceContext*deviceContext)
{
	HRESULT hr = S_OK;

	D3D11_BUFFER_DESC bd;
	D3D11_SUBRESOURCE_DATA initData;

	m_hwnd = hwnd;

	hr=m_grid->Initialize("model/PlaneB4x4.obj");
	if (FAILED(hr))
	{
		MessageBox(NULL, L"Initial Grid Plane Failed", L"Error", MB_OK);
		return hr;
	}
	indicesCounts = m_grid->GetIndicesCount();
	verticesCounts = m_grid->GetVertexCount();

	ZeroMemory(&bd, sizeof(bd));
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(VertexType)*verticesCounts;
	bd.CPUAccessFlags = 0;
	bd.MiscFlags = 0;
	bd.StructureByteStride = 0;
	ZeroMemory(&initData, sizeof(initData));
	VertexType*vertices = m_grid->GetVertices();
	initData.pSysMem = vertices;
	hr = device->CreateBuffer(&bd, &initData, &m_pVertexBuffer);
	if (FAILED(hr))
	{
		MessageBox(NULL, L"Initial Grid Vertex Buffer Failed", L"Error", MB_OK);
		return hr;
	}

	ZeroMemory(&bd, sizeof(bd));
	bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(WORD)*indicesCounts;
	bd.CPUAccessFlags = 0;
	bd.MiscFlags = 0;
	bd.StructureByteStride = 0;
	ZeroMemory(&initData, sizeof(initData));
	WORD*indices = m_grid->GetIndices();
	initData.pSysMem = indices;
	hr = device->CreateBuffer(&bd, &initData, &m_pIndexBuffer);
	if (FAILED(hr))
	{
		MessageBox(NULL, L"Initial Grid Vertex Buffer Failed", L"Error", MB_OK);
		return hr;
	}

	ZeroMemory(&bd, sizeof(bd));
	bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(ConstanceBuffer);
	bd.CPUAccessFlags = 0;
	bd.MiscFlags = 0;
	bd.StructureByteStride = 0;
	hr = device->CreateBuffer(&bd, NULL, &m_pConstBuffer);
	if (FAILED(hr))
	{
		MessageBox(NULL, L"Initial Grid Vertex Buffer Failed", L"Error", MB_OK);
		return hr;
	}
	world= XMMatrixIdentity();
	
	ZeroMemory(&bd, sizeof(bd));
	bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(GridParameter);
	bd.CPUAccessFlags = 0;
	bd.MiscFlags = 0;
	bd.StructureByteStride = 0;
	hr = device->CreateBuffer(&bd, NULL, &m_pGridParam);
	if (FAILED(hr))
	{
		MessageBox(NULL, L"Initial Grid Vertex Buffer Failed", L"Error", MB_OK);
		return hr;
	}


	//Vertex Shader
	ID3DBlob*pVertBlob = NULL;
	hr = CompileShaderFromFile(L"vgrid.fx", "vert", "vs_4_0", &pVertBlob);
	if (FAILED(hr))
	{
		MessageBox(NULL, L"Complie Vertex Shader Failed!", L"ERROR", MB_OK);
		return hr;
	}
	hr = device->CreateVertexShader(pVertBlob->GetBufferPointer(), pVertBlob->GetBufferSize(), NULL, &m_pVertexShader);
	if (FAILED(hr))
	{
		pVertBlob->Release();
		return hr;
	}

	//Set Layout
	D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};
	UINT numElements = ARRAYSIZE(layout);
	hr = device->CreateInputLayout(layout, numElements, pVertBlob->GetBufferPointer(), pVertBlob->GetBufferSize(), &m_pInputLayout);
	if (FAILED(hr))
	{
		MessageBox(NULL, L"Create Input Layout Failed", L"Error", MB_OK);
		return hr;
	}

	//Pixel Shader
	ID3DBlob*pPixelBlob = NULL;
	hr = CompileShaderFromFile(L"pgrid.fx", "pixel", "ps_4_0", &pPixelBlob);
	if (FAILED(hr))
	{
		MessageBox(NULL, L"Complie Pixel Shader Failed!", L"ERROR", MB_OK);
		return hr;
	}

	//Create PShader
	hr = device->CreatePixelShader(pPixelBlob->GetBufferPointer(), pPixelBlob->GetBufferSize(), NULL, &m_pPixelShader);
	if (FAILED(hr))
	{
		pPixelBlob->Release();
		return hr;
	}

	return hr;
}

HRESULT Grids::Update(ID3D11DeviceContext*deviceContext)
{
	HRESULT hr = S_OK;
	ConstanceBuffer matrix;
	matrix.world = world;
	matrix.view = view;
	matrix.projection = proj;
	GridParameter gridPara;
	gridPara.gridColor = Grid_C;
	gridPara.planeColor = XMFLOAT4(1.0f, 1.0f, 1.0f, 0.0f);
	gridPara.LineDistance = 0.8f;
	gridPara.LineWidth = 0.05f;

	deviceContext->UpdateSubresource(m_pConstBuffer, 0, NULL, &matrix, 0, 0);
	deviceContext->UpdateSubresource(m_pGridParam, 0, NULL, &gridPara, 0, 0);

	return hr;
}

HRESULT Grids::Render(ID3D11DeviceContext*deviceContext)
{
	HRESULT hr = S_OK;

	UINT stride = sizeof(VertexType);
	UINT offset = 0;
	deviceContext->IASetVertexBuffers(0, 1, &m_pVertexBuffer, &stride, &offset);
	deviceContext->IASetIndexBuffer(m_pIndexBuffer, DXGI_FORMAT_R16_UINT, 0);
	deviceContext->IASetInputLayout(m_pInputLayout);

	deviceContext->VSSetShader(m_pVertexShader, NULL, 0);
	deviceContext->PSSetShader(m_pPixelShader, NULL, 0);
	deviceContext->VSSetConstantBuffers(0, 1, &m_pConstBuffer);
	deviceContext->PSSetConstantBuffers(0, 1, &m_pGridParam);

	deviceContext->DrawIndexed(indicesCounts, 0, 0);

	return hr;
}

void Grids::SetView(XMMATRIX &view)
{
	this->view = view;
}

void Grids::SetProjection(XMMATRIX &proj)
{
	this->proj = proj;
}
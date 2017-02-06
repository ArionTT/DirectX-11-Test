#include "Model.h"



Model::Model()
{
	mesh = new Mesh2();
}

Model::~Model()
{
}

HRESULT Model::Initialize(std::string filename, HWND hwnd, ID3D11Device*device, ID3D11DeviceContext*deviceContext, XMMATRIX &view, XMMATRIX &projection, XMFLOAT3 viewPos)
{
	this->hwnd = hwnd;

	HRESULT hr = true;
	hr=mesh->Initialize(filename);
	if (!hr)
		return hr;

	int verticesCount = mesh->GetVertexCount();
	indicesCount = mesh->GetIndicesCount();
	VertexType*vertices = mesh->GetVertices();
	WORD*indices = mesh->GetIndices();

	D3D11_BUFFER_DESC bd;

	//Set Vertex Buffer
	ZeroMemory(&bd, sizeof(bd));
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(VertexType)*verticesCount;
	bd.CPUAccessFlags = 0;
	bd.MiscFlags = 0;
	bd.StructureByteStride = 0;
	D3D11_SUBRESOURCE_DATA initData;
	ZeroMemory(&initData, sizeof(initData));
	initData.pSysMem = vertices;
	hr = device->CreateBuffer(&bd, &initData, &pVertexBuffer);
	if (FAILED(hr))
	{
		MessageBox(NULL, L"Create Vertex Buffer Failed", L"Error", MB_OK);
		return hr;
	}

	//Set Index Buffer
	ZeroMemory(&bd, sizeof(bd));
	bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(WORD)*indicesCount;
	bd.CPUAccessFlags = 0;
	bd.MiscFlags = 0;
	bd.StructureByteStride = 0;
	ZeroMemory(&initData, sizeof(initData));
	initData.pSysMem = indices;
	hr = device->CreateBuffer(&bd, &initData, &pIndexBuffer);
	if (FAILED(hr))
	{
		MessageBox(NULL, L"Create Index Buffer Failed", L"Error", MB_OK);
		return hr;
	}

	//Set Constance Buffer
	ZeroMemory(&bd, sizeof(bd));
	bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(ConstanceBuffer);
	bd.CPUAccessFlags = 0;
	bd.MiscFlags = 0;
	bd.StructureByteStride = 0;
	hr = device->CreateBuffer(&bd, NULL, &m_pConstantBuffer);
	if (FAILED(hr))
	{
		MessageBox(NULL, L"Create Constant Buffer Failed", L"Error", MB_OK);
		return hr;
	}
	matrix.world = XMMatrixIdentity();
	matrix.view = view;
	matrix.projection = projection;

	//Set Light Buffer
	ZeroMemory(&bd, sizeof(bd));
	bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(LightConst);
	bd.CPUAccessFlags = 0;
	bd.MiscFlags = 0;
	bd.StructureByteStride = 0;
	hr = device->CreateBuffer(&bd, NULL, &m_pLightConstBuffer);
	if (FAILED(hr))
	{
		MessageBox(NULL, L"Create Light Constant Buffer Failed", L"Error", MB_OK);
		return hr;
	}
	light.LightColor = XMFLOAT4(defaultLightColor[0], defaultLightColor[1], defaultLightColor[2], defaultLightColor[3]);
	light.LightPos = XMFLOAT3(defaultLigtPos[0], defaultLigtPos[1], defaultLigtPos[2]);
	light.ViewPos = viewPos;
	light.LightDir = XMFLOAT3(defaultLigtPos[0], defaultLigtPos[1], defaultLigtPos[2]);

	//Set Material
	ZeroMemory(&bd, sizeof(bd));
	bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(Material);
	bd.CPUAccessFlags = 0;
	bd.MiscFlags = 0;
	bd.StructureByteStride = 0;
	hr = device->CreateBuffer(&bd, NULL, &m_pMaterialBuffer);
	if (FAILED(hr))
	{
		MessageBox(NULL, L"Create Light Constant Buffer Failed", L"Error", MB_OK);
		return hr;
	}
	mat.Glossness = 8.0f;
	mat.MatColor = XMFLOAT4(defaultMatColor[0], defaultMatColor[1], defaultMatColor[2], defaultMatColor[3]);

	AddInitialize(device,deviceContext);

	return true;
}

HRESULT Model::InitializeShader(ID3D11Device*device, ID3D11DeviceContext*deviceContext,WCHAR* VSFileName,WCHAR* PSFileName, LPCSTR VSEntryPoint, LPCSTR PSEntryPoint, LPCSTR VSShaderModel, PCSTR PSShaderModel)
{
	HRESULT hr = S_OK;

	//Vertex Shader
	ID3DBlob*pVertBlob = NULL;
	hr = CompileShaderFromFile(VSFileName, VSEntryPoint, VSShaderModel, &pVertBlob);
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
	hr = device->CreateInputLayout(layout, numElements, pVertBlob->GetBufferPointer(), pVertBlob->GetBufferSize(), &m_pVertexLayout);
	if (FAILED(hr))
	{
		MessageBox(NULL, L"Create Input Layout Failed", L"Error", MB_OK);
		return hr;
	}

	//Pixel Shader
	ID3DBlob*pPixelBlob = NULL;
	hr = CompileShaderFromFile(PSFileName, PSEntryPoint, PSShaderModel, &pPixelBlob);
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

void Model::AddInitialize(ID3D11Device*device, ID3D11DeviceContext*deviceContext)
{

}

void Model::AddUpdate(ID3D11DeviceContext*deviceContext)
{

}

void Model::AddRender(ID3D11DeviceContext*deviceContext)
{

}

void Model::SetPosition(FLOAT x, FLOAT y, FLOAT z)
{
	XMFLOAT3 pos = XMFLOAT3(x, y, z);
	XMVECTOR translate = XMLoadFloat3(&pos);
	this->matrix.world = XMMatrixTranslationFromVector(translate);

}

void Model::SetScaler(FLOAT sx, FLOAT sy, FLOAT sz)
{

}

void Model::SetRotation(FLOAT angelX, FLOAT angelY, FLOAT angelZ)
{

}

void Model::SetLightConst(LightConst &light)
{

}

void Model::SetMaterial(Material &mat)
{
	this->mat.Glossness = mat.Glossness;
	this->mat.MatColor = mat.MatColor;
}

void Model::Update(ID3D11DeviceContext* deviceContext)
{
	deviceContext->UpdateSubresource(m_pConstantBuffer, 0, NULL, &matrix, 0, 0);
	deviceContext->UpdateSubresource(m_pLightConstBuffer, 0, NULL, &light, 0, 0);
	deviceContext->UpdateSubresource(m_pMaterialBuffer, 0, NULL, &mat, 0, 0);

	AddUpdate(deviceContext);
}

void Model::Render(ID3D11DeviceContext*deviceContext)
{
	UINT stride = sizeof(VertexType);
	UINT offset = 0;
	deviceContext->IASetVertexBuffers(0, 1, &pVertexBuffer, &stride, &offset);
	deviceContext->IASetIndexBuffer(pIndexBuffer, DXGI_FORMAT_R16_UINT, 0);
	deviceContext->IASetInputLayout(m_pVertexLayout);

	deviceContext->VSSetShader(m_pVertexShader, NULL, 0);
	deviceContext->PSSetShader(m_pPixelShader, NULL, 0);
	deviceContext->VSSetConstantBuffers(0, 1, &m_pConstantBuffer);
	deviceContext->PSSetConstantBuffers(0, 1, &m_pLightConstBuffer);
	deviceContext->PSSetConstantBuffers(1, 1, &m_pMaterialBuffer);

	AddRender(deviceContext);

	deviceContext->DrawIndexed(indicesCount, 0, 0);
}

void Model::SetView(XMMATRIX &view)
{
	matrix.view = view;
}

void Model::SetProjection(XMMATRIX &projection)
{
	matrix.projection = projection;
}

HRESULT Model::CompileShaderFromFile(WCHAR* szFileName, LPCSTR szEntryPoint, LPCSTR szShaderModel, ID3DBlob** ppBlobOut)
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
		MessageBox(hwnd, L"Complie 1 failed", L"ERROR", MB_OK);
		if (pErrorBlob != NULL)
		{
			MessageBox(hwnd, L"Shader Compiler Debugger Info Has Out", L"Error", MB_OK);
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


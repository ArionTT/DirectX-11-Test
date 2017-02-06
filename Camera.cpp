#include "Camera.h"



Camera::Camera()
{
	m_input = new Input();
	MoveSpeed = 0.01f;
}

Camera::~Camera()
{
}

//bool Camera::Initialize(HINSTANCE hInstance, HWND hwnd, int width, int height)
//{
//	bool hr = true;
//	hr=m_input->Initialize(hInstance,hwnd,width,height);
//	if (!hr)
//		return hr;
//
//
//	//initialize view Matrix
//	Eye = XMFLOAT3(0.0f, 1.0f, -5.0f);
//	At = XMFLOAT3(0.0f, 1.0f, 0.0f);
//	Up = XMFLOAT3(0.0f, 1.0f, 0.0f);
//
//	//Initialize
//	Yaw = YAW;
//	Pitch = PITCH;
//	Front.x = At.x - Eye.x;
//	Front.y = At.y - Eye.y;
//	Front.z = At.z - Eye.z;
//	MoveSpeed = SPEED;
//	MouseSensitivity = SENCITIVITY;
//	Zoom = ZOOM;
//	WorldUp = Up;
//	DeltaTime = 0.0f;
//	constrainPitch = true;
//
//	//Set View
//	UpdateCameraVector();
//	XMFLOAT3 t_at = XMFLOAT3(Eye.x + Front.x, Eye.y + Front.y, Eye.z + Front.z);
//	SetViewParameter(Eye, t_at, Up);
//
//	//Initialize Projection Matrix
//	projection = XMMatrixPerspectiveFovLH(XM_PIDIV2, width / (FLOAT)height, 0.01f, 100.0f);
//
//	return true;
//}

bool Camera::Initialize(HINSTANCE hInstance, HWND hwnd, int width, int height, XMFLOAT3 eye, XMFLOAT3 at)
{
	bool hr = true;
	hr = m_input->Initialize(hInstance, hwnd, width, height);
	if (!hr)
		return hr;


	//initialize view Matrix
	Eye = XMFLOAT3(eye);
	At = XMFLOAT3(at);
	Up = XMFLOAT3(0.0f, 1.0f, 0.0f);

	//Initialize
	Yaw = YAW;
	Pitch = PITCH;
	Front.x = At.x - Eye.x;
	Front.y = At.y - Eye.y;
	Front.z = At.z - Eye.z;
	MoveSpeed = SPEED;
	MouseSensitivity = SENCITIVITY;
	Zoom = ZOOM;
	WorldUp = Up;
	DeltaTime = 0.0f;
	constrainPitch = true;

	//Set View
	UpdateCameraVector();
	XMFLOAT3 t_at = XMFLOAT3(Eye.x + Front.x, Eye.y + Front.y, Eye.z + Front.z);
	SetViewParameter(Eye, t_at, Up);

	//Initialize Projection Matrix
	projection = XMMatrixPerspectiveFovLH(XM_PIDIV2, width / (FLOAT)height, 0.01f, 100.0f);

	return true;
}

void Camera::SetInitPosition(XMFLOAT3 Pos)
{
	this->Eye = Pos;
	UpdateCameraVector();
	XMFLOAT3 t_at = XMFLOAT3(Eye.x + Front.x, Eye.y + Front.y, Eye.z + Front.z);
	SetViewParameter(Eye, t_at, Up);
}

void Camera::Frame()
{
	int AxisX = 0;
	int AxisY = 0;
	float moveX = 0;
	float moveY = 0;

	m_input->Frame();
	//Set View Matrix
	//Mouse Down
	if (m_input->isMouseRightDown())   //Free Camera
	{
		AxisX=m_input->GetAxisX()*MouseSensitivity;
		AxisY=m_input->GetAxisY()*MouseSensitivity;

		this->Yaw -= AxisX;
		this->Pitch -= AxisY;
		if (constrainPitch)
		{
			if (this->Pitch > 89.0f)
				this->Pitch = 89.0f;
			if (this->Pitch < -89.0f)
				this->Pitch = -89.0f;
		}
		UpdateCameraVector();
		XMFLOAT3 t_at = XMFLOAT3(Eye.x + Front.x, Eye.y + Front.y, Eye.z + Front.z);
		SetViewParameter(Eye, t_at, Up);
	}
	else if(m_input->isMouseLeftDown())  //ƽ�Ʊ任
	{

	}

	FLOAT velocity = this->MoveSpeed*DeltaTime;
	//Keyboard down
	if (m_input->isSKeyDown())
	{
		Eye = XMFLOAT3(Eye.x-Front.x*velocity, Eye.y-Front.y*velocity, Eye.z-Front.z*velocity);
		XMFLOAT3 t_at = XMFLOAT3(Eye.x + Front.x, Eye.y + Front.y, Eye.z + Front.z);
		SetViewParameter(Eye, t_at, Up);
	}
	if (m_input->isWKeyDown())
	{
		Eye = XMFLOAT3(Front.x*velocity + Eye.x, Front.y*velocity + Eye.y, Front.z*velocity + Eye.z);
		XMFLOAT3 t_at = XMFLOAT3(Eye.x + Front.x, Eye.y + Front.y, Eye.z + Front.z);
		SetViewParameter(Eye, t_at, Up);
	}
	if (m_input->isDKeyDown())
	{
		Eye = XMFLOAT3(Eye.x-Right.x*velocity , Eye.y-Right.y*velocity, Eye.z-Right.z*velocity);
		XMFLOAT3 t_at = XMFLOAT3(Eye.x + Front.x, Eye.y + Front.y, Eye.z + Front.z);
		SetViewParameter(Eye, t_at, Up);
	}
	if (m_input->isAKeyDown())
	{
		Eye = XMFLOAT3(Right.x*velocity + Eye.x, Right.y*velocity + Eye.y, Right.z*velocity + Eye.z);
		XMFLOAT3 t_at = XMFLOAT3(Eye.x + Front.x, Eye.y + Front.y, Eye.z + Front.z);
		SetViewParameter(Eye, t_at, Up);
	}

	//Set  Projection Matrix
	//...
}

XMMATRIX Camera::GetViewMatrix()
{
	return this->view;
}

XMMATRIX Camera::GetProjectionMatrix()
{
	return this->projection;
}

HRESULT Camera::SetViewParameter(XMFLOAT3 eye, XMFLOAT3 at, XMFLOAT3 up)
{
	XMVECTOR eyePos = XMLoadFloat3(&eye);
	XMVECTOR atPos = XMLoadFloat3(&at);
	XMVECTOR upPos = XMLoadFloat3(&up);
	this->view = XMMatrixLookAtLH(eyePos, atPos, upPos);
	return S_OK;
}

HRESULT Camera::SetProjectionParameter(float fieldOfView, float aspectRatio, float nearPlane, float farPlane)
{
	this->projection = XMMatrixPerspectiveFovLH(fieldOfView,aspectRatio, nearPlane, farPlane);
	return S_OK;
}

XMFLOAT3 Camera::GetEyePos()
{
	return Eye;
}

void Camera::SetEyePos(XMFLOAT3 eye)
{
	this->Eye = eye;
}

bool Camera::isPitchConstrain()
{
	return constrainPitch;
}

void Camera::SetConstrainPitch(bool constrain)
{
	this->constrainPitch = constrain;
}

void Camera::SetDeltaTime(FLOAT delta)
{
	DeltaTime = delta;
}

void Camera::UpdateCameraVector()
{
	XMFLOAT3 front;
	front.x = cosf(XMConvertToRadians(this->Yaw))*cosf(XMConvertToRadians(this->Pitch));
	front.y = sinf(XMConvertToRadians(this->Pitch));
	front.z = sinf(XMConvertToRadians(this->Yaw))*cosf(XMConvertToRadians(this->Pitch));
	XMVECTOR f = XMLoadFloat3(&front);
	XMVECTOR r = XMLoadFloat3(&Right);
	XMVECTOR u = XMLoadFloat3(&Up);
	XMVECTOR w_up = XMLoadFloat3(&WorldUp);
	f = XMVector3Normalize(f);
	r = XMVector3Cross(f, w_up);
	u = XMVector3Cross(r,f);
	XMStoreFloat3(&Front, f);
	XMStoreFloat3(&Right, r);
	XMStoreFloat3(&Up, u);
}

bool Camera::isSpaceDown()
{
	return m_input->isSpaceDown();
}


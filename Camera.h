#pragma once

#pragma comment ( lib, "D3D11.lib")
#pragma comment ( lib,"D3DX11.lib")

#include<D3D11.h>
#include<D3DX11.h>
#include<xnamath.h>
#include"Input.h"

const FLOAT YAW = 90.0f;
const FLOAT PITCH = 0.0f;
const FLOAT SPEED = 10.0f;
const FLOAT SENCITIVITY = 0.25f;
const FLOAT ZOOM = 45.0f;

class Camera
{
public:
	Camera();
	~Camera();
	//bool Initialize(HINSTANCE hInstance,HWND hwnd,int width,int height);
	bool Initialize(HINSTANCE hInstance, HWND hwnd, int width, int height, XMFLOAT3 eye = XMFLOAT3(0.0f, 1.0f, -5.0f), XMFLOAT3 at = XMFLOAT3(0.0f, 1.0f, 0.0f));

	void Frame();


public:
	XMMATRIX GetViewMatrix();
	XMMATRIX GetProjectionMatrix();
	XMFLOAT3 GetEyePos();
	HRESULT SetViewParameter(XMFLOAT3 eye,XMFLOAT3 at,XMFLOAT3 up);
	HRESULT SetProjectionParameter(float fieldOfView,float aspectRatio,float nearPlane,float farPlane);
	void SetEyePos(XMFLOAT3 eye);
	void SetInitPosition(XMFLOAT3 Pos);
	void SetDeltaTime(FLOAT delta);
	void SetConstrainPitch(bool);
	bool isSpaceDown();
private:
	void UpdateCameraVector();
	bool isPitchConstrain();

private:
	XMMATRIX view;
	XMMATRIX projection;
	XMFLOAT3 Eye;
	XMFLOAT3 At;
	XMFLOAT3 Up;

	XMFLOAT3 Front;
	XMFLOAT3 Right;
	XMFLOAT3 WorldUp;

	FLOAT Yaw;
	FLOAT Pitch;
	//FLOAT Roll;

	FLOAT MoveSpeed;
	FLOAT MouseSensitivity;
	FLOAT Zoom;
	FLOAT DeltaTime;

	Input* m_input;

	bool constrainPitch;
};


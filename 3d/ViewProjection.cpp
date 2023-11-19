#include "ViewProjection.h"
#include"WinApp.h"

void ViewProjection::Initialize() {
	WinApp* sWinApp = WinApp::GetInstance();
	cameraTransform={ {1.0f,1.0f,1.0f},{0.0f,0.0f,0.0f},{0.0f,00.0f,-15.0f} };
	//WinApp* sWinApp = WinApp::GetInstance();

	cameraMatrix = MakeAffineMatrix(cameraTransform.scale, cameraTransform.rotate, cameraTransform.translate);
	viewMatrix = Inverse(cameraMatrix);
	projectionMatrix = MakePerspectiveFovMatrix(0.45f, float(sWinApp->GetKClientWidth()) / float(sWinApp->GetKClientHeight()), 0.1f, 100.0f);
	//worldViewProjectionMatrix = Multiply(worldmatrix, Multiply(viewMatrix, projectionMatrix));

}

void ViewProjection::Update() {
	WinApp* sWinApp = WinApp::GetInstance();
	
	cameraMatrix = MakeAffineMatrix(cameraTransform.scale, cameraTransform.rotate, cameraTransform.translate);
	viewMatrix = Inverse(cameraMatrix);
	projectionMatrix = MakePerspectiveFovMatrix(0.45f, float(sWinApp->GetKClientWidth()) / float(sWinApp->GetKClientHeight()), 0.1f, 100.0f);
	worldViewProjectionMatrix = Multiply(worldmatrix, Multiply(viewMatrix, projectionMatrix));


}

void ViewProjection::Draw() {

}

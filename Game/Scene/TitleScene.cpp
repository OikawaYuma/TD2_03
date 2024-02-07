#include "TitleScene.h"
#include "ImGuiCommon.h"
#define _USE_MATH_DEFINES
#include<math.h>

void TitleScene::Init()
{
	
	camera = new Camera;
	camera->Initialize();
	camera->cameraTransform_.translate = { 0.0f,1.5f,0.0f };
	input = Input::GetInstance();
	room = new Room();
	room->Init();
	Audiohandle_ = Audio::SoundLoadWave("Resources/Audio/Dirft3.wav");
	car_ = new TitleCar();
	car_->Init();
	fadeBlack = new Sprite();
	fadeBlack->Initialize({ 1.0f, 1.0f, 1.0f, fadeColor });
	fadeBlack->SetSize({ 1280.0f, 720.0f });
	texture_ = TextureManager::StoreTexture("Resources/Fade/FadeKURO.png");


	

}

void TitleScene::Update()
{
	ImGui::Begin("Camera");

	ImGui::DragFloat3("cameratranslation_", &camera->cameraTransform_.translate.x, 0.01f);
	ImGui::DragFloat3("cameratate_", &camera->cameraTransform_.rotate.x, 0.01f);
	ImGui::End();
	sceneTime++;
	if (input->TriggerKey(DIK_SPACE)) {
		sceneNo = CLEAR;
		sceneTime = 0;
	}

	XINPUT_STATE joyState;
	Input::GetInstance()->GetJoystickState(joyState);



	if (joyState.Gamepad.wButtons & XINPUT_GAMEPAD_A && !input->GetIsTrigger()) {
		isSceneChanege = true;
	}
	if (isSceneChanege) {
		rotate_ = Lerp(rotate_, -0.0001f, 0.05f);
	}
	//else if(!(joyState.Gamepad.wButtons & XINPUT_GAMEPAD_A )&& input->GetIsTrigger()){ input->SetIsTrriger(false); }
	////カメラの更新
	//if(Input::GetInstance()->)
	cameraTime++;
	if (cameraTime > 300.0f) {
		FadeBlackChange();
	}


	switch (cameraSet) {
	case OneCame: {
		float theta = (rotate_ / 2.0f) * (float)M_PI;
		Vector2 move = { cosf(theta),sinf(theta) };
		rotate_ += 0.015f;
		
		Vector3 offset = { 0.00f,0.0f,-10.0f };
		camera->cameraTransform_.rotate.y = rotate_;
		camera->Update();
		offset = TransformNormal(offset, camera->cameraMatrix_);
		camera->cameraTransform_.translate.x = 0 + offset.x;
		camera->cameraTransform_.translate.z = 10 + offset.z;
		break;
	}
	case TwoCame: {
		camera->cameraTransform_.translate.x -= 0.020f;
		camera->Update();
		break;
	}
	case ThreeCame: {
		camera->cameraTransform_.translate.x += 0.01f;
		camera->Update();
		break;
	}

	}

	fadeBlack->Update();
	car_->Update();




	if (Input::GetInstance()->TriggerKey(DIK_A)) {
		Audio::SoundPlayWave(Audio::GetIXAudio().Get(), Audiohandle_, true);
	}

}
void TitleScene::Draw()
{

	car_->Draw(camera);
	room->Draw(camera);
	fadeBlack->Draw(texture_, { 1.0f,1.0f,1.0f,fadeColor });

}

void TitleScene::Release() {

	Audio::SoundUnload(Audiohandle_);
	delete car_;

}

// ゲームを終了
int TitleScene::GameClose()
{
	return false;
}

void TitleScene::FadeBlackChange()
{
	if (fadeBlackFlag) {
		fadeColor += 0.025f;
		if (fadeColor >= 1.0f) {
			fadeBlackTimer++;
			if (fadeBlackTimer >= 5) {
				fadeBlackFlag = false;
				fadeBlackTimer = 0;
				CameraChange();
				
			}
		}
	}
	else if (!fadeBlackFlag) {
		fadeColor -= 0.025f;
		if (fadeColor <= 0.0f) {
			fadeBlackFlag = true;
			cameraTime = 0;
		}
	}

}

void TitleScene::CameraChange()
{

	switch (cameraSet) {
	case OneCame: {
		cameraSet = TwoCame;
		camera->cameraTransform_.translate = { 6.59f,0.27f,14.20f };
		camera->cameraTransform_.rotate = { -0.09f,3.83f,0.0f };
		break;
	}
	case TwoCame: {
		cameraSet = ThreeCame;
		camera->cameraTransform_.translate = { -2.0f,0.65f,5.0f };
		camera->cameraTransform_.rotate = { 0.0f,0.0f,0.0f };
		
		break;
	}
	case ThreeCame: {
		cameraSet = OneCame;
		camera->cameraTransform_.translate = { 0.0f,1.5f,0.0f };
		camera->cameraTransform_.rotate = { 0.0f,0.0f,0.0f };
		rotate_ = 0.0f;
		Vector3 offset = { 0.00f,0.0f,-10.0f };
		camera->cameraTransform_.rotate.y = rotate_;
		camera->Update();
		break;
	}
	}

}

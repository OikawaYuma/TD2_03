#include "Car.h"
#include "ImGuiCommon.h"
Car::Car() {

}

Car::~Car() {

}

void Car::Init() {
	input = Input::GetInstance();
	worldTransform_.Initialize();
	texture_ = TextureManager::StoreTexture("Resources/uvChecker.png");
	model_ = new Model();
	model_->Initialize("Resources/demo_car", "demo_cube.obj", color);
}

void Car::Update() {
	if (input->PushKey(DIK_LSHIFT)) {
		Speed = ShiftSpeed;
	}
	else
	{
		Speed = NormalSpeed;
	}
	if (input->PushKey(DIK_W)) {
		worldTransform_.translation_.z += Speed;
	}
	if (input->PushKey(DIK_A)) {
		worldTransform_.translation_.x -= Speed;
	}
	if (input->PushKey(DIK_D)) {
		worldTransform_.translation_.x += Speed;
	}
	if (input->PushKey(DIK_S)) {
		worldTransform_.translation_.z -= Speed;
	}
	worldTransform_.UpdateMatrix();
	ImGui::Begin("Demo_Car");
	ImGui::DragFloat4("color", &color.x, 0.01f);
	ImGui::Text("Shift & WASD = Dash");
	ImGui::End();
	//Audio::SoundLoopWave(Audio::GetIXAudio().Get(), soundData);
}

void Car::Draw(Camera* camera) {
	model_->Draw(worldTransform_, texture_, camera, color);
	//sprite_->Draw(texture_,color);
}

void Car::Release()
{
}
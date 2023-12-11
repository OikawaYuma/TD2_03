#include "Player.h"

Player::Player() {

}

Player::~Player() {

}

void Player::Init() {
	input = Input::GetInstance();
	worldTransform_.Initialize();
	texture_ = TextureManager::StoreTexture("Resources/uvChecker.png");
	texture2_ = TextureManager::StoreTexture("Resources/monsterBall.png");

	model_ = new Model();
	model_->Initialize("Resources/multiMaterial","multiMaterial.obj");
}

void Player::Update() {
	if (input->TriggerKey(DIK_A)) {
		IScene::SetSceneNo(STAGE) ;
	}
}

void Player::Draw(Camera *camera) {
	model_->Draw(worldTransform_, texture_, camera);
}
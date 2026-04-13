#include "Fade.h"

using namespace KamataEngine;

Fade::Fade() {}
Fade::~Fade() {}

void Fade::Initialize() {

	sprite_ = Sprite::Create(TextureManager::Load("white1x1.png"),Vector2(0,0));
	sprite_->SetSize(Vector2(1280.0f, 720.0f));
	sprite_->SetColor(Vector4{0, 0, 0, 1});

}

void Fade::Update() {

}

void Fade::Draw() {

	Sprite::PreDraw();
	sprite_->Draw();
	Sprite::PostDraw();


}
#pragma once
#include"KamataEngine.h"
class Fade {

private:

	KamataEngine::Sprite* sprite_ = nullptr;

public:
	Fade();
	~Fade();

	void Initialize();
	void Update();
	void Draw();
};

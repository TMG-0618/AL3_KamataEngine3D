#pragma once
#include "KamataEngine.h"

class Skydome {

private:

	KamataEngine::WorldTransform worldTransform_;

	KamataEngine::Model* model_ = nullptr;
 
public:

	Skydome();
	~Skydome();

	void Initialize();
	void Update();
	void Draw();


};

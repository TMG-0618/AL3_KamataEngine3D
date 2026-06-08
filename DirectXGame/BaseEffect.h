#pragma once
class BaseEffect {
	
public:
	virtual ~BaseEffect() = default;
	virtual void Update() = 0;
	virtual void Draw() = 0;

};
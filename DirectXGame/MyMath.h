#pragma once
#include "KamataEngine.h"

class MyMath {

public:
	//Vector3

	static void InitializeVector3(KamataEngine::Vector3 vector);

	static KamataEngine::Vector3 Add(const KamataEngine::Vector3& v1, const KamataEngine::Vector3& v2);
	static KamataEngine::Vector3 Subtract(const KamataEngine::Vector3& v1, const KamataEngine::Vector3& v2);
	static KamataEngine::Vector3 Multiply(float scalar, const KamataEngine::Vector3& v);
	static float Dot(const KamataEngine::Vector3& v1, const KamataEngine::Vector3& v2);
	static float Length(const KamataEngine::Vector3& v);
	static KamataEngine::Vector3 Normalize(const KamataEngine::Vector3& v);

	//Matrix4x4
	
	static void InitializeMatrix4x4(KamataEngine::Matrix4x4& matrix);
	static KamataEngine::Matrix4x4 Add(const KamataEngine::Matrix4x4& m1, const KamataEngine::Matrix4x4& m2);
	static KamataEngine::Matrix4x4 Subtract(const KamataEngine::Matrix4x4& m1, const KamataEngine::Matrix4x4& m2);
	static KamataEngine::Matrix4x4 Multiply(const KamataEngine::Matrix4x4& m1, const KamataEngine::Matrix4x4& m2);
	static KamataEngine::Matrix4x4 Inverse(const KamataEngine::Matrix4x4& m);
	static KamataEngine::Matrix4x4 Transpose(const KamataEngine::Matrix4x4& m);
	static KamataEngine::Matrix4x4 MakeIdentity();

	static KamataEngine::Matrix4x4 MakeTranslateMatrix(const KamataEngine::Vector3& translate);
	static KamataEngine::Matrix4x4 MakeScaleMatrix(const KamataEngine::Vector3& scale);
	static KamataEngine::Vector3 Transform(const KamataEngine::Vector3& vector, const KamataEngine::Matrix4x4& matrix);

	static KamataEngine::Matrix4x4 MakeRotateXMatrix(float radian);
	static KamataEngine::Matrix4x4 MakeRotateYMatrix(float radian);
	static KamataEngine::Matrix4x4 MakeRotateZMatrix(float radian);

	static KamataEngine::Matrix4x4 MakeAffinMatrix(const KamataEngine::Vector3& scale, const KamataEngine::Vector3& rotate, const KamataEngine::Vector3& translate);
};

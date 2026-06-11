#pragma once
#include"KamataEngine.h"
#include <variant>
#include<map>
#include<json.hpp>


class GlobalVariables {

	using json = nlohmann::json;

public:

	static GlobalVariables* GetInstance();

	void CreateGroup(const std::string& groupName);

	void SetValue(const std::string& groupName, const std::string& key, int32_t value);
	void SetValue(const std::string& groupName, const std::string& key, float value);
	void SetValue(const std::string& groupName, const std::string& key, const KamataEngine::Vector3& value);

	void Update();

	void SaveFile(const std::string& groupName);

private:

	GlobalVariables() = default;
	~GlobalVariables() = default;
	GlobalVariables(const GlobalVariables& obj) = delete;
	GlobalVariables& operator=(const GlobalVariables& obj) = delete;

	struct Item {
		std::variant<int32_t, float, KamataEngine::Vector3> value;
	};

	struct Group {
		std::map<std::string, Item> items;
	};

	std::map<std::string, Group> datas_;

	const std::string kDirectoryPath = "Resources/GlobalVariables/";
};

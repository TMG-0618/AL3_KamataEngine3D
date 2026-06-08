#include "StageManager.h"
#include <fstream>

void StageManager::LoadStageData(const std::string& filename) {
	std::ifstream file;
	file.open(filename);
	assert(file.is_open() && "ステージデータファイルが存在しません");

	std::stringstream stageDataCSV;

	stageDataCSV << file.rdbuf();
	file.close();

	std::string line;
	while (std::getline(stageDataCSV, line)) {

		std::stringstream lineStream(line);

		StageData stageData;

		std::string word;
		std::getline(lineStream, word, ',');

		stageData.name = word;

		std::getline(lineStream, word, ',');
		stageData.timeLimit = std::stoi(word);
		stageDatas_.push_back(stageData);
	}
}

void StageManager::SetCurrentStageIndexByName(const std::string& name) {
	for (size_t i = 0; i < stageDatas_.size(); ++i) {
		if (stageDatas_[i].name == name) {
			currentStageIndex_ = static_cast<int32_t>(i);
			return;
		}
	}
	assert(false && "指定されたステージ名は存在しません");
}
#include "MapChipField.h"
#include <fstream>
#include <map>
#include <sstream>
#include<assert.h>

using namespace KamataEngine;

namespace {

std::map<char, MapChipType> mapChipTypeTable = {
	{'B', MapChipType::kBlock},
	{'P', MapChipType::kPlayer},
	{'E', MapChipType::kEnemy},
};

}

void MapChipField::ResetMapChipData() {

	mapChipData_.data.clear();
	mapChipData_.data.resize(kNumBlockVirtical);

	for (std::vector<MapChipDataUint>& mapChipDataLine : mapChipData_.data) {
		mapChipDataLine.resize(kNumBlockHorizontal);
	}
}

void MapChipField::LoadMapChipCsv(const std::string& filePath) {

	ResetMapChipData();

	std::ifstream file;

	file.open(filePath);

	assert(file.is_open());

	std::stringstream mapChipCsv;

	mapChipCsv << file.rdbuf();

	file.close();

	for (uint32_t i = 0; i < kNumBlockVirtical; ++i) {

		std::string line;

		getline(mapChipCsv, line);

		std::istringstream lineStream(line);

		for (uint32_t j = 0; j < kNumBlockHorizontal; ++j) {

			std::string word;
			std::getline(lineStream, word, ',');


			if (word.empty()) {
				continue;
			}

			if (!mapChipTypeTable.contains(word[kChipType])) {
				continue;
			}

			mapChipData_.data[i][j].type = mapChipTypeTable[word[kChipType]];

			if (word.size() <= kChipSubID) {
				continue;
			}
			mapChipData_.data[i][j].subID = static_cast<uint8_t>(word[kChipSubID] - '0');
		}
	}
}

MapChipType MapChipField::GetMapChipTypeByIndex(uint32_t xIndex, uint32_t yIndex) {

	if (xIndex < 0 || kNumBlockHorizontal - 1 < xIndex) {
		return MapChipType::kBlank;
	}

	if (yIndex < 0 || kNumBlockVirtical - 1 < yIndex) {
		return MapChipType::kBlank;
	}

	return mapChipData_.data[yIndex][xIndex].type;
}

Vector3 MapChipField::GetMapChipPositionByIndex(uint32_t xIndex, uint32_t yIndex) {

	return Vector3(kBlockWidth * xIndex, kBlockHeight * (kNumBlockVirtical - 1 - yIndex), 0);

}

MapChipField::IndexSet MapChipField::GetmapChipIndexSetByPosition(const Vector3& position) { 
IndexSet indexSet = {};
	indexSet.xIndex = static_cast<uint32_t>((position.x + kBlockWidth / 2.0f) / kBlockWidth);

	uint32_t y = static_cast<uint32_t>((position.y + kBlockHeight / 2.0f) / kBlockHeight);
	indexSet.yIndex = static_cast<uint32_t>((kNumBlockVirtical - 1) - y);

	return indexSet;
}

MapChipField::Rect MapChipField::GetRectByIndex(uint32_t xIndex, uint32_t yIndex) {

	Vector3 center = GetMapChipPositionByIndex(xIndex, yIndex);

	Rect rect;
	rect.left = center.x - kBlockWidth / 2.0f;
	rect.right = center.x + kBlockWidth / 2.0f;
	rect.bottom = center.y - kBlockHeight / 2.0f;
	rect.top = center.y + kBlockHeight / 2.0f;

	return rect;
}

uint8_t MapChipField::GetMapChipSubIDByIndex(uint32_t xIndex, uint32_t yIndex) {
	if (xIndex < 0 || kNumBlockHorizontal - 1 < xIndex) {
		return 0;
	}

	if (yIndex < 0 || kNumBlockVirtical - 1 < yIndex) {
		return 0;
	}

	return mapChipData_.data[yIndex][xIndex].subID;
}
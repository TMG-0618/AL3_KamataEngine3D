#pragma once
#include "KamataEngine.h"

enum class MapChipType {
	kBlank,
	kBlock,
};

struct MapChipData {
	std::vector<std::vector<MapChipType>> data;
};

class MapChipField {

private:
	static inline const float kBlockWidth = 1.0f;
	static inline const float kBlockheight = 1.0f;

	static inline const uint32_t kNumBlockVirtical = 20;
	static inline const uint32_t kNumBlockHorizontal = 100;

	MapChipData mapChipData_;

public:

	void ResetMapChipData();

	void LoadMapChipCsv(const std::string& filePath);

};

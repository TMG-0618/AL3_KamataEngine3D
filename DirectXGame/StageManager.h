#pragma once
#include"KamataEngine.h"
#include<cassert>

struct StageData {
	std::string name;
	int32_t timeLimit;
};

class StageManager {

	private:
		std::vector<StageData> stageDatas_;
	    int32_t currentStageIndex_ = 2;

	public:

		void LoadStageData(const std::string& filename);
	    const StageData& GetStageData(int32_t index) const {
		    assert(index >= 0 && index < stageDatas_.size());
		    return stageDatas_[index];
		}

		void SetCurrentStageIndex(int32_t index) { 
			assert(index >= 0 && index < stageDatas_.size());
			currentStageIndex_ = index;
		}

		int32_t GetCurrentStageIndex() const { return currentStageIndex_; }

		const StageData& GetCurrentStageData() const {
		    return GetStageData(currentStageIndex_);
	    }

		void SetCurrentStageIndexByName(const std::string& name);

};

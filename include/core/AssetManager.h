#pragma once
#include "render/StaticModel.h"
#include <unordered_map>
#include <memory>
#include <string>

namespace se
{
	class AssetManager
	{
	public:
		AssetManager() = default;
		~AssetManager() = default;
		const StaticModel& getStaticModel(const std::string& path);

	private:
		std::unordered_map<std::string, StaticModel> staticModelMap;
	};
}
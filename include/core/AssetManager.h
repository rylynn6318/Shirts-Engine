#pragma once
#include "render/StaticModel.h"
#include <unordered_map>
#include <memory>

namespace se
{
	class AssetManager
	{
	public:
		AssetManager() = default;
		~AssetManager() = default;
		const StaticModel& getStaticModel(const char* path);

	private:
		std::unordered_map<const char*, StaticModel> staticModelMap;
	};
}
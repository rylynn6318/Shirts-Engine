#include "core/AssetManager.h"

const se::StaticModel& se::AssetManager::getStaticModel(const char* path)
{
	if (staticModelMap.find(path) != staticModelMap.end())
		return staticModelMap.at(path);
	else
	{
		se::StaticModel staticModel;
		staticModel.loadModel(path);
		staticModelMap.insert({ path, staticModel });

		return staticModel;
	}
}
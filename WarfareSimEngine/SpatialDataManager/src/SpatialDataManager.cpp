#include "SpatialDataManager.h"

#include <cmath>
#include <mutex>
#include <utility>

SpatialDataManager::SpatialDataManager()
{
}

SpatialDataManager::~SpatialDataManager()
{
}

bool SpatialDataManager::RegisterEntity(const SpatialData& spatialData)
{
    std::unique_lock<std::shared_mutex> lock(mutex_);

    const auto [iterator, inserted] = spatialDataMap_.emplace(spatialData.entityId, spatialData);

    return inserted;
}
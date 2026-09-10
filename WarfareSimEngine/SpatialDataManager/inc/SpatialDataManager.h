#pragma once

#include <cstdint>        // std::uint64_t
#include <string>         // std::string
#include <cstdint>
#include <string>
#include <unordered_map>
#include <shared_mutex>

// 3차원 벡터
struct Vector3
{
    double x{ 0.0 };
    double y{ 0.0 };
    double z{ 0.0 };
};

struct SpatialData
{
    std::uint64_t entityId{ 0 };
    std::string entityName;

    Vector3 position;      // 위치(m)
    Vector3 rotation;      // Roll, Pitch, Yaw(degree)
    Vector3 velocity;      // 속도(m/s)
    Vector3 acceleration;  // 가속도(m/s^2)

    double simulationTime{ 0.0 };
};

class SpatialDataManager
{
private:
    std::unordered_map<std::uint64_t, SpatialData> spatialDataMap_;
    mutable std::shared_mutex mutex_;

public:
	SpatialDataManager();
	virtual ~SpatialDataManager();

	// 새로운 객체 등록
	// 같은 ID가 존재하면 false를 반환한다.
	bool RegisterEntity(const SpatialData & spatialData);
};
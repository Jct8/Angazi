#pragma once

namespace Angazi::AI
{
	using Propery = std::variant<int, float, Math::Vector2>;

	//template<class...Property>
	struct MemoryRecord
	{
		//std::unordered_map<std::string, std::variant<Property>> properties;
		std::unordered_map<std::string, Propery> properties;
		uint64_t entityId = 0;
		float lastRecordTime = 0.0f;
		float importance = 0.0f;
	};

	using MemoryRecords = std::list<MemoryRecord>;

	inline MemoryRecord& FindOrCreate(MemoryRecords& records, uint64_t unqiueId)
	{
		auto iter = std::find_if(records.begin(), records.end(), [&unqiueId](const MemoryRecord& record)
		{
			return record.entityId == unqiueId;
		});
		if (iter == records.end())
		{
			MemoryRecord memoryRecord;
			memoryRecord.entityId = unqiueId;
			records.push_back(memoryRecord);
			iter = --records.end();
		}
		return *iter;
	}
}
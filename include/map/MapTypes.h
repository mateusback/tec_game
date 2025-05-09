#ifndef MAP_TYPES_H
#define MAP_TYPES_H
namespace Map
{
	enum class ERoomType {
		Start,
		Normal,
		Treasure,
		Boss
	};

	enum class EEntityType {
		Enemy,
		Item,
		Unknown
	};

	inline EEntityType entityTypeFromString(const std::string& str) {
		static const std::unordered_map<std::string, EEntityType> map = {
			{"Enemy", EEntityType::Enemy},
			{"Item", EEntityType::Item}
		};

		auto it = map.find(str);
		return (it != map.end()) ? it->second : EEntityType::Unknown;
	}

	inline std::string entityTypeToString(EEntityType type) {
		static const std::unordered_map<EEntityType, std::string> map = {
			{EEntityType::Enemy, "Enemy"},
			{EEntityType::Item, "Item"},
			{EEntityType::Unknown, "Unknown"}
		};

		auto it = map.find(type);
		return (it != map.end()) ? it->second : "Unknown";
	}

	inline ERoomType roomTypeFromString(const std::string& str) {
		static const std::unordered_map<std::string, ERoomType> map = {
			{"Start", ERoomType::Start},
			{"Normal", ERoomType::Normal},
			{"Treasure", ERoomType::Treasure},
			{"Boss", ERoomType::Boss}
		};

		auto it = map.find(str);
		return (it != map.end()) ? it->second : ERoomType::Normal;
	}

	inline std::string roomTypeToString(ERoomType type) {
		static const std::unordered_map<ERoomType, std::string> map = {
			{ERoomType::Start, "Start"},
			{ERoomType::Normal, "Normal"},
			{ERoomType::Treasure, "Treasure"},
			{ERoomType::Boss, "Boss"}
		};

		auto it = map.find(type);
		return (it != map.end()) ? it->second : "Normal";
	}

}

#endif

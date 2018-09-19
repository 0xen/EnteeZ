#pragma once
#include <EnteeZ/EntityManager.hpp>
#include <typeindex>
#include <map>

namespace enteez
{
	class EnteeZ
	{
	public:
		EnteeZ();
		~EnteeZ();

		EntityManager& GetEntityManager();


		friend class EntityManager;
	private:
		template<typename T>
		unsigned int GetIndex(std::map<std::type_index, unsigned int>& map);

		EntityManager m_entity_manager;
	};

	template<typename T>
	inline unsigned int EnteeZ::GetIndex(std::map<std::type_index, unsigned int>& map)
	{
		auto found = map.find(std::type_index(typeid(T)));
		if (found == map.end())
		{
			map[std::type_index(typeid(T))] = map.size();
		}
		return map[std::type_index(typeid(T))];
	}
}
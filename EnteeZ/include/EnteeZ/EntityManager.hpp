#pragma once

#include <EnteeZ/Entity.hpp>
#include <functional>
#include <typeindex>
#include <vector>

namespace enteez
{
	template <typename T> struct lambda_function { typedef T definition; };
	class EnteeZ;
	class EntityManager
	{
	public:
		EntityManager(EnteeZ* enteez);
		~EntityManager();
		EntityManager::Entity* CreateEntity();

		void DestroyEntity(Entity* entity);

		template<typename ...components>
		void ForEach(typename lambda_function<std::function<void(Entity* entity, components&...)>>::definition f, bool cache = false);

		template<typename ...components>
		std::vector<Entity*> GetEntitysWith();

		template<typename T>
		unsigned int GetComponentIndex();

		friend class Entity;
	private:
		EnteeZ * m_enteez;
		std::map<std::type_index, unsigned int> m_component_indexs;
		std::vector<Entity*> m_entitys;
	};
	template<typename T>
	inline unsigned int EntityManager::GetComponentIndex()
	{
		return m_enteez->GetIndex<T>(m_component_indexs);
	}

	template<typename ...components>
	inline void EntityManager::ForEach(typename lambda_function<std::function<void(Entity* entity, components&...)>>::definition f, bool cache)
	{
		for (auto entity : m_entitys)
		{
			if (entity->HasComponent<components...>())
				f(entity, entity->GetComponent<components>()...);
		}
	}
	template<typename ...components>
	inline std::vector<Entity*> EntityManager::GetEntitysWith()
	{
		std::vector<Entity*> responce;
		for (auto entity : m_entitys)
		{
			if (entity->HasComponent<components...>())
				responce.push_back(entity);
		}
		return responce;
	}
}
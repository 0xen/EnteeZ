#pragma once

#include <EnteeZ/Entity.hpp>
#include <EnteeZ/TemplatePair.hpp>
#include <functional>
#include <typeindex>
#include <vector>
#include <bitset>

namespace enteez
{
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

		template<typename T>
		unsigned int GetBaseIndex();

		// return all components with base type T
		template<typename T>
		std::vector<unsigned int> GetBaseComponents();

		template<typename T>
		TemplateBase* GetTemplateBase();

		friend class Entity;
	private:

		template<typename T>
		void AddToBitHandler(std::bitset<100>& bs);
		template<typename T, typename V, typename... components>
		void AddToBitHandler(std::bitset<100>& bs);

		EnteeZ * m_enteez;
		std::vector<Entity*> m_entitys;
		std::map<unsigned long long, std::vector<Entity*>> m_cache;
	};

	template<typename T>
	inline unsigned int EntityManager::GetComponentIndex()
	{
		return m_enteez->GetIndex<T>(m_enteez->m_component_indexs);
	}

	template<typename T>
	unsigned int EntityManager::GetBaseIndex()
	{
		return m_enteez->GetIndex<T>(m_enteez->m_base_indexs);
	}

	template<typename ...components>
	inline void EntityManager::ForEach(typename lambda_function<std::function<void(Entity* entity, components&...)>>::definition f, bool cache)
	{
		// Should we generate / access a cache
		if (cache)
		{
			// Create a instance of all bit flags that represent the search
			std::bitset<100> bs;
			// Add the flags top the handler
			AddToBitHandler<components...>(bs);

			auto it = m_cache.find(bs.to_ullong());
			// We found a cache match
			if (it != m_cache.end())
			{
				// Loop through and return all needed data for the request from the cache
				for (auto entity : it->second)
				{
					f(entity, entity->GetComponent<components>()...);
				}
				return;
			}
			// Since we do not have a cache available for this search, we need to loop through all entity's and create a cache
			for (auto entity : m_entitys)
			{
				// If we find a entity that matches the criteria add it to the cache and call its function
				if (entity->HasComponent<components...>())
				{
					f(entity, entity->GetComponent<components>()...);
					m_cache[bs.to_ullong()].push_back(entity);
				}
			}
		}
		else
		{
			// Look through all entity's available
			for (auto entity : m_entitys)
			{
				// If we find a entity that matches the criteria call its function
				if (entity->HasComponent<components...>())
					f(entity, entity->GetComponent<components>()...);
			}
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

	template<typename T>
	std::vector<unsigned int> EntityManager::GetBaseComponents()
	{
		unsigned int base_index = GetBaseIndex<T>();
		return m_enteez->m_component_bases[base_index];
	}

	template<typename T>
	inline TemplateBase* EntityManager::GetTemplateBase()
	{
		unsigned int base_index = GetBaseIndex<T>();
		return m_enteez->m_component_base_templates[base_index];
	}

	template<typename T>
	void EntityManager::AddToBitHandler(std::bitset<100>& bs)
	{
		unsigned int type_index = GetComponentIndex<T>();
		bs.set(type_index);
	}

	template<typename T, typename V, typename... components>
	void EntityManager::AddToBitHandler(std::bitset<100>& bs)
	{
		AddToBitHandler<T>(bs);
		AddToBitHandler<V, components...>(bs);
	}
}
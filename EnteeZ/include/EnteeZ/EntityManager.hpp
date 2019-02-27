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
		// Create a instance of the class with refrence to the enteez manager
		EntityManager(EnteeZ* enteez);
		~EntityManager();
		// Create a new entity with x name
		virtual EntityManager::Entity* CreateEntity(std::string name = "Object");
		// Destory a entity and remove it
		void DestroyEntity(Entity* entity);
		// Remove all entitys
		void Clear();
		template<typename ...components>
		// Loop through all entitys that have all the requested components
		void ForEach(typename lambda_function<std::function<void(Entity* entity, components&...)>>::definition f, bool cache = false);

		template<typename ...components>
		// Get all entitys that have all the requested components
		std::vector<Entity*> GetEntitysWith();
		// Get all entitys
		std::vector<Entity*> GetEntitys();

		template<typename T>
		// Get a components index
		unsigned int GetComponentIndex();

		template<typename T>
		// Get a base class's index
		unsigned int GetBaseIndex();

		template<typename T>
		// Get all components with base type T
		std::vector<unsigned int> GetBaseComponents();

		template<typename T, typename V>
		// Get a Template base with the component top level and base class instance
		TemplateBase* GetTemplateBase();

		template<typename V>
		// Get a Template base with a pre found component index and base inheritance type T
		TemplateBase* GetTemplateBase(unsigned int component_index);
		// Allow Entity to access our private members
		friend class Entity;
	private:

		template<typename T>
		// Add template T to a bitset
		void AddToBitHandler(std::bitset<100>& bs);
		template<typename T, typename V, typename... components>
		// Add multiple template T's to a bitset
		void AddToBitHandler(std::bitset<100>& bs);
		// Local instance of EnteeZ
		EnteeZ * m_enteez;
		// List of all entitys currently alive
		std::vector<Entity*> m_entitys;
		// Local cache storage of what entitys match what component combinations
		std::map<unsigned long long, std::vector<Entity*>> m_cache;
	};

	template<typename T>
	// Get a components index
	inline unsigned int EntityManager::GetComponentIndex()
	{
		return m_enteez->GetIndex<T>(m_enteez->m_component_indexs);
	}

	template<typename T>
	// Get a base class's index
	unsigned int EntityManager::GetBaseIndex()
	{
		return m_enteez->GetIndex<T>(m_enteez->m_base_indexs);
	}

	template<typename ...components>
	// Loop through all entitys that have all the requested components
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
			// Init the cache
			m_cache[bs.to_ullong()] = std::vector<Entity*>();

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
	// Get all entitys that have all the requested components
	inline std::vector<Entity*> EntityManager::GetEntitysWith()
	{
		std::vector<Entity*> responce;
		// Loop through all entitys, if they match the criteria, add them to the array and return them
		for (auto entity : m_entitys)
		{
			if (entity->HasComponent<components...>())
				responce.push_back(entity);
		}
		return responce;
	}

	template<typename T>
	// Get all components with base type T
	std::vector<unsigned int> EntityManager::GetBaseComponents()
	{
		unsigned int base_index = GetBaseIndex<T>();
		return m_enteez->m_component_bases[base_index];
	}

	template<typename T, typename V>
	// Get a Template base with the component top level and base class instance
	inline TemplateBase* EntityManager::GetTemplateBase()
	{
		unsigned int component_index = GetComponentIndex<T>();
		unsigned int base_index = GetBaseIndex<V>();
		return m_enteez->m_component_base_templates[component_index][base_index];
	}

	template<typename V>
	// Get a Template base with a pre found component index and base inheritance type T
	inline TemplateBase* EntityManager::GetTemplateBase(unsigned int component_index)
	{
		unsigned int base_index = GetBaseIndex<V>();
		return m_enteez->m_component_base_templates[component_index][base_index];
	}

	template<typename T>
	// Add template T to a bitset
	void EntityManager::AddToBitHandler(std::bitset<100>& bs)
	{
		unsigned int type_index = GetComponentIndex<T>();
		bs.set(type_index);
	}

	template<typename T, typename V, typename... components>
	// Add multiple template T's to a bitset
	void EntityManager::AddToBitHandler(std::bitset<100>& bs)
	{
		AddToBitHandler<T>(bs);
		AddToBitHandler<V, components...>(bs);
	}
}
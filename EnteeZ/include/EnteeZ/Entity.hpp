#pragma once
#include <iostream>
#include <type_traits>
#include <functional>
#include <map>
#include <vector>
#include <bitset>
#include <typeindex>

#include <EnteeZ/ComponentWrapper.hpp>

namespace enteez
{
	class EntityManager;
	// Define that we will be using a lambda function in x format that is templated
	template <typename T> struct lambda_function { typedef T definition; };

	class Entity
	{
	public:
		// Refrences back to the entity manager
		Entity(EntityManager* entity_manager, std::string name);
		~Entity();

		template <typename T>
		// Add a component to the entity
		ComponentWrapper<T>* AddComponent(T* component);

		template <typename T, typename ... Args>
		// Add a series of components to the entity
		ComponentWrapper<T>* AddComponent(Args&& ... args);
		template <typename T>
		// Get a component from the entity if it exists
		T& GetComponent();
		template <typename T>
		// Remove a component from the entity if it exists
		void RemoveComponent();
		// Remove a component by its pointer
		void RemoveComponent(void* ptr);
		template <typename T>
		// Dose the entity have a component
		bool HasComponent();


		template<typename T, typename V, typename... components>
		// Dose the entity have a series of components
		bool HasComponent();

		template<typename T>
		// Loop through each component that has x base class
		void ForEach(typename lambda_function<std::function<void(Entity* entity, T& t)>>::definition f);

		template<typename T>
		// Loop through each component that has x base class, Return the whole base component wrapper
		void ForEach(typename lambda_function<std::function<void(Entity* entity, BaseComponentWrapper& wrapper, T* t)>>::definition f);
		// Loop through each component
		void ForEach(typename lambda_function<std::function<void(BaseComponentWrapper& wrapper)>>::definition f);
		// Get the entity manager from the entity
		EntityManager& GetManager();
		// Destroy the entity
		void Destroy();
		// Set the entitys name
		void SetName(std::string name);
		// Get the entitys name
		std::string& GetName();
		// Get the total component count on the entity
		unsigned int GetComponentCount();

	private:
		// Remove a component by internal ID
		void RemoveComponent(unsigned int type_index);

		std::string m_name;                                           // Entity Name
		std::bitset<100> m_component_flags;                           // What components dose the entity have, represented by a bitset
		std::map<unsigned int, BaseComponentWrapper*> m_components;   // A map of all components attached to the entity
		EntityManager* m_entity_manager;                              // Local instance of the entity manager
		unsigned int m_component_count;                               // Running count of the amount of components attached to the entity
	};

	template <typename T>
	// Add a component to the entity
	inline ComponentWrapper<T>* Entity::AddComponent(T* component)
	{
		unsigned int index = m_entity_manager->GetComponentIndex<T>();
		// If the bitset dose not contain the component, increment the counter
		if (!m_component_flags.test(index)) m_component_count++;
		m_component_flags.set(index);
		// Generate a new wrapper for the component
		ComponentWrapper<T>* wrapper = new ComponentWrapper<T>(component,sizeof(T), index, false);
		m_components[index] = wrapper;

		// Update cache's
		for (auto& it = m_entity_manager->m_cache.begin(); it != m_entity_manager->m_cache.end(); it++)
		{
			std::bitset<100> components(it->first);
			// If the entity now matches the cache bitset, continue
			if ((m_component_flags & components) == components)
			{
				// If the cache dose not contain the component, add it to the cache
				if (std::find(it->second.begin(), it->second.end(), this) == it->second.end())
				{
					it->second.push_back(this);
				}
			}
		}
		return wrapper;
	}

	template<typename T, typename ...Args>
	// Add a series of components to the entity
	inline ComponentWrapper<T>* Entity::AddComponent(Args && ...args)
	{
		T* t = new T(std::forward<Args>(args) ...);
		unsigned int index = m_entity_manager->GetComponentIndex<T>();
		// If the bitset dose not contain the component, increment the counter
		if (!m_component_flags.test(index)) m_component_count++;
		m_component_flags.set(index);
		// Generate a new wrapper for the component
		ComponentWrapper<T>* wrapper = new ComponentWrapper<T>(t, sizeof(T), index);
		m_components[index] = wrapper;

		// Update cache's
		for (auto& it = m_entity_manager->m_cache.begin(); it != m_entity_manager->m_cache.end(); it++)
		{
			std::bitset<100> components(it->first);
			// If the entity now matches the cache bitset, continue
			if ((m_component_flags & components) == components)
			{
				// If the cache dose not contain the component, add it to the cache
				if (std::find(it->second.begin(), it->second.end(), this) == it->second.end())
				{
					it->second.push_back(this);
				}
			}
		}
		return wrapper;
	}

	template<typename T>
	// Get a component from the entity if it exists
	inline T& Entity::GetComponent()
	{
		unsigned int type_index = m_entity_manager->GetComponentIndex<T>();
		// Get the compnent from the map and cast it to its wrapper type
		ComponentWrapper<T>* wrapper = reinterpret_cast<ComponentWrapper<T>*>(m_components[type_index]);
		// Return a refrence instance back to the origional compoent, but in the corrent type (void* -> T&)
		return wrapper->Get();
	}

	template<typename T>
	// Remove a component from the entity if it exists
	inline void Entity::RemoveComponent()
	{
		unsigned int type_index = m_entity_manager->GetComponentIndex<T>();
		// Remove a component with id x
		RemoveComponent(type_index);
	}

	template<typename T>
	// Dose the entity have a component
	inline bool Entity::HasComponent()
	{
		unsigned int type_index = m_entity_manager->GetComponentIndex<T>();
		// Return true if the bitset has the component bit toggled
		return m_component_flags.test(type_index);
	}

	template<typename T, typename V, typename... components>
	// Dose the entity have a series of components
	inline bool Entity::HasComponent()
	{
		// Loop through all requested components and return if it contains it
		return HasComponent<T>() && HasComponent<V, components...>();
	}

	template<typename T>
	// Loop through each component that has x base class
	inline void Entity::ForEach(typename lambda_function<std::function<void(Entity* entity, T& t)>>::definition f)
	{
		std::vector<unsigned int>& component_bases = m_entity_manager->GetBaseComponents<T>();
		// Loop through all components that use base class T
		for (auto base : component_bases)
		{
			// If we have a component that matches the reqired base class, continue
			if (m_component_flags.test(base))
			{
				TemplateBase* tb = m_entity_manager->GetTemplateBase<T>(m_components[base]->GetID());
				TemplateStorage<T>& ts = static_cast<TemplateStorage<T>&>(*tb);
				T* t = ts.Get(m_components[base]->GetComponentPtr());
				// Return a instance to that component
				f(this, *t);
			}
		}
	}

	template<typename T>
	// Loop through each component that has x base class
	inline void Entity::ForEach(typename lambda_function<std::function<void(Entity* entity, BaseComponentWrapper& wrapper, T* t)>>::definition f)
	{
		std::vector<unsigned int>& component_bases = m_entity_manager->GetBaseComponents<T>();
		// Loop through all components that use base class T
		for (auto base : component_bases)
		{
			// If we have a component that matches the reqired base class, continue
			if (m_component_flags.test(base))
			{
				TemplateBase* tb = m_entity_manager->GetTemplateBase<T>(m_components[base]->GetID());
				TemplateStorage<T>& ts = static_cast<TemplateStorage<T>&>(*tb);
				T* t = ts.Get(m_components[base]->GetComponentPtr());
				// Return a instance to that component
				f(this, *m_components[base], t);
			}
		}
	}

	// Remove a component by its pointer
	inline void Entity::RemoveComponent(void * ptr)
	{
		// Loop through all components untill we find one that match the ptr, remove it and return
		for (auto it = m_components.begin(); it != m_components.end(); it++)
		{
			if (it->second->GetComponentPtr() == ptr)
			{
				unsigned int type_index = it->second->GetID();
				RemoveComponent(type_index);
				return;
			}
		}
	}

	// Loop through each component
	inline void Entity::ForEach(typename lambda_function<std::function<void(BaseComponentWrapper& wrapper)>>::definition f)
	{
		// Loop through each component and return them through the lambda
		for (auto it = m_components.begin(); it != m_components.end(); it++)
		{
			f(*it->second);
		}
	}
}
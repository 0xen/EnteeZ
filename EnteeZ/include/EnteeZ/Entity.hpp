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
	template <typename T> struct lambda_function { typedef T definition; };
	class Entity
	{
	public:
		Entity(EntityManager* entity_manager, std::string name);
		~Entity();

		template <typename T>
		ComponentWrapper<T>* AddComponent(T* component);

		template <typename T, typename ... Args>
		ComponentWrapper<T>* AddComponent(Args&& ... args);



		template <typename T>
		T& GetComponent();

		template <typename T>
		void RemoveComponent();

		void RemoveComponent(void* ptr);

		template <typename T>
		bool HasComponent();

		template<typename T, typename V, typename... components>
		bool HasComponent();

		template<typename T>
		void ForEach(typename lambda_function<std::function<void(Entity* entity, T& t)>>::definition f);

		template<typename T>
		void ForEach(typename lambda_function<std::function<void(Entity* entity, BaseComponentWrapper& wrapper, T* t)>>::definition f);

		void ForEach(typename lambda_function<std::function<void(BaseComponentWrapper& wrapper)>>::definition f);

		EntityManager& GetManager();

		void Destroy();

		void SetName(std::string name);

		std::string& GetName();

		unsigned int GetComponentCount();

	private:

		void RemoveComponent(unsigned int type_index);

		std::string m_name;
		std::bitset<100> m_component_flags;
		std::map<unsigned int, BaseComponentWrapper*> m_components;
		EntityManager* m_entity_manager;
		unsigned int m_component_count;
	};

	template <typename T>
	inline ComponentWrapper<T>* Entity::AddComponent(T* component)
	{
		unsigned int index = m_entity_manager->GetComponentIndex<T>();
		if (!m_component_flags.test(index)) m_component_count++;
		m_component_flags.set(index);
		ComponentWrapper<T>* wrapper = new ComponentWrapper<T>(component,sizeof(T), index, false);
		m_components[index] = wrapper;

		// Update cache's
		for (auto& it = m_entity_manager->m_cache.begin(); it != m_entity_manager->m_cache.end(); it++)
		{
			//std::bitset<100>(it->first);
			std::bitset<100> components(it->first);
			if ((m_component_flags & components) == components)
			{
				if (std::find(it->second.begin(), it->second.end(), this) == it->second.end())
				{
					it->second.push_back(this);
				}
			}
		}
		return wrapper;
	}

	template<typename T, typename ...Args>
	inline ComponentWrapper<T>* Entity::AddComponent(Args && ...args)
	{
		T* t = new T(std::forward<Args>(args) ...);
		unsigned int index = m_entity_manager->GetComponentIndex<T>();
		if (!m_component_flags.test(index)) m_component_count++;
		m_component_flags.set(index);

		ComponentWrapper<T>* wrapper = new ComponentWrapper<T>(t, sizeof(T), index);
		m_components[index] = wrapper;

		// Update cache's
		for (auto& it = m_entity_manager->m_cache.begin(); it != m_entity_manager->m_cache.end(); it++)
		{
			//std::bitset<100>(it->first);
			std::bitset<100> components(it->first);
			if ((m_component_flags & components) == components)
			{
				if (std::find(it->second.begin(), it->second.end(), this) == it->second.end())
				{
					it->second.push_back(this);
				}
			}
		}
		return wrapper;
	}

	template<typename T>
	inline T& Entity::GetComponent()
	{
		unsigned int type_index = m_entity_manager->GetComponentIndex<T>();
		ComponentWrapper<T>* wrapper = reinterpret_cast<ComponentWrapper<T>*>(m_components[type_index]);
		return wrapper->Get();
	}

	template<typename T>
	inline void Entity::RemoveComponent()
	{
		unsigned int type_index = m_entity_manager->GetComponentIndex<T>();
		RemoveComponent(type_index);
	}

	template<typename T>
	inline bool Entity::HasComponent()
	{
		unsigned int type_index = m_entity_manager->GetComponentIndex<T>();
		return m_component_flags.test(type_index);
	}

	template<typename T, typename V, typename... components>
	inline bool Entity::HasComponent()
	{
		return HasComponent<T>() && HasComponent<V, components...>();
	}

	template<typename T>
	inline void Entity::ForEach(typename lambda_function<std::function<void(Entity* entity, T& t)>>::definition f)
	{
		std::vector<unsigned int> component_bases = m_entity_manager->GetBaseComponents<T>();
		for (auto base : component_bases)
		{
			if (m_component_flags.test(base))
			{
				TemplateBase* tb = m_entity_manager->GetTemplateBase<T>(m_components[base]->GetID());
				TemplateStorage<T>& ts = static_cast<TemplateStorage<T>&>(*tb);
				T* t = ts.Get(m_components[base]->GetComponentPtr());
				f(this, *t);
			}
		}
	}

	template<typename T>
	inline void Entity::ForEach(typename lambda_function<std::function<void(Entity* entity, BaseComponentWrapper& wrapper, T* t)>>::definition f)
	{
		std::vector<unsigned int> component_bases = m_entity_manager->GetBaseComponents<T>();
		for (auto base : component_bases)
		{
			if (m_component_flags.test(base))
			{
				TemplateBase* tb = m_entity_manager->GetTemplateBase<T>(m_components[base]->GetID());
				TemplateStorage<T>& ts = static_cast<TemplateStorage<T>&>(*tb);
				T* t = ts.Get(m_components[base]->GetComponentPtr());
				f(this, *m_components[base], t);
			}
		}
	}

	inline void Entity::RemoveComponent(void * ptr)
	{
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

	inline void Entity::ForEach(typename lambda_function<std::function<void(BaseComponentWrapper& wrapper)>>::definition f)
	{
		for (auto it = m_components.begin(); it != m_components.end(); it++)
		{
			f(*it->second);
		}
	}
}
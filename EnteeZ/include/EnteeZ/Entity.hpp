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
		Entity(EntityManager* entity_manager);
		~Entity();


		template <typename T, typename ... Args>
		ComponentWrapper<T>* AddComponent(Args&& ... args);

		template <typename T>
		T& GetComponent();

		template <typename T>
		void RemoveComponent();

		template <typename T>
		bool HasComponent();

		template<typename T, typename V, typename... components>
		bool HasComponent();

		template<typename T>
		void ForEach(typename lambda_function<std::function<void(Entity* entity, T& t)>>::definition f);

		EntityManager& GetManager();

		void Destroy();

	private:
		std::bitset<100> m_component_flags;
		std::map<unsigned int, BaseComponentWrapper*> m_components;
		EntityManager* m_entity_manager;
	};

	template<typename T, typename ...Args>
	inline ComponentWrapper<T>* Entity::AddComponent(Args && ...args)
	{
		T* t = new T(std::forward<Args>(args) ...);
		unsigned int index = m_entity_manager->GetComponentIndex<T>();
		m_component_flags.set(index);

		ComponentWrapper<T>* wrapper = new ComponentWrapper<T>(t);
		m_components[index] = wrapper;
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
		auto it = m_components.find(type_index);
		if (it != m_components.end())
		{
			m_components.erase(it);
		}
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




				//ComponentWrapper<T>* wrapper = reinterpret_cast<ComponentWrapper<T>*>(m_components[base]);

				//f(this, &wrapper->Get());

				TemplateBase* tb = m_entity_manager->GetTemplateBase<T>();

				TemplateStorage<T>& ts = static_cast<TemplateStorage<T>&>(*tb);
				T& t = ts.Get(m_components[base]->GetComponentPtr());

				f(this, t);
			}
		}
	}
}
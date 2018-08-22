#pragma once
#include <iostream>
#include <type_traits>
#include <bitset>
#include <map>

#include <EnteeZ/ComponentWrapper.hpp>

namespace enteez
{
	class EnteeZ;

	class Entity
	{
	public:
		Entity(EnteeZ* entity_manager);


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

		EnteeZ& GetManager();

		void Destroy();

	private:
		std::bitset<40> m_component_flags;
		std::map<unsigned int, BaseComponentWrapper*> m_components;
		EnteeZ* m_entity_manager;
	};
	template<typename T, typename ...Args>
	inline ComponentWrapper<T>* Entity::AddComponent(Args && ...args)
	{
		T t(std::forward<Args>(args) ...);
		unsigned int index = m_entity_manager->GetTypeIndex<T>();
		m_component_flags.set(index);

		ComponentWrapper<T>* wrapper = new ComponentWrapper<T>(t);
		m_components[index] = wrapper;
		return wrapper;
	}
	template<typename T>
	inline T& Entity::GetComponent()
	{
		unsigned int type_index = m_entity_manager->GetTypeIndex<T>();
		ComponentWrapper<T>* wrapper = reinterpret_cast<ComponentWrapper<T>*>(m_components[type_index]);
		return wrapper->Get();
	}
	template<typename T>
	inline void Entity::RemoveComponent()
	{
		unsigned int type_index = m_entity_manager->GetTypeIndex<T>();
		auto it = m_components.find(type_index);
		if (it != m_components.end())
		{
			m_components.erase(it);
		}
	}
	template<typename T>
	inline bool Entity::HasComponent()
	{
		unsigned int type_index = m_entity_manager->GetTypeIndex<T>();
		return m_component_flags.test(type_index);
	}

	template<typename T, typename V, typename... components>
	inline bool Entity::HasComponent()
	{
		return HasComponent<T>() && HasComponent<V, components...>();
	}
}
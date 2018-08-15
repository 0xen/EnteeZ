#pragma once
#include <iostream>
#include <type_traits>
#include <bitset>

namespace enteez
{
	class EnteeZ;

	class Entity
	{
	public:
		Entity(EnteeZ* entity_manager, unsigned int id);


		template <typename T, typename ... Args>
		T AddComponent(Args&& ... args);

		template <typename T>
		bool HasComponent();

	private:
		std::bitset<40> m_components;
		unsigned int m_id;
		EnteeZ* m_entity_manager;
	};
	template<typename T, typename ...Args>
	inline T Entity::AddComponent(Args && ...args)
	{
		T t(std::forward<Args>(args) ...);
		m_components.set(m_entity_manager->GetTypeIndex<T>());
		return t;
	}
	template<typename T>
	inline bool Entity::HasComponent()
	{
		return m_components.test(m_entity_manager->GetTypeIndex<T>());
	}
}
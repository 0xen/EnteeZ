#pragma once
#include <EnteeZ/Entity.hpp>
#include <functional>
#include <typeindex>
#include <map>
#include <vector>

namespace enteez
{
	template <typename T> struct lambda_function { typedef T definition; };
	class EnteeZ
	{
	public:
		EnteeZ();

		EnteeZ::Entity* CreateEntity();

		template<typename ...components>
		void ForEach(typename lambda_function<std::function<void(components&...)>>::definition f);

		friend class Entity;
	private:
		template<typename T>
		unsigned int GetTypeIndex();

		std::map<std::type_index, unsigned int> m_component_indexs;
		std::vector<Entity*> m_entitys;
		unsigned int m_registered_component_count = 0;
		unsigned int m_current_entity_count = 0;
	};


	template<typename ...components>
	inline void EnteeZ::ForEach(typename lambda_function<std::function<void(components&...)>>::definition f)
	{
		for (auto entity : m_entitys)
		{
			//*(it.template component<Components>().get())
			if (entity->HasComponent<components...>())
				f(entity->GetComponent<components>()...);
		}
	}
	template<typename T>
	inline unsigned int EnteeZ::GetTypeIndex()
	{
		auto found = m_component_indexs.find(std::type_index(typeid(T)));
		if (found == m_component_indexs.end())
		{
			m_component_indexs[std::type_index(typeid(T))] = m_registered_component_count;
			m_registered_component_count++;
		}
		return m_component_indexs[std::type_index(typeid(T))];
	}
}
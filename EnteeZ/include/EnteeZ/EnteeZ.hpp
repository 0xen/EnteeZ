#pragma once
#include <EnteeZ/Entity.hpp>
#include <functional>
#include <typeindex>
#include <map>

namespace enteez
{
	template <typename T> struct lambda_function { typedef T definition; };
	class EnteeZ
	{
	public:
		EnteeZ();

		EnteeZ::Entity CreateEntity();

		template<typename ...components>
		void ForEach(typename lambda_function<std::function<void(Entity, components ...)>>::definition f);

		friend class Entity;
	private:
		template<typename T>
		unsigned int GetTypeIndex();

		std::map<std::type_index, unsigned int> m_component_indexs;
		unsigned int m_registered_component_count = 0;
		unsigned int m_current_entity_count = 0;
	};


	template<typename ...components>
	inline void EnteeZ::ForEach(typename lambda_function<std::function<void(Entity, components ...)>>::definition f)
	{
		//f(CreateEntity(),1);
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
#pragma once
#include <EnteeZ/EntityManager.hpp>
#include <EnteeZ\TemplatePair.hpp>
#include <typeindex>
#include <map>
#include <vector>

namespace enteez
{
	class EnteeZ
	{
	public:
		EnteeZ();
		~EnteeZ();

		EntityManager& GetEntityManager();

		template<typename T, typename V>
		void RegisterBase();
		template<typename T, typename A, typename B, typename... bases>
		void RegisterBase();


		friend class EntityManager;
	private:
		template<typename T>
		unsigned int GetIndex(std::map<std::type_index, unsigned int>& map);

		std::map<std::type_index, unsigned int> m_base_indexs;
		std::map<std::type_index, unsigned int> m_component_indexs;

		std::map<unsigned int, std::vector<unsigned int>> m_component_bases;
		std::map<unsigned int, std::map<unsigned int, TemplateBase*>> m_component_base_templates;

		EntityManager m_entity_manager;
	};

	template<typename T, typename V>
	inline void EnteeZ::RegisterBase()
	{
		unsigned int component_index = GetIndex<T>(m_component_indexs);
		unsigned int base_index = GetIndex<V>(m_base_indexs);
		m_component_base_templates[component_index][base_index] = new TemplatePair<T, V>();
		auto it = std::find(m_component_bases[base_index].begin(), m_component_bases[base_index].end(), component_index);
		if (it == m_component_bases[base_index].end())
		{
			m_component_bases[base_index].push_back(component_index);
		}
	}

	template<typename T, typename A, typename B, typename... bases>
	inline void EnteeZ::RegisterBase()
	{
		RegisterBase<T, A>();
		RegisterBase<T, B, bases...>();
	}

	template<typename T>
	inline unsigned int EnteeZ::GetIndex(std::map<std::type_index, unsigned int>& map)
	{
		auto found = map.find(std::type_index(typeid(T)));
		if (found == map.end())
		{
			map[std::type_index(typeid(T))] = map.size();
			return map.size() - 1;
		}
		return found->second;
	}
}
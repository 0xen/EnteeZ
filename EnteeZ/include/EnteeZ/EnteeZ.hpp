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
		// Create a instance of EnteeZ (Entity Component Manager)
		EnteeZ();
		// Handle destruction of the object
		~EnteeZ();
		// Get the local instance of the entity manager
		// Entity manager is used to create and destroy entity instances
		EntityManager& GetEntityManager();

		template<typename T, typename V>
		// Links internaly a object to what base classes it inherits from, EG:
		// class PlayerMovment : public Movment -> RegisterBase<PlayerMovment, Movment>();
		void RegisterBase();
		template<typename T, typename A, typename B, typename... bases>
		// Links internaly a object to what base classes it inherits from, EG:
		// class PlayerMovment : public Movment, public KeyboardHandler -> RegisterBase<PlayerMovment, Movment, KeyboardHandler>();
		void RegisterBase();
		// Allow entity manager access to our private members
		friend class EntityManager;
	private:
		template<typename T>
		// Get component index using x index storage map
		unsigned int GetIndex(std::map<std::type_index, unsigned int>& map);

		// Stores objects that are inherited from a component
		std::map<std::type_index, unsigned int> m_base_indexs;
		// Used to store the top level component
		std::map<std::type_index, unsigned int> m_component_indexs;
		// Stores What classes a component inherits
		std::map<unsigned int, std::vector<unsigned int>> m_component_bases;
		// Stores all the casting information required to cast void* to base class whilest retaining the top level inheritance
		std::map<unsigned int, std::map<unsigned int, TemplateBase*>> m_component_base_templates;
		// Local instance of the entity manager
		EntityManager m_entity_manager;
	};

	template<typename T, typename V>
	// Links internaly a object to what base classes it inherits from, EG:
	// class PlayerMovment : public Movment -> RegisterBase<PlayerMovment, Movment>();
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
	// Links internaly a object to what base classes it inherits from, EG:
	// class PlayerMovment : public Movment, public KeyboardHandler -> RegisterBase<PlayerMovment, Movment, KeyboardHandler>();
	inline void EnteeZ::RegisterBase()
	{
		// Extract the first component that is inherited and store it
		RegisterBase<T, A>();
		// Call itself if there are 2+ base classes left or add if 1 left (as 'bases' will be 0 and we will be calling <T, B>)
		RegisterBase<T, B, bases...>();
	}

	template<typename T>
	// Get component index using x index storage map
	inline unsigned int EnteeZ::GetIndex(std::map<std::type_index, unsigned int>& map)
	{
		auto found = map.find(std::type_index(typeid(T)));
		// If the component was not found
		if (found == map.end())
		{
			// Add component to the map
			map[(std::type_index)std::type_index(typeid(T))] = (unsigned int)map.size();
			return (unsigned int)map.size() - 1;
		}
		// Return the component index
		return found->second;
	}
}
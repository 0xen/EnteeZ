#include <EnteeZ/Entity.hpp>
#include <EnteeZ/EnteeZ.hpp>

using namespace enteez;

Entity::Entity(EntityManager* entity_manager, std::string name) : m_name(name)
{
	m_entity_manager = entity_manager;
}

enteez::Entity::~Entity()
{
	for (auto it = m_components.begin(); it != m_components.end(); it++)
	{
		delete it->second;
	}
	m_components.clear();
}

EntityManager & enteez::Entity::GetManager()
{
	return *m_entity_manager;
}

void enteez::Entity::Destroy()
{
	m_entity_manager->DestroyEntity(this);
}

void enteez::Entity::SetName(std::string name)
{
	m_name = name;
}

std::string& enteez::Entity::GetName()
{
	return m_name;
}

void enteez::Entity::RemoveComponent(unsigned int type_index)
{
	std::bitset<100> last_flags = m_component_flags;
	m_component_flags.set(type_index, false);
	auto it = m_components.find(type_index);
	if (it != m_components.end())
	{
		//ComponentWrapper<T>* wrapper = reinterpret_cast<ComponentWrapper<T>*>(it->second);
		BaseComponentWrapper* wrapper = it->second;
		delete wrapper;
		m_components.erase(it);
	}
	// Update cache's
	for (auto& it = m_entity_manager->m_cache.begin(); it != m_entity_manager->m_cache.end(); it++)
	{
		//std::bitset<100>(it->first);
		std::bitset<100> components(it->first);
		// If the components was valid for the cache but the new one is not, remove it
		if ((last_flags & components) == components && (m_component_flags & components) != components)
		{
			// Search for the entity
			auto& search = std::find(it->second.begin(), it->second.end(), this);
			// If the item was found, remove the entity
			if (search != it->second.end())
			{
				it->second.erase(search);
			}
		}
	}
}

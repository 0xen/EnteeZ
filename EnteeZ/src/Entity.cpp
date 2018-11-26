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

std::string enteez::Entity::GetName()
{
	return m_name;
}

#include <EnteeZ/Entity.hpp>
#include <EnteeZ/EnteeZ.hpp>

using namespace enteez;

Entity::Entity(EnteeZ* entity_manager)
{
	m_entity_manager = entity_manager;
}

EnteeZ & enteez::Entity::GetManager()
{
	return *m_entity_manager;
}

void enteez::Entity::Destroy()
{
	m_entity_manager->DestroyEntity(this);
}

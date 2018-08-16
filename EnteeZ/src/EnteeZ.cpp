#include <EnteeZ/EnteeZ.hpp>

using namespace enteez;

EnteeZ::EnteeZ()
{
}

Entity* EnteeZ::CreateEntity()
{
	Entity* entity = new Entity(this, m_current_entity_count);
	m_entitys.push_back(entity);
	m_current_entity_count++;
	return entity;
}

#include <EnteeZ/EnteeZ.hpp>

using namespace enteez;

EnteeZ::EnteeZ()
{
}

Entity EnteeZ::CreateEntity()
{
	Entity entity(this, m_current_entity_count);
	m_current_entity_count++;
	return entity;
}

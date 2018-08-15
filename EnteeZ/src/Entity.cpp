#include <EnteeZ/Entity.hpp>
#include <EnteeZ/EnteeZ.hpp>

using namespace enteez;

Entity::Entity(EnteeZ* entity_manager, unsigned int id)
{
	m_entity_manager = entity_manager;
	m_id = id;
}

#include <EnteeZ/EnteeZ.hpp>

using namespace enteez;

EnteeZ::EnteeZ() : m_entity_manager(this)
{

}

enteez::EnteeZ::~EnteeZ()
{
	for (auto it = m_component_base_templates.begin(); it != m_component_base_templates.end(); it++)
	{
		delete it->second;
	}
}

EntityManager & enteez::EnteeZ::GetEntityManager()
{
	return m_entity_manager;
}
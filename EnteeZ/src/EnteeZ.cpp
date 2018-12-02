#include <EnteeZ/EnteeZ.hpp>

using namespace enteez;

EnteeZ::EnteeZ() : m_entity_manager(this)
{

}

enteez::EnteeZ::~EnteeZ()
{
	for (auto it1 = m_component_base_templates.begin(); it1 != m_component_base_templates.end(); it1++)
	{
		for (auto it2 = it1->second.begin(); it2 != it1->second.end(); it2++)
		{
			delete it2->second;
		}
	}
}

EntityManager & enteez::EnteeZ::GetEntityManager()
{
	return m_entity_manager;
}
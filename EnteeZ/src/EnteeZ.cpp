#include <EnteeZ/EnteeZ.hpp>

using namespace enteez;

EnteeZ::EnteeZ() : m_entity_manager(this)
{

}

enteez::EnteeZ::~EnteeZ()
{

}

EntityManager & enteez::EnteeZ::GetEntityManager()
{
	return m_entity_manager;
}
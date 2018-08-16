#include <EnteeZ/EnteeZ.hpp>

using namespace enteez;

EnteeZ::EnteeZ()
{
}

Entity* EnteeZ::CreateEntity()
{
	Entity* entity = new Entity(this);
	m_entitys.push_back(entity);
	return entity;
}

void enteez::EnteeZ::DestroyEntity(Entity * entity)
{
	auto it = std::find(m_entitys.begin(), m_entitys.end(), entity);
	if (it != m_entitys.end())
	{
		m_entitys.erase(it);
	}
	delete entity;
}

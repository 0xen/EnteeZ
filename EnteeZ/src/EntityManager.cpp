#include <EnteeZ/EntityManager.hpp>

using namespace enteez;

enteez::EntityManager::EntityManager(EnteeZ * enteez) : m_enteez(enteez)
{
}

enteez::EntityManager::~EntityManager()
{
	for (auto e : m_entitys)
	{
		delete e;
	}
	m_entitys.clear();
}


Entity* EntityManager::CreateEntity()
{
	Entity* entity = new Entity(this);
	m_entitys.push_back(entity);
	return entity;
}

void enteez::EntityManager::DestroyEntity(Entity * entity)
{
	auto it = std::find(m_entitys.begin(), m_entitys.end(), entity);
	if (it != m_entitys.end())
	{
		m_entitys.erase(it);
	}
	delete entity;
}

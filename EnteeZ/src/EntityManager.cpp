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
	// Remove the entity from the cache
	for (auto& it = m_cache.begin(); it != m_cache.end(); it++)
	{
		// Search for the entity
		auto& search = std::find(it->second.begin(), it->second.end(), entity);
		// If the item was found, remove the entity
		if (search != it->second.end())
		{
			it->second.erase(search);
		}
	}
	delete entity;
}

void enteez::EntityManager::Clear()
{
	for (auto it : m_entitys)
	{
		delete it;
	}
	m_entitys.clear();
	m_cache.clear();
}

std::vector<Entity*> enteez::EntityManager::GetEntitys()
{
	return m_entitys;
}

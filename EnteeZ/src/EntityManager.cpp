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


Entity* EntityManager::CreateEntity(std::string name)
{
	Entity* entity = new Entity(this, name);
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
	std::vector<Entity*> temp = m_entitys;
	m_entitys.clear();
	m_cache.clear();
	for (auto it : temp)
	{
		delete it;
	}
}

std::vector<Entity*> enteez::EntityManager::GetEntitys()
{
	return m_entitys;
}

bool enteez::EntityManager::ValidEntity(Entity* entity)
{
	for (auto it : m_entitys)
	{
		if (it == entity)return true;
	}
	return false;
}

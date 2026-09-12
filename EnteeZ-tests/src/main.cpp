#include <EnteeZ/EnteeZ.hpp>

#include <iostream>
#include <string>
#include <utility>
#include <vector>

// Minimal assert-style test harness, keeps the library dependency free
static int g_failures = 0;
static int g_checks = 0;

#define TEST_ASSERT(cond)                                                              \
	do                                                                                 \
	{                                                                                  \
		g_checks++;                                                                    \
		if (!(cond))                                                                   \
		{                                                                              \
			g_failures++;                                                              \
			std::cout << "FAILED: " << #cond << " (" << __FUNCTION__ << ":" << __LINE__ << ")" << std::endl; \
		}                                                                              \
	} while (0)

struct Position
{
	Position(float x, float y, float z) : x(x), y(y), z(z) {}
	float x;
	float y;
	float z;
};

struct Velocity
{
	Velocity(float x, float y, float z) : x(x), y(y), z(z) {}
	float x;
	float y;
	float z;
};

struct Logic
{
	virtual ~Logic() {}
	virtual void Update() = 0;
};

struct Tag
{
	virtual ~Tag() {}
};

struct Mover : public Logic, public Tag
{
	Mover(enteez::Entity* entity) : m_entity(entity) {}

	virtual void Update()
	{
		if (m_entity->HasComponent<Position>())
			m_entity->GetComponent<Position>().z += 1.0f;
	}
private:
	enteez::Entity* m_entity;
};

// Add / Has / Get / Remove basics
void TestBasics()
{
	enteez::EnteeZ ez;
	enteez::EntityManager& em = ez.GetEntityManager();

	enteez::Entity* entity = em.CreateEntity("Basics");
	TEST_ASSERT(em.ValidEntity(entity));
	TEST_ASSERT(entity->GetName() == "Basics");
	TEST_ASSERT(!entity->HasComponent<Position>());
	TEST_ASSERT(entity->GetComponentCount() == 0);

	entity->AddComponent<Position>(1.0f, 2.0f, 3.0f);
	TEST_ASSERT(entity->HasComponent<Position>());
	TEST_ASSERT(entity->GetComponentCount() == 1);

	Position& position = entity->GetComponent<Position>();
	TEST_ASSERT(position.x == 1.0f && position.y == 2.0f && position.z == 3.0f);

	entity->AddComponent<Velocity>(0.0f, 0.0f, 0.0f);
	TEST_ASSERT((entity->HasComponent<Position, Velocity>()));
	TEST_ASSERT(entity->GetComponentCount() == 2);

	entity->RemoveComponent<Velocity>();
	TEST_ASSERT(!entity->HasComponent<Velocity>());
	TEST_ASSERT(entity->HasComponent<Position>());
	TEST_ASSERT(entity->GetComponentCount() == 1);

	entity->Destroy();
	TEST_ASSERT(!em.ValidEntity(entity));
	TEST_ASSERT(em.GetEntitys().size() == 0);
}

// AddComponent(T*) leaves ownership with the caller
void TestCallerOwnedComponent()
{
	enteez::EnteeZ ez;
	enteez::EntityManager& em = ez.GetEntityManager();

	Position* position = new Position(5.0f, 6.0f, 7.0f);
	enteez::Entity* entity = em.CreateEntity();
	entity->AddComponent<Position>(position);
	TEST_ASSERT(&entity->GetComponent<Position>() == position);

	entity->Destroy();
	// The entity must not have deleted the caller-owned component
	TEST_ASSERT(position->x == 5.0f);
	delete position;
}

// Re-adding a component replaces the old one without leaking or double counting
void TestDuplicateAddReplaces()
{
	enteez::EnteeZ ez;
	enteez::EntityManager& em = ez.GetEntityManager();

	enteez::Entity* entity = em.CreateEntity();
	entity->AddComponent<Position>(1.0f, 1.0f, 1.0f);
	entity->AddComponent<Position>(9.0f, 9.0f, 9.0f);

	TEST_ASSERT(entity->GetComponentCount() == 1);
	TEST_ASSERT(entity->GetComponent<Position>().x == 9.0f);
	entity->Destroy();
}

// ForEach matching, with and without the cache, and incremental cache updates
void TestForEachAndCache()
{
	enteez::EnteeZ ez;
	enteez::EntityManager& em = ez.GetEntityManager();

	for (int i = 0; i < 10; i++)
	{
		enteez::Entity* entity = em.CreateEntity();
		if (i % 2 == 0)
			entity->AddComponent<Position>(0.0f, 0.0f, 0.0f);
	}

	int visited = 0;
	em.ForEach<Position>([&](enteez::Entity* entity, Position& position) { visited++; });
	TEST_ASSERT(visited == 5);

	// Prime the cache
	visited = 0;
	em.ForEach<Position>([&](enteez::Entity* entity, Position& position) { visited++; }, true);
	TEST_ASSERT(visited == 5);

	// New matching entity must be picked up by the existing cache
	enteez::Entity* extra = em.CreateEntity();
	extra->AddComponent<Position>(0.0f, 0.0f, 0.0f);
	visited = 0;
	em.ForEach<Position>([&](enteez::Entity* entity, Position& position) { visited++; }, true);
	TEST_ASSERT(visited == 6);

	// Removing the component must evict the entity from the cache
	extra->RemoveComponent<Position>();
	visited = 0;
	em.ForEach<Position>([&](enteez::Entity* entity, Position& position) { visited++; }, true);
	TEST_ASSERT(visited == 5);

	TEST_ASSERT(em.GetEntitysWith<Position>().size() == 5);

	em.Clear();
	TEST_ASSERT(em.GetEntitys().size() == 0);
}

// Destroying entities from inside a ForEach lambda must be safe (cached and uncached)
void TestDestroyDuringForEach()
{
	enteez::EnteeZ ez;
	enteez::EntityManager& em = ez.GetEntityManager();

	for (int i = 0; i < 5; i++)
		em.CreateEntity()->AddComponent<Position>(0.0f, 0.0f, 0.0f);

	em.ForEach<Position>([](enteez::Entity* entity, Position& position) { entity->Destroy(); });
	TEST_ASSERT(em.GetEntitys().size() == 0);

	for (int i = 0; i < 5; i++)
		em.CreateEntity()->AddComponent<Position>(0.0f, 0.0f, 0.0f);

	// Prime the cache, then destroy through the cached path
	em.ForEach<Position>([](enteez::Entity* entity, Position& position) {}, true);
	em.ForEach<Position>([](enteez::Entity* entity, Position& position) { entity->Destroy(); }, true);
	TEST_ASSERT(em.GetEntitys().size() == 0);
}

// RegisterBase, base-class iteration and BaseClassInstance
void TestBaseClassIteration()
{
	enteez::EnteeZ ez;
	enteez::EntityManager& em = ez.GetEntityManager();

	// Multiple bases registered in a single call
	ez.RegisterBase<Mover, Logic, Tag>();

	enteez::Entity* entity = em.CreateEntity();
	entity->AddComponent<Position>(0.0f, 0.0f, 0.0f);
	enteez::ComponentWrapper<Mover>* wrapper = entity->AddComponent<Mover>(entity);

	int visited = 0;
	entity->ForEach<Logic>([&](enteez::Entity* entity, Logic& logic) {
		visited++;
		logic.Update();
	});
	TEST_ASSERT(visited == 1);
	TEST_ASSERT(entity->GetComponent<Position>().z == 1.0f);

	visited = 0;
	entity->ForEach<Tag>([&](enteez::Entity* entity, Tag& tag) { visited++; });
	TEST_ASSERT(visited == 1);

	Logic* logic = nullptr;
	TEST_ASSERT(em.BaseClassInstance(*wrapper, logic));
	TEST_ASSERT(logic != nullptr);

	entity->Destroy();
}

// Regression: the query cache must keep working once component indices pass 64
// (the old ullong cache key overflowed there)
template<int N> struct Numbered { int value = N; };

template<std::size_t... Is>
void AddNumberedComponents(enteez::Entity* entity, std::index_sequence<Is...>)
{
	int expand[] = { (entity->AddComponent<Numbered<(int)Is>>(), 0)... };
	(void)expand;
}

void TestManyComponentTypes()
{
	enteez::EnteeZ ez;
	enteez::EntityManager& em = ez.GetEntityManager();

	enteez::Entity* entity = em.CreateEntity();
	// 70 distinct component types pushes indices well past 64
	AddNumberedComponents(entity, std::make_index_sequence<70>{});
	TEST_ASSERT(entity->GetComponentCount() == 70);
	TEST_ASSERT(entity->HasComponent<Numbered<69>>());

	int visited = 0;
	em.ForEach<Numbered<69>>([&](enteez::Entity* entity, Numbered<69>& n) {
		visited++;
		TEST_ASSERT(n.value == 69);
	}, true);
	TEST_ASSERT(visited == 1);

	entity->Destroy();
}

int main(int argc, char** argv)
{
	TestBasics();
	TestCallerOwnedComponent();
	TestDuplicateAddReplaces();
	TestForEachAndCache();
	TestDestroyDuringForEach();
	TestBaseClassIteration();
	TestManyComponentTypes();

	std::cout << (g_checks - g_failures) << "/" << g_checks << " checks passed" << std::endl;
	return g_failures == 0 ? 0 : 1;
}

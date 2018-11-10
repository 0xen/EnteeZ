#include <EnteeZ/EnteeZ.hpp>
#include <iostream>
#include <vector>


struct Position
{
	Position() {}
	Position(float x, float y) : x(x), y(y) {}
	float x;
	float y;
};


class Collidable
{
public:
	Collidable() {}
	Collidable(float rad) :rad(rad) {}
	float rad;
};

template<typename T>
class MsgRecive
{
public:
	virtual void ReciveMessage(enteez::Entity* sender, T message) = 0;
private:

};

class MsgSend
{
public:
	MsgSend() {}
	MsgSend(enteez::Entity* entity) : m_entity(entity) {}
	template<typename T>
	void Send(T data)
	{
		m_entity->ForEach<MsgRecive<T>>([data](enteez::Entity* entity, MsgRecive<T>& recive)
		{
			recive.ReciveMessage(entity, data);
		});
	}
private:
	enteez::Entity* m_entity;
};

class ComponentA : public MsgSend
{
public:
	ComponentA() {}
	ComponentA(enteez::Entity* entity) : MsgSend(entity){}
	void Update()
	{
		Send(123);
	}
private:

};

class ComponentB : public MsgRecive<int>
{
public:
	ComponentB() {}
	virtual void ReciveMessage(enteez::Entity* sender, int message)
	{
		std::cout << "B Wahoo " << message << std::endl;
	}
private:

};

struct A
{
	~A()
	{
		std::cout << "Called" << std::endl;
	}
};


int main(int argc, char **argv)
{

	enteez::EnteeZ ez;

	enteez::EntityManager& em = ez.GetEntityManager();


	enteez::Entity* entity1 = em.CreateEntity();
	entity1->AddComponent<A>();
	entity1->RemoveComponent<A>();



	ez.RegisterBase<ComponentA, MsgSend>();
	ez.RegisterBase<ComponentB, MsgRecive<int>>();


	Position* position = new Position(1.0f,2.0f);

	enteez::Entity* entity = em.CreateEntity();
	entity->AddComponent(position);
	std::cout << entity->HasComponent<Position>() << std::endl;



	em.ForEach<ComponentA>([](enteez::Entity* entity, ComponentA& caomponent_a)
	{

	}, true);


	for (unsigned int i = 0; i < 10; i++)
	{
		enteez::Entity* entity = em.CreateEntity();
		entity->AddComponent<Position>(1.2f, 1.3f);
		ComponentA* compa = entity->AddComponent<ComponentA>(entity);
		entity->AddComponent<ComponentB>();
		compa->Update();
		entity->RemoveComponent<ComponentA>();
	}

	em.ForEach<ComponentA>([](enteez::Entity* entity, ComponentA& caomponent_a)
	{
		std::cout << "a" << std::endl;
	}, true);

	for (auto& entity : em.GetEntitysWith<ComponentA>())
	{
		entity->Destroy();
	}

	em.ForEach<ComponentA>([](enteez::Entity* entity, ComponentA& caomponent_a)
	{
		std::cout << "b" << std::endl;
	}, true);

	


    return 0;
}

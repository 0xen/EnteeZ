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

class ComponentA : public MsgRecive<int>, public MsgSend
{
public:
	ComponentA() {}
	ComponentA(enteez::Entity* entity) : MsgSend(entity){}
	virtual void ReciveMessage(enteez::Entity* sender, int message)
	{
		std::cout << "A Wahoo " << message << std::endl;
	}
	void Update()
	{
		Send(123);
	}
private:

};

class ComponentB : public MsgRecive<int>, public MsgSend
{
public:
	ComponentB() {}
	ComponentB(enteez::Entity* entity) : MsgSend(entity) {}
	virtual void ReciveMessage(enteez::Entity* sender, int message)
	{
		std::cout << "B Wahoo " << message << std::endl;
	}
private:

};

int main(int argc, char **argv)
{

	enteez::EnteeZ ez;
	ez.RegisterBase<ComponentA, MsgSend, MsgRecive<int>>();
	ez.RegisterBase<ComponentB, MsgSend, MsgRecive<int>>();

	enteez::EntityManager& em = ez.GetEntityManager();


	for (unsigned int i = 0; i < 10; i++)
	{
		enteez::Entity* entity = em.CreateEntity();
		entity->AddComponent<Position>(1.2f, 1.3f);
		entity->AddComponent<ComponentA>(entity);
		entity->AddComponent<ComponentB>(entity);

	}
	em.ForEach<ComponentA>([](enteez::Entity* entity, ComponentA& caomponent_a)
	{
		caomponent_a.Update();
	}, true);

	


    return 0;
}

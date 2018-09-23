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


class MsgSend
{
public:
	MsgSend() {}
	void Send()
	{
		//std::cout << "Send " << a << std::endl;
	}

private:

};

template<typename T>
class MsgRecive
{
public:

	void ReciveMessage(T message)
	{
		std::cout << "ReciveMessage" << std::endl;
	}

private:

};

class ComponentA : public MsgSend, public MsgRecive<int>
{
public:
	ComponentA() {}

	char a;
private:

};

class ComponentB : public MsgRecive<int>, public MsgSend
{
public:
	ComponentB() {}
private:

};


int main(int argc, char **argv)
{
	ComponentA a;
	std::cout << sizeof(a) << std::endl;
	enteez::EnteeZ ez;
	ez.RegisterBase<ComponentA, MsgSend, MsgRecive<int>>();
	ez.RegisterBase<ComponentB, MsgSend, MsgRecive<int>>();

	enteez::EntityManager em = ez.GetEntityManager();

	enteez::Entity* entity = em.CreateEntity();
	entity->AddComponent<Position>();
	entity->AddComponent<ComponentA>();
	entity->AddComponent<ComponentB>();
	

	entity->ForEach<MsgSend>([](enteez::Entity* entity, MsgSend* send)
	{
		send->Send();
	});

	entity->ForEach<MsgRecive<int>>([](enteez::Entity* entity, MsgRecive<int>* recive)
	{
		int a = 1;
		recive->ReciveMessage(a);
	});
	

	em.ForEach<Position>([](enteez::Entity* entity, Position& pos)
	{
		//entity->AddComponent<Collidable>(1.0f);
	}, true);

	em.ForEach<Position, Collidable>([](enteez::Entity* entity, Position& pos, Collidable& coll)
	{
		std::cout << "Heya" << std::endl;
	}, true);
	


    return 0;
}

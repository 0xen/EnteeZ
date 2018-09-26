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
	MsgSend() { }
	void Send()
	{

	}
private:

};

template<typename T>
class MsgRecive
{
public:

	virtual void ReciveMessage(T message) = 0;

	int a = 0;
private:

};

class ComponentA : public MsgRecive<int>, public MsgSend
{
public:
	ComponentA() { a = 10; }
	virtual void ReciveMessage(int message)
	{
		std::cout << message << std::endl;
		std::cout << a << std::endl;
		a = message;
	}
private:

};

class ComponentB : public MsgRecive<int>, public MsgSend
{
public:
	ComponentB() { a = 30; }

	virtual void ReciveMessage(int message)
	{

	}
private:

};






int main(int argc, char **argv)
{
	/*
	// Creation
	enteez::TemplateBase* base = new enteez::TemplatePair<ComponentA, MsgRecive<int>>();


	// Base only
	//B<AA>* test = reinterpret_cast<B<AA>*>(base);
	enteez::TemplateStorage<MsgRecive<int>>& test = static_cast<enteez::TemplateStorage<MsgRecive<int>>&>(*base);
		

	void* examplePtr = new ComponentA;

	MsgRecive<int>& sender = test.Get(examplePtr);

	sender.ReciveMessage(69);

	std::cout << sender.a << std::endl;

	exit(0);*/




	enteez::EnteeZ ez;
	ez.RegisterBase<ComponentA, MsgSend, MsgRecive<int>>();
	ez.RegisterBase<ComponentB, MsgSend, MsgRecive<int>>();

	enteez::EntityManager em = ez.GetEntityManager();

	enteez::Entity* entity = em.CreateEntity();
	entity->AddComponent<Position>(1.2f,1.3f);
	entity->AddComponent<ComponentA>();
	entity->AddComponent<ComponentB>();


	entity->ForEach<MsgRecive<int>>([](enteez::Entity* entity, MsgRecive<int>& send)
	{
		send.ReciveMessage(2);
		std::cout << send.a << std::endl;
	});

	entity->ForEach<MsgSend>([](enteez::Entity* entity, MsgSend& send)
	{
		send.Send();
	});
	

	em.ForEach<Position>([](enteez::Entity* entity, Position& pos)
	{
		entity->AddComponent<Collidable>(1.0f);
	}, true);

	em.ForEach<Position, Collidable>([](enteez::Entity* entity, Position& pos, Collidable& coll)
	{
		std::cout << "Heya" << std::endl;
	}, true);
	


    return 0;
}

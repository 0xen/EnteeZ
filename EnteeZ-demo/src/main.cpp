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


int main(int argc, char **argv)
{

	enteez::EnteeZ ez;

	enteez::EntityManager em = ez.GetEntityManager();

	for (int i = 0; i < 10000; i++)
	{
		enteez::Entity* entity = em.CreateEntity();
		entity->AddComponent<Position>();
		entity->AddComponent<Collidable>(1.0f);
	}
	for (int i = 0; i < 10000; i++)
	{
		enteez::Entity* entity = em.CreateEntity();
		entity->AddComponent<Position>();
	}

	em.ForEach<Position, Collidable>([](enteez::Entity* entity, Position& pos, Collidable& coll)
	{
		//std::cout << "Heya" << std::endl;
	}, true);

	em.ForEach<Position, Collidable>([](enteez::Entity* entity, Position& pos, Collidable& coll)
	{
		//std::cout << "Heya" << std::endl;
	}, true);



    return 0;
}

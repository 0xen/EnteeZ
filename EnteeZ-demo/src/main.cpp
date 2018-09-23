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

	enteez::BitHandler bh1;




	enteez::EnteeZ ez;

	enteez::EntityManager em = ez.GetEntityManager();


	enteez::Entity* entity = em.CreateEntity();
	entity->AddComponent<Position>();
	entity->AddComponent<Collidable>(1.0f);



	em.ForEach<Position>([](enteez::Entity* entity, Position& pos)
	{

	});



    return 0;
}

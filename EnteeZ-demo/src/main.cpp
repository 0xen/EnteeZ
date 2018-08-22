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


int main(int argc, char **argv)
{

	enteez::EnteeZ ez;

	std::vector<enteez::Entity*> entities;

	for (int i = 0; i < 10; i++)
	{
		enteez::Entity* entity = ez.CreateEntity();

		entity->AddComponent<Position>(12.0f * i, 22.0f * i);

		entities.push_back(entity);
	}

	ez.ForEach<Position>([](enteez::Entity* entity, Position& a)
	{
		std::cout << a.x << " " << a.y << std::endl;
	});

    return 0;
}

#include <EnteeZ/EnteeZ.hpp>
#include "TestComponent.hpp"
#include <iostream>
#include <bitset>


int main(int argc, char **argv)
{

	enteez::EnteeZ ez;

	enteez::Entity* entity = ez.CreateEntity();

	entity->AddComponent<TestComponent>(1, 2);

	entity->RemoveComponent<TestComponent>();

	ez.ForEach<TestComponent>([](enteez::Entity* entity, TestComponent& a)
	{
		a.a = 10;
		a.b = 20;
	});

	ez.ForEach<TestComponent>([](enteez::Entity* entity, TestComponent& a)
	{
		std::cout << a.a << " " << a.b << std::endl;
	});


	delete entity;

    return 0;
}

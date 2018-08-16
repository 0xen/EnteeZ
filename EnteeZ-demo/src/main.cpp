#include <EnteeZ/EnteeZ.hpp>
#include "TestComponent.hpp"
#include <iostream>
#include <bitset>

template<typename ...components>
void ForEach(std::function<void(enteez::Entity, components ...)> f);


template<typename ...components>
void ForEach(std::function<void(enteez::Entity, components...)> f)
{

}

template<typename T>
void Test()
{

}

struct Pos
{
	Pos() {}
	Pos(float x, float y, float z) :x(x), y(y), z(z) {}
	float x;
	float y;
	float z;
};

int main(int argc, char **argv)
{

	enteez::EnteeZ ez;

	enteez::Entity* entity = ez.CreateEntity();

	entity->AddComponent<TestComponent>(1, 2);

	entity->Destroy();

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

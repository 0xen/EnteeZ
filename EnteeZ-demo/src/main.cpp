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

};


class A
{

};

template <typename T>
class B : public A
{

};

A a;

int main(int argc, char **argv)
{
	B<int> b;

	a = b;

	//B<int> c = reinterpret_cast<B<int>>(a);


	enteez::EnteeZ ez;

	enteez::Entity* entity = ez.CreateEntity();

	entity->AddComponent<TestComponent>(1, 2);

	std::cout << entity->HasComponent<TestComponent>() << std::endl;

	ez.ForEach<TestComponent>([](TestComponent& a)
	{
		a.a = 10;
		a.b = 20;
	});

	ez.ForEach<TestComponent>([](TestComponent& a)
	{
		std::cout << a.a << " " << a.b << std::endl;
	});


	delete entity;

    return 0;
}

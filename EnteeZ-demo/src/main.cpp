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

//typename std::remove_const<C>::type

int main(int argc, char **argv)
{
	enteez::EnteeZ ez;

	enteez::Entity entity = ez.CreateEntity();

	entity.AddComponent<TestComponent>(1, 2);

	std::cout << entity.HasComponent<TestComponent>() << std::endl;;

	ez.ForEach<TestComponent>([](enteez::Entity entity, TestComponent a)
	{
		std::cout << "a" << std::endl;
	});


    return 0;
}

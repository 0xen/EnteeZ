#include <EnteeZ/EnteeZ.hpp>
#include "TestComponent.hpp"
#include <iostream>

template<typename ...components>
void ForEach(std::function<void(enteez::Entity, components ...)> f);


template<typename ...components>
void ForEach(std::function<void(enteez::Entity, components...)> f)
{

}

int main(int argc, char **argv)
{
	enteez::EnteeZ ez;

	enteez::Entity entity = ez.CreateEntity();

	entity.AddComponent<TestComponent>(1,2);

	ez.ForEach<TestComponent>([](enteez::Entity entity, TestComponent a)
	{
		std::cout << "a" << std::endl;
	});


    return 0;
}

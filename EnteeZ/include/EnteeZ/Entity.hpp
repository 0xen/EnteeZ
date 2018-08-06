#pragma once
#include <iostream>
#include <type_traits>
#include <bitset>

namespace enteez
{
	class Entity
	{
	public:
		Entity();


		template <typename T, typename ... Args>
		T AddComponent(Args&& ... args);

	private:
		std::bitset<40> components;
	};
	template<typename T, typename ...Args>
	inline T Entity::AddComponent(Args && ...args)
	{
		T t(std::forward<Args>(args) ...);

		return t;
	}
}
#pragma once
#include <EnteeZ/Entity.hpp>
#include <functional>

namespace enteez
{
	template <typename T> struct lambda_function { typedef T definition; };
	class EnteeZ
	{
	public:
		EnteeZ();

		EnteeZ::Entity CreateEntity();

		template<typename ...components>
		void ForEach(typename lambda_function<std::function<void(Entity, components ...)>>::definition f);

	};


	template<typename ...components>
	inline void EnteeZ::ForEach(typename lambda_function<std::function<void(Entity, components ...)>>::definition f)
	{
		//f(CreateEntity(),1);
	}
}
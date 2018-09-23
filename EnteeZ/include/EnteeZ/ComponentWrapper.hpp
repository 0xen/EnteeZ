#pragma once

#include <EnteeZ\BaseComponentWrapper.hpp>

#include <iostream>
#include <type_traits>
#include <bitset>

namespace enteez
{
	template <typename T>
	class ComponentWrapper : public BaseComponentWrapper
	{
	public:
		ComponentWrapper() {}
		ComponentWrapper(T* component);

		T& Get();

	private:
	};
	template<typename T>
	inline ComponentWrapper<T>::ComponentWrapper(T* component)
	{
		m_component = component;
	}
	template<typename T>
	inline T& ComponentWrapper<T>::Get()
	{
		return *static_cast<T*>(m_component);
	}
}
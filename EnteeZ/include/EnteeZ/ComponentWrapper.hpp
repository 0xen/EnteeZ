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
		ComponentWrapper() : m_origional(true){}
		ComponentWrapper(T* component, bool origional = true);
		~ComponentWrapper();
		T& Get();

	private:
		bool m_origional;
	};

	template<typename T>
	inline ComponentWrapper<T>::ComponentWrapper(T* component, bool origional) : m_origional(origional)
	{
		m_component = component;
	}
	template<typename T>
	inline ComponentWrapper<T>::~ComponentWrapper()
	{
		if (m_origional) delete m_component;
	}
	template<typename T>
	inline T& ComponentWrapper<T>::Get()
	{
		return *static_cast<T*>(m_component);
	}
}
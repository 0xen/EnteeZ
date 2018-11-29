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
		ComponentWrapper(T* component,unsigned int size, unsigned int id, bool origional = true);
		virtual ~ComponentWrapper();
		T& Get();

	private:
		bool m_origional;
	};

	template<typename T>
	inline ComponentWrapper<T>::ComponentWrapper(T * component, unsigned int size, unsigned int id, bool origional) : BaseComponentWrapper(id), m_origional(origional)
	{
		m_size = size;
		m_component = component;
	}
	template<typename T>
	inline ComponentWrapper<T>::~ComponentWrapper()
	{
		if (m_origional)
			delete reinterpret_cast<T*>(m_component);
	}
	template<typename T>
	inline T& ComponentWrapper<T>::Get()
	{
		return *static_cast<T*>(m_component);
	}
}
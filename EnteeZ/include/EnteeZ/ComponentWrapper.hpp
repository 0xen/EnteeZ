#pragma once

#include <EnteeZ\BaseComponentWrapper.hpp>

#include <iostream>
#include <type_traits>
#include <bitset>

namespace enteez
{
	template <typename T>
	// Stores a component instance.
	// This holds the templated type and origional pointer back to the object so it can be refrenced
	class ComponentWrapper : public BaseComponentWrapper
	{
	public:
		// Create a default instance of the component
		ComponentWrapper() : m_origional(true)
		{
			m_component = nullptr;
		}
		// Create a instance of the componet wrapper passing all required knowlage about the component
		ComponentWrapper(T* component,unsigned int size, unsigned int id, bool origional = true);
		// Override the base deconstructor
		virtual ~ComponentWrapper();
		// Returns a pointer to the component - Override the component pointer
		virtual void* GetComponentPtr();
		// Get a instance of the component in its origional format
		T& Get();
	private:
		// Local instance of the component
		T * m_component;
		// Is this instance of the component the origional?
		bool m_origional;
	};

	template<typename T>
	// Create a instance of the componet wrapper passing all required knowlage about the component
	inline ComponentWrapper<T>::ComponentWrapper(T * component, unsigned int size, unsigned int id, bool origional) : BaseComponentWrapper(id), m_origional(origional)
	{
		m_size = size;
		m_component = component;
	}
	template<typename T>
	// Override the base deconstructor
	inline ComponentWrapper<T>::~ComponentWrapper()
	{
		// If we are not responcable for the memory, dont delete it
		if (m_origional)
			delete reinterpret_cast<T*>(m_component);
	}
	template<typename T>
	// Returns a pointer to the component - Override the component pointer
	inline void * ComponentWrapper<T>::GetComponentPtr()
	{
		return m_component;
	}
	template<typename T>
	// Get a instance of the component in its origional format
	inline T& ComponentWrapper<T>::Get()
	{
		return *m_component;
	}
}
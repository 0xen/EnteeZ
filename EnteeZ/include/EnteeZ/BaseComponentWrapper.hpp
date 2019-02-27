#pragma once

#include <string>

namespace enteez
{
	// Holds basic info that  is required to be known for all components
	class BaseComponentWrapper
	{
	public:
		// Initilize the class and pass what the components unique ID is, this is represents its class type
		BaseComponentWrapper(unsigned int id) : m_name("Component"), m_id(id){}
		virtual ~BaseComponentWrapper() {}
		// Returns a pointer to the component but with no type
		virtual void* GetComponentPtr() = 0;
		// Get the component size
		unsigned int GetSize();
		// Get the components name
		std::string GetName();
		// Set the components name
		void SetName(std::string name);
		// Get the components object type ID
		unsigned int GetID();
	protected:
		unsigned int m_id;   // Object type id
		unsigned int m_size; // Size of the component object
		std::string m_name;  // Components name
	};
}
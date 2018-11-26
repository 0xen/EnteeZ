#pragma once

#include <string>

namespace enteez
{
	class BaseComponentWrapper
	{
	public:
		BaseComponentWrapper() : m_name("Component"){}
		void* GetComponentPtr();
		unsigned int GetSize();
		std::string GetName();
		void SetName(std::string name);
	protected:
		void* m_component;
		unsigned int m_size;
		std::string m_name;
	};
}
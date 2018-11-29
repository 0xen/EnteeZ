#pragma once

#include <string>

namespace enteez
{
	class BaseComponentWrapper
	{
	public:
		BaseComponentWrapper(unsigned int id) : m_name("Component"), m_id(id){}
		virtual ~BaseComponentWrapper() {}
		void* GetComponentPtr();
		unsigned int GetSize();
		std::string GetName();
		void SetName(std::string name);
		unsigned int GetID();
	protected:
		unsigned int m_id;
		void* m_component;
		unsigned int m_size;
		std::string m_name;
	};
}
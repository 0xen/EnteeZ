#pragma once

#include <string>

namespace enteez
{
	class BaseComponentWrapper
	{
	public:
		BaseComponentWrapper(unsigned int id) : m_name("Component"), m_id(id){}
		virtual ~BaseComponentWrapper() {}
		virtual void* GetComponentPtr() = 0;
		unsigned int GetSize();
		std::string GetName();
		void SetName(std::string name);
		unsigned int GetID();
	protected:
		unsigned int m_id;
		unsigned int m_size;
		std::string m_name;
	};
}
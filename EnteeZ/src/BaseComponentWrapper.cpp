#include <EnteeZ/BaseComponentWrapper.hpp>

using namespace enteez;


unsigned int enteez::BaseComponentWrapper::GetSize()
{
	return m_size;
}

std::string enteez::BaseComponentWrapper::GetName()
{
	return m_name;
}

void enteez::BaseComponentWrapper::SetName(std::string name)
{
	m_name = name;
}

unsigned int enteez::BaseComponentWrapper::GetID()
{
	return m_id;
}

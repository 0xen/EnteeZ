#include <EnteeZ/BaseComponentWrapper.hpp>

using namespace enteez;

void * enteez::BaseComponentWrapper::GetComponentPtr()
{
	return m_component;
}

#pragma once

namespace enteez
{
	class BaseComponentWrapper
	{
	public:
		BaseComponentWrapper() {}
		void* GetComponentPtr();
	protected:
		void* m_component;
	};
}
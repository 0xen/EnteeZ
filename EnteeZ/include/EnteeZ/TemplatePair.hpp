#pragma once

#include <EnteeZ\TemplateStorage.hpp>

namespace enteez
{
	template <typename V, typename T>
	class TemplatePair : public TemplateStorage<T>
	{
	public:
		virtual T* Get(void* data)
		{
			return dynamic_cast<T*>(static_cast<V*>(data));
		}
	};
}
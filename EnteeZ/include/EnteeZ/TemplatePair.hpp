#pragma once

#include <EnteeZ\TemplateStorage.hpp>

namespace enteez
{
	template <typename V, typename T>
	class TemplatePair : public TemplateStorage<T>
	{
	public:
		virtual T& Get(void* data)
		{
			return *static_cast<V*>(data);
		}
	};
}
#pragma once

#include <EnteeZ\TemplateBase.hpp>

namespace enteez
{
	template <typename T>
	class TemplateStorage : public TemplateBase
	{
	public:
		virtual T* Get(void* data) = 0;
	};
}
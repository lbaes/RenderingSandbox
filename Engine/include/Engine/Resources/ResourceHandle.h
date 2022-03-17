#pragma once
#include <memory>

namespace Eng {
	template <class T>
	struct ResourceHandle
	{
		std::shared_ptr<T> resource = nullptr;
	};
}
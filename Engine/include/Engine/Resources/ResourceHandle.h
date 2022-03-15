#pragma once
#include <memory>

template <class T>
struct ResourceHandle
{
	std::shared_ptr<T> resource = nullptr;
};
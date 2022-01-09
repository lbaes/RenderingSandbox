#pragma once

#include "ICallback.hpp"
#include <stdexcept>
namespace Eng
{
	template<typename T, class DerivedEvent>
	class EventCallback : public ICallback
	{
	public:
		typedef void (T::* CallbackT)(const DerivedEvent&); // Pointer to any member function of T
		EventCallback(T* instance, EventCallback::CallbackT callback)
				: callback(callback)
		{
			if (instance == nullptr)
			{
				throw std::runtime_error("tried to instantiate Callback with a null instance");
			}
			this->instance = instance;
		}

		void run_aux(EventBase& eventBase) const override
		{
			if (instance == nullptr)
				throw std::runtime_error("cannot execute callback with a null instance");
			(instance->*callback)(static_cast<const DerivedEvent&>(eventBase));
		}

		bool equals(const EventCallback<T, DerivedEvent>& b) const
		{
			return instance == b.instance && callback == b.callback;
		}

	private:
		CallbackT callback;
		T* instance;
	};
}
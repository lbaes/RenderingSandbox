#pragma once

#include "ICallback.hpp"
#include <stdexcept>

namespace Eng
{
	template<typename DerivedEvent>
	class FreeFunctionCallback : public ICallback
	{
	public:
		typedef void (* FreeCallbackT)(const DerivedEvent&);

		explicit FreeFunctionCallback(FreeCallbackT callback)
		{
			if (callback == nullptr)
				throw std::runtime_error("Tried to instantiate callback with a null instance");
			this->callback = callback;
		}

	private:
		void run_aux(const EventBase& event) const override
		{
			if (callback == nullptr)
				throw std::runtime_error("cannot execute callback with a null instance");
			callback(static_cast<const DerivedEvent&>(event));
		}

		// Non owning free function pointer
		FreeCallbackT callback;
	};
}
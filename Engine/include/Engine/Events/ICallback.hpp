#pragma once

#include "EventBase.h"

namespace Eng
{
	class ICallback
	{
	public:
		virtual ~ICallback() = default;

		void run(const EventBase& event) const
		{
			run_aux(event);
		}

	private:
		virtual void run_aux(const EventBase& event) const = 0;
	};
}
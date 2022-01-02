#pragma once

namespace Eng
{
	class EventBase
	{
	public:
		virtual ~EventBase() = default;
		bool isHandled = false;
	};
}
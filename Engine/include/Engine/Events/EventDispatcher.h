#pragma once

#include <unordered_map>
#include <vector>
#include <memory>
#include <typeindex>
#include <spdlog/spdlog.h>
#include "EventCallback.h"

namespace Eng
{

	class EventDispatcher
	{
	public:
		template<typename T, typename DerivedEvent>
		void add_listener(T* instance, void(T::* callbackT)(const DerivedEvent&))
		{
			auto cb = new EventCallback(instance, callbackT);
			callback_list[typeid(DerivedEvent)].push_back(cb);
		}

		template<typename T, typename DerivedEvent>
		void remove_listener(T* instance, void(T::* callbackT)(const DerivedEvent&))
		{
			const EventCallback<T, DerivedEvent> tmp(instance, callbackT);
			auto& callbacks = callback_list[typeid(DerivedEvent)];
			auto it = std::find_if(callbacks.begin(), callbacks.end(), [tmp](const ICallback* c)
				{
					return tmp.equals(*static_cast<const EventCallback<T, DerivedEvent>*>(c));
				});
			if (it != callbacks.end())
			{
				delete* it;
				callbacks.erase(it);
			}
		}

		void dispatch(const EventBase& event) const
		{
			const auto& callbacks = callback_list.at(typeid(event));
			for (auto const cb : callbacks)
			{
				cb->run(event);
			}
		}

	private:
		std::unordered_map<std::type_index, std::vector<ICallback*>> callback_list{};
	};

}
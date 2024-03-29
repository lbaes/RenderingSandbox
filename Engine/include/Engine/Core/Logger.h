#pragma once

#include <string>
#include <memory>
#include <iostream>
#include "spdlog/spdlog.h"
#include "spdlog/async.h"
#include "spdlog/sinks/basic_file_sink.h"
#include "spdlog/sinks/stdout_color_sinks.h"
#include "Engine/Utility/Singleton.h"

namespace Eng
{
	class Logger : Singleton<Logger>
	{
	private:
		friend class Singleton<Logger>;
		Logger()
		{
			try
			{
				auto console_sink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
				console_sink->set_level(spdlog::level::info);
				auto file_sink = std::make_shared<spdlog::sinks::basic_file_sink_mt>("logs/engine_log.txt", true);
				file_sink->set_level(spdlog::level::trace);

				spdlog::sinks_init_list sink_list = { file_sink, console_sink };
				const spdlog::logger logger{ "ENGINE", sink_list.begin(), sink_list.end() };
				engine_log = std::make_unique<spdlog::logger>(logger);
			}
			catch (const spdlog::spdlog_ex& ex)
			{
				std::cout << "Log initialization failed: " << ex.what() << std::endl;
			}
		}

		~Logger()
		{
			// Under VisualStudio, this must be called before main finishes to workaround a known VS issue
			spdlog::drop_all();
		}

	public:

		static Logger* GetLogger() {
			return Singleton<Logger>::Create();
		}

		template<typename... Args>
		void LogTrace(fmt::format_string<Args...> message, Args&& ...args) const
		{
			engine_log->info(message, std::forward<Args>(args)...);
		}

		template<typename... Args>
		void LogInfo(fmt::format_string<Args...> message, Args&& ...args) const
		{
			engine_log->info(message, std::forward<Args>(args)...);
		}

		template<typename... Args>
		void LogWarning(fmt::format_string<Args...> message, Args&& ...args) const
		{
			engine_log->warn(message, std::forward<Args>(args)...);
		}

		template<typename... Args>
		void LogError(fmt::format_string<Args...> message, Args&& ...args) const
		{
			engine_log->error(message, std::forward<Args>(args)...);
		}

		template<typename... Args>
		void LogFatal(fmt::format_string<Args...> message, Args&& ...args) const
		{
			engine_log->critical(message, std::forward<Args>(args)...);
		}

	private:
		std::unique_ptr<spdlog::logger> engine_log = nullptr;
	};
}



#pragma once

#include "Engine/Core/Application.h"
#include "Engine/Utility/Singleton.h"
#include "Engine/Core/Logger.h"
namespace Eng {
	class Startup : Singleton<Startup>{
	public :
		Startup(){}
		~Startup(){}
	private:
		// Singletons
		Application* Application = nullptr;
		Logger* EngineLogger = nullptr;

		
	};
}

#ifdef ENG_WINDOWS

int main()
{
	int status = 0;
	Eng::Logger* engine_logger = Eng::Logger::GetLogger();
	Eng::Application* app = Eng::GetApplication();
	if (app)
	{
		engine_logger->LogInfo("Starting Application");
		try {
			return app->Start();
		}
		catch (const std::exception& ex) {
			engine_logger->LogError("unhandled exception:\n{}", ex.what());
			status = -1;
		}
	}
	engine_logger->LogError("Could not start application");
	return -1;
}

#endif // ENG_WINDOWS
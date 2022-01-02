#pragma once

#include "Engine/Core/Application.h"
#include "Engine/Core/Logger.h"

#ifdef ENG_WINDOWS

int main()
{
	Eng::Logger* engine_logger = Eng::Logger::GetLogger();
	Eng::Application* app = Eng::GetApplication();
	if (app)
	{
		engine_logger->LogInfo("Starting Application");
		return app->Start();
	}
	engine_logger->LogError("Could not start application");
	return -1;
}

#endif // ENG_WINDOWS
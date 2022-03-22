#include "Demo.h"


int main()
{
	Demo demo;
	Eng::Logger* engine_logger = Eng::Logger::GetLogger();
	Eng::Application* app = &demo;
	if (app)
	{
		engine_logger->LogInfo("Starting Application");
		try
		{
			return app->Start();
		}
		catch (const std::exception& ex)
		{
			engine_logger->LogError("unhandled exception:\n{}", ex.what());
			return -1;
		}
	}
	engine_logger->LogError("Could not start application");
	return -1;
}
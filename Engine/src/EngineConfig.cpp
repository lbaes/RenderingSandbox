#include "Engine/Core/EngineConfig.h"
#include <iostream>
#include <filesystem>

namespace Eng
{
	void EngineConfig::ReadFromFile(const std::string& file_name)
	{
		// TODO Read from file
	}

	const std::string& EngineConfig::ApplicationName() const
	{
		return application_name;
	}

	int EngineConfig::WindowWidth() const
	{
		return window_width;
	}

	int EngineConfig::WindowHeight() const
	{
		return window_height;
	}

	bool EngineConfig::VSyncEnabled() const
	{
		return vsync;
	}
}
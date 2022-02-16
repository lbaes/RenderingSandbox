#pragma once
#include "Engine/Utility/Singleton.h"
#include <string>

namespace Eng {
	class EngineConfig : Singleton<EngineConfig> {
	public:
		friend class Singleton<EngineConfig>;
		friend class EngineConfigParser;
		void ReadFromFile(const std::string& file_name);
		const std::string& ApplicationName() const;
		int WindowWidth() const;
		int WindowHeight() const;
		bool VSyncEnabled() const;
		// More...
	private:
		std::string application_name;
		int window_width;
		int window_height;
		bool vsync;
	};

}
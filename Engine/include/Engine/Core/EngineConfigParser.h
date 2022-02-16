#pragma once
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include "Engine/Core/Logger.h"
namespace Eng
{
	struct Token
	{
		std::string name;
		std::string data;
	};

	class EngineConfigParser
	{
	public:
		explicit EngineConfigParser(const std::string& filename) {};

	private:
		// terminal symbols
		const std::string letters = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";
		const std::string digits = "0123456789";

		std::vector<Token> tokens;
		Token current_token;
		std::string file;
		Logger* logger;

	};

}
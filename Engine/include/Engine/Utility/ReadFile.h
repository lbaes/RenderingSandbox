#pragma once
#include <string_view>

namespace Util {
	auto read_file(std::string_view path) -> std::string; // https://gist.github.com/klmr/849cbb0c6e872dff0fdcc54787a66103
}// namespace Util
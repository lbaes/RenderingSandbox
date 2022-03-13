#include "ReadFile.h"
#include <fstream>

auto Util::read_file(std::string_view path) -> std::string {
	constexpr auto readSize = size_t{4096};
	auto stream = std::ifstream{path.data()};
	stream.exceptions(std::ios_base::badbit);

	auto out = std::string{};
	auto buf = std::string(readSize, '\0');
	while (stream.read(&buf[0], readSize)) {
		out.append(buf, 0, stream.gcount());
	}
	out.append(buf, 0, stream.gcount());
	return out;
}

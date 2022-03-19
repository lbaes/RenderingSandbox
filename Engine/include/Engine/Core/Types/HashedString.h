#pragma once
#include <MurmurHash3.h>
#include <cstdint>
#include <string>

namespace Eng {
	inline uint32_t hash_string(const std::string& key) {
		static uint32_t HASH_SEED = 1;
		uint32_t output; // 32 bit output;
		MurmurHash3_x86_32(key.c_str(), key.size(), HASH_SEED, &output);
		return output;
	}
	namespace literal {
		inline uint32_t operator "" _hash(const char* key, size_t s) {
			static uint32_t HASH_SEED = 1;
			uint32_t output; // 32 bit output;
			MurmurHash3_x86_32(key, s, HASH_SEED, &output);
			return output;
		}
	}

}
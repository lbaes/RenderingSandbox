#pragma once
#include <MurmurHash3.h>
#include <cstdint>

namespace Eng {
	typedef uint32_t HashedString;
	inline namespace literal {
		HashedString operator "" _hash(const char* key, size_t s) {
			static uint32_t HASH_SEED = 1;
			uint32_t output; // 32 bit output;
			MurmurHash3_x86_32(key, s, HASH_SEED, &output);
			return output;
		}
	}
}
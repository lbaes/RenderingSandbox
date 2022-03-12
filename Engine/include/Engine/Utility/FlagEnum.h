#pragma once

template <typename T> operator| (T a, T b) {
	return static_cast<T>(static_cast<int>(a), static_cast<int>(b));
}
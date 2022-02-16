#pragma once
#include <utility>

namespace Eng {
	template <class T>
	class Singleton {
	public:
		// Delete copy constructors
		Singleton(const Singleton&) = delete;
		Singleton& operator=(Singleton const&) = delete;
	protected:
		// Return the singleton instance of T
		template <typename ...Args>
		static T* Create(Args && ...args) {
			if (instance == nullptr)
				instance = new T(std::forward(args)...);
			return instance;
		}

		// Destroys the singleton instance of T
		static void Destroy() {
			delete instance;
		}
		Singleton() = default;
		~Singleton() = default;
	private:
		static inline T* instance = nullptr;

	};
}
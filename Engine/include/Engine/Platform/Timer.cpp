#include <Engine/Platform/Timer.h>

#ifdef ENG_WINDOWS
#include <GLFW/glfw3.h>
#endif

namespace Eng {

#ifdef ENG_WINDOWS
	double TimeMillis() {
		return glfwGetTime() * 1000.0;
	}
#endif

	Ticks Timer::GetTime() const
	{
		if (started && !paused) {
			return TimeMillis() - start_time;
		}

		if (started && paused) {
			return paused_time;
		}

		return 0;
	}

	void Timer::Start()
	{
		start_time = TimeMillis();
		paused_time = 0;
		started = true;
		paused = false;
	}

	void Timer::Stop()
	{
		start_time = 0;
		paused_time = 0;
		paused = false;
		started = false;
	}

	void Timer::Pause()
	{
		if (started && !paused) {
			paused = true;
			paused_time = TimeMillis() - start_time;
			start_time = 0;
		}
	}

	void Timer::Resume()
	{
		if (started && paused) {
			paused = false;
			start_time = TimeMillis() - paused_time;
			paused_time = 0;
		}
	}

	bool Timer::IsPaused() const
	{
		return paused && started;
	}

	bool Timer::IsStarted() const
	{
		return started;
	}

}
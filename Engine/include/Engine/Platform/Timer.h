#pragma once

namespace Eng {

	#if ENG_WINDOWS
		typedef double Ticks;
	#endif // ENG_WINDOWS

	class Timer {
	public:
		void Start();
		void Stop();
		void Pause();
		virtual void Resume();
		virtual bool IsPaused() const;
		virtual bool IsStarted() const;
		virtual Ticks GetTime() const;
		bool IsStopped() {
			return !IsStarted();
		}
		bool IsRunning() {
			return !IsPaused();
		}
	private:
		Ticks start_time = 0;
		Ticks paused_time = 0;
		bool started = false;
		bool paused = false;
	};
}
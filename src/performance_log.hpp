#ifndef PERFORMANCE_LOG_HPP
#define PERFORMANCE_LOG_HPP

#include <string>
#include <chrono>

class PerformanceLog {
	public:
		typedef std::chrono::time_point<std::chrono::steady_clock> time_point;

		PerformanceLog()
		: _lastTime(std::chrono::steady_clock::now())
		{ }

		void log(std::string msg) {
			time_point now = std::chrono::steady_clock::now();

			auto diff = std::chrono::duration_cast<std::chrono::microseconds>(now - _lastTime).count();

			std::cout << msg << " (" << diff << " µs)" << std::endl;

			_lastTime = now;
		}

	private:
		time_point _lastTime;

};

#endif

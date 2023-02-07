#pragma once

#include <chrono>


namespace ig
{

	template< class Clock >
	class timer
	{
		typename Clock::time_point start;

	public:

		timer() : start( Clock::now() ) 
		{
		}

		double elapsed() const
		{
			return (Clock::now() - start).count() * ((double)Clock::period::num/Clock::period::den);
		}

		void restart() { start = Clock::now(); }
	};

	typedef timer<std::chrono::system_clock> system_timer;
	typedef timer<std::chrono::steady_clock> steady_timer;
	typedef timer<std::chrono::high_resolution_clock> high_resolution_timer;
}



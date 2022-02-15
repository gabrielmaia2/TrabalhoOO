#include "time2.h"

using namespace std::chrono;

//									Time

	Time::Time()
			: _prev(high_resolution_clock::now()),
			  _delta(0.0)
	{
		update();
	}
	Time::~Time()
	{
	
	}

	void Time::update()
	{
		_delta = static_cast<duration<double>>(high_resolution_clock ::now() - _prev).count();
		_prev = high_resolution_clock::now();
	}

	// delta time
	const double &Time::deltaTime() const
	{
		return _delta;
	}
	
	// elapsed time
	double Time::elapTime() const
	{
		return static_cast<duration<double>>(high_resolution_clock ::now() - _prev).count();
	}
	
	// fps
	const int &Time::fps() const
	{
		static int fps;
		fps = static_cast<int>(1.0/_delta + 0.5);
		return fps;
	}

//										Timer

	Timer::Timer(double i)
			: _interval(i)
	{
	
	}
	Timer::~Timer()
	{
	
	}

	Timer::operator double()
	{
		return _interval;
	}

	void Timer::operator=(double i)
	{
		_interval = i;
	}
	const bool &Timer::operator()()
	{
		static bool b;
		if((_interval > 0)&&(elapTime() > _interval))
		{
			update();
			b = true;
		}
		else
			b = false;
		return b;
	}
	const bool &Timer::updated()
	{
		static bool b;
		b = (_interval > 0) && (elapTime() > _interval);
		return b;
	}

	const double &Timer::interval()
	{
		return _interval;
	}

//								GameTimer

	GameTimer::GameTimer(double s, double i)
			: Timer(i),
			  _start(s)
	{
	
	}
	GameTimer::~GameTimer()
	{
	
	}

	const double &GameTimer::timeSinceStart() const
	{
		static double d;
		d = static_cast<duration<double>>(high_resolution_clock::now().time_since_epoch()).count() - _start;
		return d;
	}

	const bool &GameTimer::fpsLimiter() const
	{
		static bool b;
		b = (_fps_limiter.elapTime() > 1.0 / 60.0);
		return b;
	}
	void GameTimer::updateFpsLimiter()
	{
		_fps_limiter.update();
	}

	GameTimer &GameTimer::instance()
	{
		static GameTimer inst(0.0, 1.0 / allg_frame_limiter);
		return inst;
	}

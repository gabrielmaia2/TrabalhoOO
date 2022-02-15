#ifndef TIME_H
#define TIME_H

#include <chrono>

using namespace std::chrono;

class Time
{
protected:
	time_point<high_resolution_clock> _prev;
	double _delta;
public:
	Time();
	virtual ~Time();
	
	void update(); // updates time
	
	// delta time
	const double &deltaTime() const; // returns the time variation in the last update
	
	// elapsed time
	double elapTime() const; // returns the time elapsed since last update
	
	// fps
	const int &fps() const; // returns fps
};

class Timer : public Time
{
private:
	double _interval;
	
	using Time::update;
public:
	Timer(double i = 0.0);
	virtual ~Timer();
	
	operator double();
	
	void operator=(double i);
	const bool &operator()();
	const bool &updated();
	
	const double &interval();
};

class GameTimer final : public Timer
{
private:
	double _start;
	
	Time _fps_limiter;
	
	GameTimer(double s, double i);
	
	GameTimer(const GameTimer &) = delete;
	GameTimer(GameTimer &&) = delete;
	
	void operator=(const GameTimer &) = delete;
	void operator=(GameTimer &&) = delete;
	
	using Time::operator=;
public:
	~GameTimer();
	
	const double &timeSinceStart() const;
	
	const bool &fpsLimiter() const;
	void updateFpsLimiter();
	
	static GameTimer &instance();
};

#endif

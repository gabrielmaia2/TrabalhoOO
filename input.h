#ifndef INPUT_H
#define INPUT_H

#include "graphics.h"
#include "container.h"

class Keyboard final
{
private:
	ALLEGRO_EVENT_QUEUE *_events;
	Container<int> _keys;
	
	Keyboard();
	
	Keyboard(const Keyboard &) = delete;
	Keyboard(Keyboard &&) = delete;
	
	void operator=(const Keyboard &) = delete;
	void operator=(Keyboard &&) = delete;
public:
	~Keyboard();
	
	void update();
	
	bool find(int key) const;
	
	Container<int> keys();
	
	/// Gets singleton instance
	static Keyboard &instance();
};

class Mouse final
{
private:
	ALLEGRO_EVENT_QUEUE *_events;
	
	int _x;
	int _y;
	bool _left;
	bool _right;
	
	Image _mouse;
	
	Mouse();
	
	Mouse(const Mouse &) = delete;
	Mouse(Mouse &&) = delete;
	
	void operator=(const Mouse &) = delete;
	void operator=(Mouse &&) = delete;
public:
	~Mouse();
	
	const int &x() const;
	const int &y() const;
	const bool &left() const;
	const bool &right() const;
	
	Point size();
	
	void update();
	
	/// Gets singleton instance
	static Mouse &instance();
};

#endif

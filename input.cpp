#include "input.h"

using namespace std;

//									Keyboard

	Keyboard::Keyboard()
			: _events(al_create_event_queue())
	{
		al_register_event_source(_events,al_get_keyboard_event_source());
	}
	Keyboard::~Keyboard()
	{
		al_destroy_event_queue(_events);
	}

	void Keyboard::update()
	{
		ALLEGRO_EVENT event;
		while(al_get_next_event(_events, &event))
		{
			switch (event.type)
			{
				case ALLEGRO_EVENT_KEY_DOWN:
					if (find(event.keyboard.keycode))
						_keys.erase(event.keyboard.keycode);
					_keys.push_back(event.keyboard.keycode);
					break;
				case ALLEGRO_EVENT_KEY_UP:
					if (find(event.keyboard.keycode))
						_keys.erase(event.keyboard.keycode);
					break;
				default:break;
			}
		}
	}

	bool Keyboard::find(int key) const
	{
		return _keys.find(key) != err_not_found;
	}

	Container<int> Keyboard::keys()
	{
		return _keys;
	}

	Keyboard &Keyboard::instance()
	{
		static Keyboard inst;
		return inst;
	}

//									Mouse

	Mouse::Mouse()
			: _events(al_create_event_queue()),
			  _x(0),
			  _y(0),
			  _left(false),
			  _right(false)
	{
		al_register_event_source(_events,al_get_mouse_event_source());
		_mouse.load("../files/textures/sprites/mouse.png");
	}
	Mouse::~Mouse()
	{
		al_destroy_event_queue(_events);
	}


	const int &Mouse::x() const
	{
		return _x;
	}
	const int &Mouse::y() const
	{
		return _y;
	}
	const bool &Mouse::left() const
	{
		return _left;
	}
	const bool &Mouse::right() const
	{
		return _right;
	}

	Point Mouse::size()
	{
		return _mouse.size();
	}

	void Mouse::update()
	{
		ALLEGRO_EVENT event;
		while(al_get_next_event(_events, &event))
		{
			switch (event.type)
			{
				case ALLEGRO_EVENT_MOUSE_AXES:
					_x = event.mouse.x;
					_y = event.mouse.y;
					break;
				case ALLEGRO_EVENT_MOUSE_BUTTON_DOWN:
					if (event.mouse.button == 1)
						_left = true;
					else
						_right = true;
					break;
				case ALLEGRO_EVENT_MOUSE_BUTTON_UP:
					if (event.mouse.button == 1)
						_left = false;
					else
						_right = false;
					break;
				default:
					break;
			}
		}
		_mouse.draw(static_cast<int>(_x + _mouse.size().x() / 2), static_cast<int>(_y + _mouse.size().y() / 2));
	}

	Mouse &Mouse::instance()
	{
		static Mouse inst;
		return inst;
	}

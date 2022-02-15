#include "display.h"
#include "input.h"
#include "map.h"

using namespace std;

//							Display

	Display::Display()
			: _show_info(false)
	{
		if(!al_init()) Error::instance().error() = Error::err::allg_init_libr; // initialize allegro graphics
		if(!(_display = al_create_display(allg_width,allg_height))) Error::instance().error() = Error::err::allg_init_disp; // initializes display
		
		// initializing
		if(!al_install_keyboard()) Error::instance().error() = Error::err::allg_install_keyboard;
		if(!al_install_mouse()) Error::instance().error() = Error::err::allg_install_mouse;
		al_init_image_addon();
		al_init_font_addon();
		al_init_ttf_addon();
		al_init_primitives_addon();
		
		// registering display events
		_events = al_create_event_queue();
		al_register_event_source(_events,al_get_display_event_source(_display));
		
		// hiding mouse cursor
		al_hide_mouse_cursor(_display);
		_scene = new MainMenu();
	}
	Display::~Display()
	{
		al_destroy_display(_display);
		al_destroy_event_queue(_events);
	}

//

	void Display::showsInfo()
	{
		static TextBox info;
		static TextBox keys;
		static string str;
		
		str = "game fps : ";
		str += to_string(GameTimer::instance().fps());
		
		str += "\ndelta time : ";
		str += to_string(GameTimer::instance().deltaTime());
		
		str += "\nresolution : ";
		str += to_string(allg_width);
		str += "x";
		str += to_string(allg_height);
		
		str += "\nmouse pos : ";
		str += to_string(Mouse::instance().x());
		str += "x";
		str += to_string(Mouse::instance().y());
		
		str += "\nmouse left : ";
		str += (Mouse::instance().left())?"true":"false";
		
		str += "\nmouse right : ";
		str += (Mouse::instance().right())?"true":"false";
		
		str += "\ncamera pos : ";
		str += "(";
		str += to_string(Player::camPos().x());
		str += ", ";
		str += to_string(Player::camPos().y());
		str += ")";
		
		if(&Player::player1())
		{
			str += "\nplayer 1 pos : ";
			str += "(";
			str += to_string(Player::player1().collision().x());
			str += ", ";
			str += to_string(Player::player1().collision().y());
			str += ")";
			if(&Player::player2())
			{
				str += "\nplayer 2 pos : ";
				str += "(";
				str += to_string(Player::player2().collision().x());
				str += ", ";
				str += to_string(Player::player2().collision().y());
				str += ")";
			}
		}
		if(&Boss::instance())
		{
			str += "\nboss pos : ";
			str += "(";
			str += to_string(Boss::instance().collision().x());
			str += ", ";
			str += to_string(Boss::instance().collision().y());
			str += ")";
		}
		
		if(Map::instance().enemies().size())
		{
			str += "\nEnemies : ";
			str += to_string(Map::instance().enemies().size());
		}
		
		info.text(str);
		info.draw(TextBox::Flag::up_left);
		
		str = "Keys :\n";
		for(auto &i : Keyboard::instance().keys())
		{
			str += to_string(i);
			str += "\n";
		}
		
		keys.text(str);
		keys.draw(TextBox::Flag::up_right);
	}

// updates the display buffer

	void Display::update()
	{
		if(GameTimer::instance()())
		{
			static bool limiter;
			limiter = GameTimer::instance().fpsLimiter(); // updates display
			_scene.update();
			// closes the display
			ALLEGRO_EVENT event;
			while(al_get_next_event(_events, &event))
				if(event.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
					Error::instance().error() = Error::err::allg_close;
			
			// limits display update fps to 60
			if(limiter)
			{
				static bool up = true;
				
				// shows game info
				if(Keyboard::instance().find(ALLEGRO_KEY_F3)&&Keyboard::instance().find(ALLEGRO_KEY_LCTRL)&&up)
				{
					_show_info = !_show_info;
					up = false;
				}
				else if(!(Keyboard::instance().find(ALLEGRO_KEY_F3)&&Keyboard::instance().find(ALLEGRO_KEY_LCTRL)))
					up = true;
				if(_show_info) showsInfo();
				
				Keyboard::instance().update();
				Mouse::instance().update();
				
				al_flip_display();
				al_clear_to_color(al_map_rgb(0,0,0)); // clear buffer
				GameTimer::instance().updateFpsLimiter();
			}
		}
	}

	const bool &Display::showInfo()
	{
		return _show_info;
	}

// singleton instance

	Display &Display::instance()
	{
		static Display inst;
		return inst;
	}

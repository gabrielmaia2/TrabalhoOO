#ifndef VARS_H
#define VARS_H

/*
 * Game project for completion of object-oriented c++ course
 * This project was built using c++17
 *
 * The singleton pattern was used with static memory allocation
 *
 * Keywords:
 *
 * nullptr							(c++11)	http://en.cppreference.com/w/cpp/language/nullptr
 * for(type var:list) {...}			(c++11)	http://en.cppreference.com/w/cpp/language/range-for
 * Class(Class &&var)				(c++11)	http://en.cppreference.com/w/cpp/language/move_constructor
 * Class &operator=(Class &&var)	(c++11)	http://en.cppreference.com/w/cpp/language/move_assignment
 * auto var							(c++11)	http://en.cppreference.com/w/cpp/language/auto
 * using name = type;				(c++11)	http://en.cppreference.com/w/cpp/language/type_alias
 * type function() override {...}	(c++11)	http://en.cppreference.com/w/cpp/language/override
 * class Class final;				(c++11)	http://en.cppreference.com/w/cpp/language/final
 * inline type function() {...}		(c++17)	http://en.cppreference.com/w/cpp/language/inline
 */

#include <iostream>

#define allg_width				1280
#define allg_height				720

#define default_font			"../files/fonts/DejaVuSansMono.ttf"
#define default_font_size		20
#define maximum_font_size		50

#define allg_frame_limiter		200.0f

#define gravity					0.06
#define max_fall_speed			12
#define collision_border		0.1
#define PI						3.141592

#define block_size				32
#define chunk_scale				1.0
#define platform_limits_x		40000

#define platform_random_height	0.12
#define platform_random_holes	0.05
#define platform_random_towers	0.03

#define walk_accel				0.2
#define stop_accel				0.14

#define enemy_look_range		600

#define blood_range				200
#define blood_speed				3
#define blood_number			12

#define kunai_speed				7

#define default_spawn_timer		0.2
#define default_spawn_chance	0.2
#define max_enemies				25
#define max_projectiles			8

#define collider_color			al_map_rgba(50, 10, 20, 20)

#define sky_background			"../files/textures/background/sky0.png"
#define game_background			"../files/textures/background/game.png"

#define zombie_random_value		0.5
#define spitter_random_value	0.5
#define slime_random_value		0.5

#define boss_speed				8

// this class stores error and the display is closed when this error is not equal to err::allg_success

using namespace std;

class Error
{
public:
	enum class err
	{
		allg_success			= 1,
		
		allg_close				= 0,
		
		allg_init_libr			= -1,
		allg_init_disp			= -2,
		allg_load_font			= -3,
		allg_load_img			= -4,
		
		allg_install_keyboard	= -5,
		allg_install_mouse		= -6,
		
		zero_div				= -7,
	};
private:
	
	err _error;
	
	inline Error(): _error(err::allg_success) { }
	
	Error(const Error &) = delete;
	Error(Error &&) = delete;
	
	void operator=(const Error &) = delete;
	void operator=(Error &&) = delete;
public:
	inline ~Error() { if(_error != err::allg_close) cout << "Game closed with error : " << static_cast<int>(_error) << endl; }
	
	inline err &error() { return _error; }
	inline const err &error() const { return _error; }
	
	inline static Error &instance() { static Error inst; return inst; }
};

#endif
#include "sound.h"
/*
//									Sound

	Sound::Sound()
			: _sample(nullptr),
			  _pan(0),
			  _volume(1),
			  _speed(1)
	{
	
	}
	Sound::~Sound()
	{
		al_destroy_sample(_sample);
	}

	double &Sound::pan()
	{
		return _pan;
	}
	const double &Sound::pan() const
	{
		return _pan;
	}
	double &Sound::volume()
	{
		return _volume;
	}
	const double &Sound::volume() const
	{
		return _volume;
	}
	double &Sound::speed()
	{
		return _speed;
	}
	const double &Sound::speed() const
	{
		return _speed;
	}

	void Sound::play()
	{
		al_play_sample(_sample, _volume, _pan, _speed, ALLEGRO_PLAYMODE_ONCE, nullptr);
	}

	void Sound::load(string path)
	{
		al_destroy_sample(_sample);
		_sample = al_load_sample(path.c_str());
	}

//									Song

	Song::Song()
			: _sound(nullptr),
			  _pan(0),
			  _volume(1),
			  _speed(1)
	{
	
	}
	Song::~Song()
	{
		al_destroy_sample_instance(_sound);
	}

	double &Song::pan()
	{
		return _pan;
	}
	const double &Song::pan() const
	{
		return _pan;
	}
	double &Song::volume()
	{
		return _volume;
	}
	const double &Song::volume() const
	{
		return _volume;
	}
	double &Song::speed()
	{
		return _speed;
	}
	const double &Song::speed() const
	{
		return _speed;
	}

	void Song::play()
	{
		al_play_sample_instance(_sound);
		al_get_sample_instance_playing(_sound);
		al_get_sample_instance_length(_sound);
		al_get_sample_instance_position(_sound);
		al_stop_sample();
	}

	void Song::load(string path)
	{
		al_destroy_sample_instance(_sound);
		_sound = al_create_sample_instance(al_load_sample(path.c_str()));
	}
 
 
		//if(!al_install_audio()) Error::instance().error() = Error::err::allg_install_audio;
		//al_init_acodec_addon();
		
		// TODO apt install libogg-dev libvorbis-dev
		
		//_mixer = al_create_mixer(44100, ALLEGRO_AUDIO_DEPTH_FLOAT32, ALLEGRO_CHANNEL_CONF_1);
		//al_set_default_mixer(_mixer);
		
		//al_reserve_samples(100);
		
		//_music.load("../files/sounds/music/boss1.m4a");
		
		
		
		allg_install_audio		= -8,
		
		
		
		_frog1.load("../files/textures/sprites/frog.png", 0.01, 9, 1);
		_frog2.load("../files/textures/sprites/frog.png", 0.01, 9, 1);
		_frog1.scale()(0.16, 0.16);
		_frog2.scale()(0.16, 0.16);
		_frog1.scale().x() *= -1; // TODO
		
		
	
	//Sound _music;
	//_music.play(); TODO
	
	Animation _frog1;
	Animation _frog2; // TODO
 
 
		
		_frog1.draw(1 * allg_width / 4, 37 * allg_height / 40); // TODO
		_frog2.draw(3 * allg_width / 4, 37 * allg_height / 40);
*/
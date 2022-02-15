#ifndef SCENE_H
#define SCENE_H

#include "entity.h"

class Scene;

class SceneBase
{
	private: Scene *_scene;
	
	protected: Scene &scene() { return *_scene; }
	
	public: SceneBase() : _scene(nullptr) { }
	public: virtual ~SceneBase() { }
	
	public: void operator=(Scene *s) { _scene = s; }
	
	public: virtual void update() = 0;
};

class Scene final
{
	private: SceneBase *_scene_base;
	private: SceneBase *_deleted;
	
	protected: SceneBase &scene() { return *_scene_base; }
	
	public: Scene(SceneBase *s = nullptr) : _scene_base(s), _deleted(nullptr) { if(_scene_base) *_scene_base = this; }
	public: ~Scene()
	{
		if(_deleted)
			delete _deleted;
		if(_scene_base&&(_scene_base != _deleted))
			delete _scene_base;
	}
	
	public: void operator=(SceneBase *sc)
	{
		if(!_deleted)
		{
			_deleted = _scene_base;
			_scene_base = sc;
			*_scene_base = this;
		}
	}
	
	public: void update()
	{
		if(_deleted)
		{
			delete _deleted;
			_deleted = nullptr;
		}
		if(_scene_base)
			_scene_base->update();
	}
};

class MainMenu final : public SceneBase
{
private:
	Button _play;
	Button _quit;
	Background _background;
	TextBox _title;
public:
	MainMenu();
	~MainMenu();
	
	void update();
};

class SelectPlayers final : public SceneBase
{
private:
	Button _1player;
	Button _2player;
	Button _back;
	Background _background;
	TextBox _title;
public:
	SelectPlayers();
	~SelectPlayers();
	
	void update();
};

class LoadMap final : public SceneBase
{
private:
	Button _new;
	Button _back;
	Background _background;
	TextBox _title;
	int _players;
public:
	LoadMap(int players);
	~LoadMap();
	
	void update();
};

class SelectPhase final : public SceneBase
{
private:
	Button _phase1;
	Button _phase2;
	Button _phase3;
	Button _back;
	Background _background;
	TextBox _title;
	int _players;
public:
	SelectPhase(int players);
	~SelectPhase();
	
	void update();
};

class BuildingMap final : public SceneBase
{
private:
	bool _player1;
	bool _player2;
	TextBox _building;
	Background _background;
	Timer _timer;
	int _points;
	int _phase;
	int _score1;
	int _score2;
public:
	BuildingMap(bool p1, bool p2, int phase, int sc1, int sc2);
	~BuildingMap();
	
	void update();
};

class Phase final : public SceneBase
{
private:
	Background _back;
	Background _trees;
	Button _menu;
	Button _quit;
	bool _show;
	bool _flag;
	BoxImg _fade;
	int _phase;
	Timer _timer;
	Portal _portal;
	bool _active_portal;
public:
	Phase(bool p1, bool p2, int phase, int sc1, int sc2);
	~Phase();
	
	void update();
};

class YouWin final : public SceneBase
{
private:
	Button _menu;
	TextBox _win;
	Background _background;
public:
	YouWin(int p1score, int p2score);
	~YouWin();
	
	void update();
};

class GameOver final : public SceneBase
{
private:
	Button _retry;
	Button _quit;
	Background _background;
public:
	GameOver();
	~GameOver();
	
	void update();
};

#endif

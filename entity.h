#ifndef ENTITY_H
#define ENTITY_H

#include <string>

#include "container.h"
#include "graphics.h"
#include "sound.h"


using namespace std;

class Entity
{
protected:
	class State;
	
	class StateBase
	{
	private:
		Entity *_entity;
		State *_state;
	protected:
		inline State &state() { return *_state; }
		inline Entity &entity() { return *_entity; }
	public:
		inline StateBase() : _entity(nullptr), _state(nullptr) { }
		inline virtual ~StateBase() { }
		
		inline void operator=(State *s) { _state = s; }
		inline void operator=(Entity *e) { _entity = e; }
		
		virtual void update() = 0;
	};
	
	class State final
	{
	private:
		Entity *_entity;
		StateBase *_state_base;
	protected:
		inline StateBase &state() { return *_state_base; }
		inline Entity &entity() { return *_entity; }
	public:
		inline State(Entity *e, StateBase *s = nullptr) : _entity(e), _state_base(s)
		{
			if(_state_base)
			{
				*_state_base = this;
				*_state_base = _entity;
			}
		}
		inline ~State()  { if(_state_base) delete _state_base; }
		
		inline void operator=(StateBase *sb)
		{
			static StateBase *s;
			s = _state_base;
			_state_base = sb;
			*_state_base = this;
			*_state_base = _entity;
			if(s) delete s;
		}
		
		inline void update() { if(_state_base&&_entity) _state_base->update(); }
	};
	
	//
	
	BoxCol _pos;
	Point _speed;
	Point _accel;
	
	Animation _anim;
	
	ValueBar _life;
	double _damage;
	
	bool _use_phys;
	bool _faded;
	bool _jumping;
	bool _hole;
	bool _block;
	bool _use_ai;
	bool _drawnable;
	bool _on_col;
	bool _use_grav;
	bool _use_col;
	
	int _idle_anim;
	int _walk_anim;
	
	double _walk_speed;
	double _default_interval;
	
	virtual void updAI() = 0;
	virtual void walkAI(double speed);
	
	const BoxCol &checkBlock();
	const BoxCol &checkHole();
	
	void updCol();
	virtual void draw();
	
	Entity();
	
	Entity(const Entity &) = delete;
	Entity(Entity &&) = delete;
	
	void operator=(const Entity &) = delete;
	void operator=(Entity &&) = delete;
public:
	virtual ~Entity();
	
	virtual void walk(double speed);
	
	virtual void fade();
	
	virtual void jump(double speed);
	
	ValueBar & life();
	const ValueBar & life() const;
	double &damage();
	const double &damage() const;
	bool &usePhys();
	const bool &usePhys() const;
	
	bool &drawnable();
	const bool &drawnable() const;
	const double &walkSpeed() const;
	
	bool &faded();
	const bool &faded() const;
	
	const BoxCol &collision() const;
	void pos(double x, double y);
	
	Animation &anim();
	const Animation &anim() const;
	
	void update();
	
	friend class StateBase;
};

class Tower : public Entity
{
private:
	bool _mounted1;
	bool _mounted2;
	
	void updAI();
public:
	Tower();
	Tower(double posx, double posy);
	~Tower();
};

class RedTower final : public Tower
{
public:
	RedTower();
	RedTower(double posx, double posy);
	~RedTower();
};

class Kunai;

class Player final : public Entity
{
private:
	static Point _cam_pos;
	
	static Player *_player1;
	static Player *_player2;
	
	Container<Kunai*> _projectiles;
	
	int _up;
	int _left;
	int _right;
	int _run;
	int _attack;
	int _mount;
	int _flash;
	
	double _multiplier;
	
	int _draw_light;
	
	Tower *_tower;
	LinearLight _light;
	
	Timer _attack_timer;
	
	int _score;
	
	void draw();
	
	void drawLight();
	
	void updAI();
	
	void updProj();
	
	Player(string path);
	
	using Entity::update;
public:
	~Player();
	
	const int &mount() const;
	const int &attack() const;
	Tower *&tower();
	const Tower *tower() const;
	
	void fade();
	
	const int &score() const;
	int &score();
	
	static void update();
	
	static void updateMounted();
	
	static void newPlayer1();
	static void newPlayer2();
	
	static Player &player1();
	static Player &player2();
	
	static void remPlayer();
	
	static const Point &camPos();
};

class FleshBall;

class Enemy : public Entity
{
protected:
	class WalkAround final : public StateBase
	{
	private:
		double _direction;
		Timer _walk_timer;
	public:
		WalkAround();
		~WalkAround();
		
		void update();
	};
	
	class FollowPlayer final : public StateBase
	{
	private:
		Player *_player;
		double _multiplier;
	public:
		FollowPlayer(Player *p);
		~FollowPlayer();
		
		void update();
	};
	
	class Explode final : public StateBase
	{
	private:
		Container<FleshBall *> _blood;
		bool _destroyed;
	public:
		Explode();
		~Explode();
		
		void update();
	};
	
	State _current_state;
	int _jump_case;
	bool _follow;
	Timer _hit;
	Timer _htimer;
	
	virtual void updAI();
public:
	Enemy(double x, double y);
	virtual ~Enemy();
	
	virtual void hit(Player *p);
	
	virtual void walk(double speed);
	
	static Enemy *get(double x, double y);
};

class FleshBall final : public Enemy
{
private:
	void updAI();
public:
	FleshBall(double x, double y, double sx, double sy, double d);
	~FleshBall();
	
	void hit(Player *p);
};

class Zombie : public Enemy
{
public:
	Zombie(double x, double y);
	virtual ~Zombie();
};

class SpitterZombie final : public Zombie
{
private:
	Container<FleshBall*> _spit;
	Timer _spit_timer;
	
	void updAI();
public:
	SpitterZombie(double x, double y);
	~SpitterZombie();
	
	void hit(Player *p);
};

class Slime final : public Enemy
{
private:
	Timer _jump_timer;
public:
	Slime(double x, double y);
	~Slime();
	
	void walkAI(double speed);
	void jump(double speed);
	
	void fade();
};

class Kunai final : public Entity
{
private:
	Timer _fade_timer;
	Player *_player;
	
	void updAI();
public:
	Kunai(double x, double y, double sx, double sy, double d, Player *p);
	virtual ~Kunai();
};

class Boss final : public Enemy
{
private:
	class Wait final : public StateBase
	{
	private:
		Timer _timer;
	public:
		Wait();
		~Wait();
		
		void update();
	};
	
	class Attack final : public StateBase
	{
	private:
		Player *_player;
		Timer _timer;
		Timer _tp_timer;
		double _speed;
		int _mult;
	public:
		Attack(Player *p);
		~Attack();
		
		void update();
	};
	
	class SpawnEnemies final : public StateBase
	{
	private:
		Timer _timer;
		Timer _sp_timer;
	public:
		SpawnEnemies();
		~SpawnEnemies();
		
		void update();
	};
	
	static Boss *_instance;
	
	RoundLight _light;
	
	void updAI();
	
	Boss(double x, double y);
public:
	~Boss();
	
	void fade();
	
	static Boss &instance(double x = 0.0, double y = 0.0);
	static void destroy();
};

class Portal final : public Entity
{
private:
	void updAI();
	
	bool _activated;
public:
	Portal();
	~Portal();
	
	operator bool();
};

#endif

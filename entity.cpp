#include "entity.h"
#include "map.h"
#include "input.h"
#include "display.h"
#include <cmath>

//											Entity

	void Entity::walkAI(double speed)
	{
		if(speed == 0.0)
		{
			if(abs(_accel.x()) < stop_accel)
			{
				_speed.x() = 0;
				_accel.x() = 0;
				_anim.anim(static_cast<unsigned int>(_idle_anim));
			}
			if(abs(_speed.x()) > stop_accel)
				_accel.x() -= stop_accel * _speed.x() / abs(_speed.x());
		}
		else
		{
			if(abs(_accel.x()) < abs(speed))
				_accel.x() += walk_accel * speed / abs(speed);
			else if(_speed.x() * speed < 0)
				_accel.x() += stop_accel * speed / abs(speed);
			else
				_accel.x() -= stop_accel * speed / abs(speed);
			
			if(_anim.scale().x() * speed < 0) _anim.scale().x() *= -1;
			
			_anim.timer() = _walk_speed * _default_interval / abs(speed);
			
			_anim.anim(static_cast<unsigned int>(_walk_anim));
		}
	}

	const BoxCol &Entity::checkBlock()
	{
		static BoxCol box;
		box(_pos.x() + _anim.scale().x() * (_pos.bounds().x() + block_size / 2),
			_pos.y() + _pos.bounds().y() - block_size / 2);
		box.bounds()(block_size / 16, block_size / 4);
		return box;
	}
	const BoxCol &Entity::checkHole()
	{
		static BoxCol box;
		box(_pos.x() + _anim.scale().x() * (_pos.bounds().x() + block_size / 2),
			_pos.y() + _pos.bounds().y() + 4 * block_size);
		box.bounds()(block_size / 16, 2.5 * block_size);
		return box;
	}

	void Entity::updCol()
	{
		static bool on_ground;
		on_ground = false;
		_hole = true;
		_block = false;
		_on_col = false;
		for(auto &c : Map::instance().chunks())
		{
			for(auto &i : c->blocks())
			{
				static BoxCol::ColType ctype;
				ctype = _pos.onCol(i->colBox());
				if(ctype != BoxCol::ColType::none)
				{
					_on_col = true;
					static Point d;
					d = (_pos - i->colBox()) / (_pos.bounds() + i->colBox().bounds());
					if(d.x() < 0) d.x() = -d.x();
					if(d.y() < 0) d.y() = -d.y();
					switch (ctype)
					{
						case BoxCol::ColType::left :
							_pos.x() -= _pos.bounds().x() + i->colBox().bounds().x() - (abs(_pos.x() - i->colBox().x())) - collision_border * 0.5;
						case BoxCol::ColType::border_left :
							if(_accel.x() > 0) _accel.x() = 0;
							break;
						case BoxCol::ColType::right :
							_pos.x() += _pos.bounds().x() + i->colBox().bounds().x() - (abs(_pos.x() - i->colBox().x())) - collision_border * 0.5;
						case BoxCol::ColType::border_right :
							if(_accel.x() < 0) _accel.x() = 0;
							break;
						case BoxCol::ColType::up :
							_pos.y() -= _pos.bounds().y() + i->colBox().bounds().y() - (abs(_pos.y() - i->colBox().y())) - collision_border * 0.5;
						case BoxCol::ColType::border_up :
							if(_accel.y() > 0) _accel.y() = 0;
							on_ground = true;
							break;
						case BoxCol::ColType::down :
							_pos.y() += _pos.bounds().y() + i->colBox().bounds().y() - (abs(_pos.y() - i->colBox().y())) - collision_border * 0.5;
						case BoxCol::ColType::border_down :
							if(_accel.y() < 0) _accel.y() = 0;
							break;
						default :
							break;
					}
				}
				if(_use_ai)
				{
					if(checkHole().onCol(i->colBox()) != BoxCol::ColType::none)
						_hole = false;
					else if(checkBlock().onCol(i->colBox()) != BoxCol::ColType::none)
						_block = true;
				}
			}
		}
		if(!on_ground) _jumping = true;
		else if(_speed.y() >= 0.0) _jumping = false;
	}
	void Entity::draw()
	{
		_anim.draw(static_cast<int>(_pos.x() + allg_width / 2 - Player::camPos().x()),
				   static_cast<int>(_pos.y() + allg_height / 2 - Player::camPos().y()));
		_life.draw(static_cast<int>(_pos.x() + allg_width / 2 - Player::camPos().x()),
				   static_cast<int>(_pos.y() + allg_height / 2 - Player::camPos().y() - (_anim.size().y() * _anim.scale().y() + _life.limits().y()) / 2 - 5));
		if(Display::instance().showInfo()&&GameTimer::instance().fpsLimiter())
		{ // TODO
			al_draw_filled_rectangle(static_cast<float>(_pos.x() + allg_width / 2 - Player::camPos().x() - _pos.bounds().x()),
									 static_cast<float>(_pos.y() + allg_height / 2 - Player::camPos().y() - _pos.bounds().y()),
									 static_cast<float>(_pos.x() + allg_width / 2 - Player::camPos().x() + _pos.bounds().x()),
									 static_cast<float>(_pos.y() + allg_height / 2 - Player::camPos().y() + _pos.bounds().y()),
									 collider_color);
			al_draw_filled_rectangle(static_cast<float>(checkHole().x() + allg_width / 2 - Player::camPos().x() - checkHole().bounds().x()),
									 static_cast<float>(checkHole().y() + allg_height / 2 - Player::camPos().y() - checkHole().bounds().y()),
									 static_cast<float>(checkHole().x() + allg_width / 2 - Player::camPos().x() + checkHole().bounds().x()),
									 static_cast<float>(checkHole().y() + allg_height / 2 - Player::camPos().y() + checkHole().bounds().y()),
									 collider_color);
			al_draw_filled_rectangle(static_cast<float>(checkBlock().x() + allg_width / 2 - Player::camPos().x() - checkBlock().bounds().x()),
									 static_cast<float>(checkBlock().y() + allg_height / 2 - Player::camPos().y() - checkBlock().bounds().y()),
									 static_cast<float>(checkBlock().x() + allg_width / 2 - Player::camPos().x() + checkBlock().bounds().x()),
									 static_cast<float>(checkBlock().y() + allg_height / 2 - Player::camPos().y() + checkBlock().bounds().y()),
									 collider_color);
		}
	}

	Entity::Entity()
		: _life(1),
		  _damage(0),
		  _use_phys(true),
		  _faded(false),
		  _jumping(false),
		  _hole(false),
		  _block(false),
		  _use_ai(true),
		  _drawnable(true),
		  _on_col(false),
		  _use_grav(true),
		  _use_col(true),
		  _idle_anim(0),
		  _walk_anim(1),
		  _walk_speed(1)
	{
	
	}
	Entity::~Entity()
	{
	
	}

	void Entity::walk(double speed)
	{
		walkAI(speed);
	}

	void Entity::fade()
	{
		_faded = true;
	}

	void Entity::jump(double speed)
	{
		if(!_jumping)
			_accel.y() -= speed;
	}

	ValueBar &Entity::life()
	{
		return _life;
	}
	const ValueBar &Entity::life() const
	{
		return _life;
	}
	double &Entity::damage()
	{
		return _damage;
	}
	const double &Entity::damage() const
	{
		return _damage;
	}
	bool &Entity::usePhys()
	{
		return _use_phys;
	}
	const bool &Entity::usePhys() const
	{
		return _use_phys;
	}
	bool &Entity::drawnable()
	{
		return _drawnable;
	}
	const bool &Entity::drawnable() const
	{
		return _drawnable;
	}
	const double &Entity::walkSpeed() const
	{
		return _walk_speed;
	}


	bool &Entity::faded()
	{
		return _faded;
	}
	const bool &Entity::faded() const
	{
		return _faded;
	}
	const BoxCol &Entity::collision() const
	{
		return _pos;
	}
	void Entity::pos(double x, double y)
	{
		_pos(x, y);
	}

	Animation &Entity::anim()
	{
		return _anim;
	}
	const Animation &Entity::anim() const
	{
		return _anim;
	}

	void Entity::update()
	{
		updAI();
		if(_use_phys)
		{
			if(_use_grav&&(_speed.y() < max_fall_speed))
				_accel.y() += gravity;
			_speed(0,0);
			_speed += _accel;
			_pos += _speed;
			if(_use_col)
				updCol();
		}
		else
		{
			_speed(0,0);
			_accel(0,0);
		}
		if((_pos.y() > Map::instance().maxPosition())||(_life.value() <= 0))
			fade();
		if(_drawnable)
			draw();
	}

//											Tower

	void Tower::updAI()
	{
		static bool down1 = false;
		static bool down2 = false;
		
		if(&Player::player1())
		{
			if(Keyboard::instance().find(Player::player1().mount())&&!down1)
			{
				if((Player::player1().collision().onCol(_pos) != BoxCol::ColType::none)&&!_mounted1)
				{
					Player::player1().tower() = this;
					_mounted1 = true;
					down1 = true;
				}
				else if(_mounted1)
				{
					Player::player1().tower() = nullptr;
					_mounted1 = false;
					down1 = true;
				}
			}
			else if(!Keyboard::instance().find(Player::player1().mount()))
			{
				down1 = false;
			}
		}
		if(&Player::player2())
		{
			if(Keyboard::instance().find(Player::player2().mount())&&!down2)
			{
				if((Player::player2().collision().onCol(_pos) != BoxCol::ColType::none)&&!_mounted2)
				{
					Player::player2().tower() = this;
					_mounted2 = true;
					down2 = true;
				}
				else if(_mounted2)
				{
					Player::player2().tower() = nullptr;
					_mounted2 = false;
					down2 = true;
				}
			}
			else if(!Keyboard::instance().find(Player::player2().mount()))
			{
				down2 = false;
			}
		}
	}

	Tower::Tower()
			: _mounted1(false),
			  _mounted2(false)
	{
		_use_ai = false;
		_drawnable = false;
		_use_phys = false;
	}

	Tower::Tower(double posx, double posy)
			: _mounted1(false),
			  _mounted2(false)
	{
		_use_ai = false;
		_life = 2000;
		_damage = 0;
		_anim.load("../files/textures/sprites/tower.png", 0.0, 1, 1);
		_pos.bounds() = _anim.size().prod(_anim.scale()) / 2;
		_pos(posx, posy - _pos.bounds().y());
		_use_phys = false;
	}
	Tower::~Tower()
	{
		if(&Player::player1())
		{
			if(Player::player1().tower() == this)
				Player::player1().tower() = nullptr;
		}
		if(&Player::player2())
		{
			if(Player::player2().tower() == this)
				Player::player2().tower() = nullptr;
		}
	}

//											RedTower


	RedTower::RedTower()
	{
	
	}
	RedTower::RedTower(double posx, double posy)
			: Tower(posx, posy)
	{
		_life.value() = 3000;
		_anim.load("../files/textures/sprites/redtower.png", 0.0, 1, 1);
		_pos.bounds() = _anim.size().prod(_anim.scale()) / 2;
		_pos(posx, posy - _pos.bounds().y());
	}
	RedTower::~RedTower()
	{
	
	}

//											Player

	Point Player::_cam_pos;

	Player *Player::_player1 = nullptr;
	Player *Player::_player2 = nullptr;

	void Player::draw()
	{
		Entity::draw();
		if(_score > 0)
		{
			Font::instance().writeText(12,
									   static_cast<int>(_pos.x() + allg_width / 2 - _cam_pos.x()),
									   static_cast<int>(_pos.y() + allg_height / 2 - _cam_pos.y() - _pos.bounds().y() - 30),
									   to_string(_score));
		}
	}

	void Player::drawLight()
	{
		if(Keyboard::instance().find(_flash))
		{
			if(_draw_light == 1) _draw_light = 2;
			else if(_draw_light == 3) _draw_light = 0;
		}
		else
		{
			if(_draw_light == 0) _draw_light = 1;
			else if(_draw_light == 2) _draw_light = 3;
		}
		if(_draw_light >= 2)
			_light.draw(static_cast<int>(_pos.x() + allg_width / 2 - _cam_pos.x() + 10 * _anim.scale().x()),
						static_cast<int>(_pos.y() + allg_height / 2 - _cam_pos.y() - 0.38 * _anim.size().y()));
	}

	void Player::updAI()
	{
		if (_tower)
		{
			_pos(_tower->collision().x(), _tower->collision().y() - _tower->anim().size().y() * _tower->anim().scale().y() / 2 + 8);
			_use_phys = false;
		}
		else
		{
			_use_phys = true;
			_multiplier = 0;
			
			if (Keyboard::instance().find(_left))
			{
				_multiplier = -_walk_speed;
				_light.angle(PI);
			}
			if (Keyboard::instance().find(_right))
			{
				_multiplier += _walk_speed;
				_light.angle(0);
			}
			
			if (Keyboard::instance().find(_run))
				_multiplier *= 1.6;
			
			walkAI(_multiplier);
			
			if (Keyboard::instance().find(_up))
				jump(4);
		}
		
		if (Keyboard::instance().find(_attack) && (_projectiles.size() < max_projectiles) && Map::instance().enemies().size())
		{
			if (_attack_timer())
			{
				static Enemy *e;
				static double spd;
				static double angle;
				static double dx, dy;
				spd = kunai_speed;
				e = *Map::instance().enemies().begin();
				for (auto &i : Map::instance().enemies())
				{
					if (i->collision().distance(_pos) < e->collision().distance(_pos))
						e = i;
				}
				dx = e->collision().x() - _pos.x();
				dy = e->collision().y() - _pos.y();
				if (dy == 0.0)
					_projectiles.push_back(new Kunai(_pos.x(), _pos.y(), spd * dx / abs(dx), - 1, _damage, this));
				else
				{
					angle = atan(((spd * spd) / (gravity * abs(dx))) -
								 sqrt(((spd * spd * spd * spd) / (gravity * gravity * dx * dx)) - ((2 * spd * spd * abs(dy)) / (gravity * dx * dx))));
					_projectiles.push_back(new Kunai(_pos.x(), _pos.y(), (spd * cos(angle)) * dx / abs(dx), (spd * sin(angle)) * dy / abs(dy) - 1, _damage, this));
				}
			}
		}
	}

	void Player::updProj()
	{
		static Container<Kunai*> removed;
		for(auto &i : _projectiles)
		{
			i->update();
			if(i->faded()) removed.push_back(i);
		}
		for(auto &i : removed)
		{
			_projectiles.erase(i);
			delete i;
		}
		removed.clear();
	}

	Player::Player(string path)
			: _up(ALLEGRO_KEY_W),
			  _left(ALLEGRO_KEY_A),
			  _right(ALLEGRO_KEY_D),
			  _run(ALLEGRO_KEY_LSHIFT),
			  _attack(ALLEGRO_KEY_F),
			  _mount(ALLEGRO_KEY_R),
			  _flash(ALLEGRO_KEY_E),
			  _multiplier(1),
			  _draw_light(0),
			  _tower(nullptr),
			  _attack_timer(0.2),
			  _score(0)
	{
		_walk_speed = 2.4;
		_use_ai = false;
		_life = 1000;
		_damage = 100;
		Map::instance().update();
		_anim.load(path, 0.04, 10, 2);
		_default_interval = 0.04;
		_pos.bounds()(11, 22);
		_pos.y() = Map::instance().chunks()[1]->pos().y() - (block_size + _anim.size().y() * _anim.scale().y()) / 2;
		_light.distance(allg_width / 2);
		_light.colorize(20,20,20,0);
		int flag = 8;
		while(flag > 0)
		{
			for(auto &i : Map::instance().chunks()[1]->blocks())
			{
				if(abs(i->colBox().x() - _pos.x()) < block_size / 2)
				{
					flag = 0;
					break;
				}
			}
			if(flag) _pos.x() += block_size;
			flag--;
		}
		if(flag == 0)
		{
			while(flag != 0)
			{
				for(auto &i : Map::instance().chunks()[1]->blocks())
				{
					if(abs(i->colBox().x() - _pos.x()) < block_size)
					{
						flag = 0;
						break;
					}
				}
				_pos.x() -= block_size;
			}
		}
	}
	Player::~Player()
	{
		if(this == _player1) _player1 = nullptr;
		else if(this == _player2) _player2 = nullptr;
		for(auto &i : _projectiles)
			delete i;
	}

	const int &Player::mount() const
	{
		return _mount;
	}
	const int &Player::attack() const
	{
		return _attack;
	}
	Tower *&Player::tower()
	{
		return _tower;
	}
	const Tower *Player::tower() const
	{
		return _tower;
	}

	void Player::fade()
	{
		_life.value() -= _life.max() / 4;
		_pos.y() = Map::instance().chunks()[1]->pos().y() - 2 * block_size;
		static int flag;
		flag = 8;
		while(flag > 0)
		{
			for(auto &i : Map::instance().chunks()[1]->blocks())
			{
				if(abs(i->colBox().x() - _pos.x()) < block_size / 2)
				{
					flag = 0;
					break;
				}
			}
			if(flag) _pos.x() += block_size;
			flag--;
		}
		if(flag == 0)
		{
			while(flag != 0)
			{
				for(auto &i : Map::instance().chunks()[1]->blocks())
				{
					if(abs(i->colBox().x() - _pos.x()) < block_size)
					{
						flag = 0;
						break;
					}
				}
				_pos.x() -= block_size;
			}
		}
		_speed(0,0);
		_accel(0,0);
		if(_life.value() <= 0)
			_faded = true;
	}

	const int &Player::score() const
	{
		return _score;
	}
	int &Player::score()
	{
		return _score;
	}

	void Player::update()
	{
		static int div;
		div = 0;
		_cam_pos(0,0);
		if(_player1)
		{
			_cam_pos += _player1->_pos;
			div++;
		}
		if(_player2)
		{
			_cam_pos += _player2->_pos;
			div++;
		}
		if(div) _cam_pos /= div;
		_cam_pos.y() -= 100;
		if(_player1)
		{
			_player1->drawLight();
			if(!_player1->_tower)
				_player1->Entity::update();
			_player1->updProj();
			if(_player1->_faded)
			{
				delete _player1;
				_player1 = nullptr;
			}
			if(_player2)
			{
				static double lastx1;
				static double lastx2;
				if(_player1->_pos.distance(_player2->_pos) >= allg_width)
				{
					_player1->_accel.x() = 0;
					_player2->_accel.x() = 0;
					_player1->_pos.x() = lastx1;
					_player2->_pos.x() = lastx2;
				}
				lastx1 = _player1->_pos.x();
				lastx2 = _player2->_pos.x();
			}
		}
		if(_player2)
		{
			_player2->drawLight();
			if(!_player2->_tower)
				_player2->Entity::update();
			_player2->updProj();
			if(_player2->_faded)
			{
				delete _player2;
				_player2 = nullptr;
			}
		}
	}

	void Player::updateMounted()
	{
		if(_player1)
			if(_player1->_tower) _player1->Entity::update();
		if(_player2)
			if(_player2->_tower) _player2->Entity::update();
	}

	void Player::newPlayer1()
	{
		if(!_player1)
		{
			_player1 = new Player("../files/textures/sprites/player1.png");
		}
	}
	void Player::newPlayer2()
	{
		if(!_player2)
		{
			_player2 = new Player("../files/textures/sprites/player2.png");
			_player2->_up = ALLEGRO_KEY_I;
			_player2->_left = ALLEGRO_KEY_J;
			_player2->_right = ALLEGRO_KEY_L;
			_player2->_run = ALLEGRO_KEY_SLASH;
			_player2->_attack = ALLEGRO_KEY_H;
			_player2->_mount = ALLEGRO_KEY_Y;
			_player2->_flash = ALLEGRO_KEY_U;
		}
	}

	Player &Player::player1()
	{
		return *_player1;
	}
	Player &Player::player2()
	{
		return *_player2;
	}

	void Player::remPlayer()
	{
		if(_player1)
		{
			delete _player1;
			_player1 = nullptr;
		}
		if(_player2)
		{
			delete _player2;
			_player2 = nullptr;
		}
	}

	const Point &Player::camPos()
	{
		return _cam_pos;
	}

//											Enemy

//								WalkAround

	Enemy::WalkAround::WalkAround()
			: _direction(1),
			  _walk_timer(3)
	{
	
	}
	Enemy::WalkAround::~WalkAround()
	{
	
	}

	void Enemy::WalkAround::update()
	{
		if(static_cast<Enemy &>(entity())._follow)
		{
			if((rand()%10000 / 10000.0 < 0.006)&&_walk_timer.updated())
			{
				_walk_timer();
				_direction = (2 * (rand()%2) - 1) * entity().walkSpeed();
			}
			
			if(&Player::player1())
			{
				if(&Player::player2())
				{
					if(Player::player1().collision().distance(entity().collision()) < Player::player2().collision().distance(entity().collision()))
					{
						if(Player::player1().collision().distance(entity().collision()) < enemy_look_range)
						{
							state() = new FollowPlayer(&Player::player1());
						}
					}
					else if(Player::player2().collision().distance(entity().collision()) < enemy_look_range)
					{
						state() = new FollowPlayer(&Player::player2());
					}
				}
				else
				{
					if(Player::player1().collision().distance(entity().collision()) < enemy_look_range)
					{
						state() = new FollowPlayer(&Player::player1());
					}
				}
			}
			else if(&Player::player2())
			{
				if(Player::player2().collision().distance(entity().collision()) < enemy_look_range)
				{
					state() = new FollowPlayer(&Player::player2());
				}
			}
		}
		
		entity().walk(_direction);
	}

//								FollowPlayer

	Enemy::FollowPlayer::FollowPlayer(Player *p)
			: _player(p),
			  _multiplier(1)
	{
	
	}
	Enemy::FollowPlayer::~FollowPlayer()
	{
	
	}

	void Enemy::FollowPlayer::update()
	{
		if(_player)
		{
			if(static_cast<Enemy &>(entity())._follow)
			{
				if(entity().collision().x() > _player->collision().x())
					_multiplier = -entity().walkSpeed();
				else
					_multiplier = entity().walkSpeed();
				
				if(entity().life().value() < entity().life().max() / 3)
					_multiplier *= 1.6;
			}
			
			entity().walk(_multiplier);
			
			static_cast<Enemy &>(entity()).hit(_player);
			
			if(_player->life().value() <= 0)
			{
				_player = nullptr;
			}
		}
		else state() = new WalkAround();
	}

//								Explode

	Enemy::Explode::Explode()
			: _destroyed(false)
	{
	
	}
	Enemy::Explode::~Explode()
	{
	
	}

	void Enemy::Explode::update()
	{
		static int i;
		if(!_blood.size() && _destroyed)
			entity().faded() = true;
		else if(!_blood.size())
		{
			_destroyed = true;
			entity().drawnable() = false;
			entity().usePhys() = false;
			for(i = 0; i < blood_number; i++)
			{
				_blood.push_back(
						new FleshBall(entity().collision().x(),
									  entity().collision().y(),
									  (rand() % 10000 / 10000.0 - 0.5) * 0.3 * blood_speed,
									  (rand() % 10000 / 50000.0 - 1.0) * blood_speed,
									  entity().damage() / 2));
			}
		}
		
		static Container<FleshBall *> removed;
		for(auto &f : _blood)
		{
			f->update();
			if(f->faded()) removed.push_back(f);
		}
		for(auto &f : removed)
		{
			_blood.erase(f);
			delete f;
		}
		removed.clear();
	}

	void Enemy::updAI()
	{
		_current_state.update();
	}

	Enemy::Enemy(double x, double y)
			: _current_state(this, new WalkAround()),
			  _jump_case(0),
			  _follow(true),
			  _hit(1),
			  _htimer(0.2)
	{
		_walk_speed = 1.5;
		_pos(x, y);
	}
	Enemy::~Enemy()
	{
	
	}

	void Enemy::hit(Player *p)
	{
		if(p->tower())
		{
			if(_hit.updated()&&_htimer())
			{
				if (_pos.onCol(p->tower()->collision()) != BoxCol::ColType::none)
					p->tower()->life().value() -= _damage;
			}
		}
		else if(_pos.onCol(p->collision()) != BoxCol::ColType::none)
			if(_hit.updated()&&_htimer())
			{
				_hit();
				p->life().value() -= _damage;
			}
	}

	void Enemy::walk(double speed)
	{
		if(_block)
		{
			jump(4);
			walkAI(speed);
		}
		else if(_hole)
		{
			if(_jump_case == 0) _jump_case = 1;
			if(_jump_case == 1)
			{
				walkAI(0.0);
				if((!_jumping)&&(_speed.x() == 0.0)) _jump_case = 2;
			}
			else if(_jump_case == 2)
			{
				_follow = false;
				jump(4);
				walkAI(speed);
			}
		}
		else
			walkAI(speed);
		if(!_hole)
		{
			_jump_case = 0;
			if(!_jumping) _follow = true;
		}
	}

	Enemy *Enemy::get(double x, double y)
	{
		if(random()%10000 / 10000.0 < zombie_random_value)
		{
			return new Zombie(x, y);
		}
		else if(random()%10000 / 10000.0 < spitter_random_value)
		{
			return new SpitterZombie(x, y);
		}
		else if(random()%10000 / 10000.0 < slime_random_value)
		{
			return new Slime(x, y);
		}
		else
		{
			return new Slime(x, y);
		}
	}

//											FleshBall

	void FleshBall::updAI()
	{
		if(&Player::player1())
		{
			hit(&Player::player1());
		}
		if(&Player::player2())
		{
			if(_life.value() >= 0)
				hit(&Player::player2());
		}
		if(_on_col) _life.value() = -1;
	}

	FleshBall::FleshBall(double x, double y, double sx, double sy, double d)
			: Enemy(x, y)
	{
		_walk_speed = abs(sx);
		_damage = d;
		_pos(x, y);
		_accel(sx, sy);
		_use_ai = false;
		_anim.load("../files/textures/sprites/blood.png", 0.0, 1, 1);
		_pos.bounds()(2, 2);
		_jumping = true;
	}
	FleshBall::~FleshBall()
	{
	
	}

	void FleshBall::hit(Player *p)
	{
		if(p->tower())
		{
			if(_pos.onCol(p->tower()->collision()) != BoxCol::ColType::none)
				p->tower()->life().value() -= _damage;
		}
		else if(_pos.onCol(p->collision()) != BoxCol::ColType::none)
		{
			p->life().value() -= _damage;
			_life.value() = -1;
		}
	}

//											Zombie

	Zombie::Zombie(double x, double y)
			: Enemy(x, y)
	{
		_life = 400;
		_damage = 40;
		_anim.load("../files/textures/sprites/zombie.png", 0.05, 8, 2);
		_default_interval = 0.05;
		_pos.bounds()(11, 22);
	}
	Zombie::~Zombie()
	{
	
	}

//											SpitterZombie

	void SpitterZombie::updAI()
	{
		static Container<FleshBall *> removed;
		Enemy::updAI();
		for(auto &i : _spit)
		{
			i->update();
			if(i->faded()) removed.push_back(i);
		}
		for(auto &i : removed)
		{
			_spit.erase(i);
			delete i;
		}
		removed.clear();
	}

	SpitterZombie::SpitterZombie(double x, double y)
			: Zombie(x, y),
			  _spit_timer(0.3)
	{
		_life = 500;
		_damage = 50;
		_hit = 3;
	}
	SpitterZombie::~SpitterZombie()
	{
	
	}

	void SpitterZombie::hit(Player *p)
	{
		if(_pos.distance(p->collision()) <= blood_range)
			if(_hit.updated()&&_spit_timer())
			{
				_hit();
				static int i;
				
				static int multipiler;
				if(_pos.x() < p->collision().x()) multipiler = 1;
				else multipiler = -1;
				
				for(i = 0; i < blood_number; i++)
				{
					_spit.push_back(
							new FleshBall(_pos.x(),
									 _pos.y() - _anim.size().y() * 0.7,
									 (rand() % 10000 / 50000.0 + 1.0) * blood_speed * multipiler,
									 (rand() % 10000 / 50000.0 - 0.2) * blood_speed,
									 _damage / 4));
				}
				if(p->tower())
				{
					if(_pos.onCol(p->tower()->collision()) != BoxCol::ColType::none)
						p->tower()->life().value() -= _damage;
				}
				else if(_pos.onCol(p->collision()) != BoxCol::ColType::none)
					p->life().value() -= _damage;
			}
	}

//											Slime

	Slime::Slime(double x, double y)
			: Enemy(x, y),
			  _jump_timer(2.5)
	{
		_walk_anim = 0;
		_life = 200;
		_damage = 20;
		_anim.load("../files/textures/sprites/slime.png", 0.2, 4, 1);
		_default_interval = 0.2;
		_pos.bounds()(10, 10);
	}
	Slime::~Slime()
	{
	
	}

	void Slime::walkAI(double speed)
	{
		jump(4);
		if(_jumping)
			Entity::walkAI(speed);
		else
			Entity::walkAI(0.0);
	}
	void Slime::jump(double speed)
	{
		if(_jump_timer())
			Entity::jump(speed);
	}

	void Slime::fade()
	{
		if(_drawnable)
			_current_state = new Explode();
	}

//											Kunai

	void Kunai::updAI()
	{
		_anim.angle(_speed.y() / _speed.x());
		for(auto &e : Map::instance().enemies())
		{
			if(_pos.onCol(e->collision()) != BoxCol::ColType::none)
			{
				e->life().value() -= _damage;
				_life.value() = -1;
				_player->score() += _damage;
			}
		}
		if(_on_col||_fade_timer()) _life.value() = -1;
	}

	Kunai::Kunai(double x, double y, double sx, double sy, double d, Player *p)
			: _fade_timer(2.5),
			  _player(p)
	{
		_damage = d;
		_pos(x, y);
		_accel(sx, sy);
		_use_ai = false;
		_jumping = true;
		_anim.load("../files/textures/items/kunai.png", 0.0, 1, 1);
		_pos.bounds()(4, 4);
		if(sx < 0) _anim.scale().x() *= -1;
	}
	Kunai::~Kunai()
	{
	
	}

//											Boss

	Boss *Boss::_instance;

//								Wait

	Boss::Wait::Wait()
			: _timer(4)
	{
	
	}
	Boss::Wait::~Wait()
	{
	
	}

	void Boss::Wait::update()
	{
		if(_timer())
		{
			if(rand()%2)
			{
				if(&Player::player1())
				{
					if(&Player::player2())
					{
						if(Player::player1().collision().distance(entity().collision()) < Player::player2().collision().distance(entity().collision()))
						{
							if(Player::player1().collision().distance(entity().collision()) < enemy_look_range)
							{
								state() = new Attack(&Player::player1());
							}
						}
						else if(Player::player2().collision().distance(entity().collision()) < enemy_look_range)
						{
							state() = new Attack(&Player::player2());
						}
					}
					else
					{
						if(Player::player1().collision().distance(entity().collision()) < enemy_look_range)
						{
							state() = new Attack(&Player::player1());
						}
					}
				}
				else if(&Player::player2())
				{
					if(Player::player2().collision().distance(entity().collision()) < enemy_look_range)
					{
						state() = new Attack(&Player::player2());
					}
				}
			}
			else state() = new SpawnEnemies();
		}
	}

//								Attack

	Boss::Attack::Attack(Player *p)
			: _player(p),
			  _timer(10),
			  _tp_timer(1),
			  _speed(0.0),
			  _mult(1)
	{
	
	}
	Boss::Attack::~Attack()
	{
	
	}

	void Boss::Attack::update()
	{
		if(_tp_timer())
		{
			entity().pos(Player::camPos().x() + _mult * allg_width / 2, _player->collision().y());
			_speed =  - _mult * entity().walkSpeed();
			_mult *= -1;
		}
		
		static_cast<Boss &>(entity()).walkAI(_speed);
		static_cast<Enemy &>(entity()).hit(_player);
		
		if(_timer())
		{
			if(rand()%2)
				state() = new SpawnEnemies();
			else
				state() = new Wait();
		}
	}

//								SpawnEnemies

	Boss::SpawnEnemies::SpawnEnemies()
			: _timer(1.3),
			  _sp_timer(0.3)
	{
	
	}
	Boss::SpawnEnemies::~SpawnEnemies()
	{
	
	}

	void Boss::SpawnEnemies::update()
	{
		if(_sp_timer())
			Map::instance().addEnemy(Enemy::get(entity().collision().x() + 20 * (rand()%7 - 3), Map::instance().chunks()[1]->pos().y() - 2 * block_size));
		
		if(_timer())
			state() = new Wait();
	}

	void Boss::updAI()
	{
		Enemy::updAI();
		if(Player::camPos().distance(_pos) >= allg_width / 3)
		{
			static double speed;
			if(Player::camPos().x() > _pos.x())
				speed = _walk_speed;
			else
				speed = -_walk_speed;
			
			walkAI(speed);
		}
		else
			walkAI(0.0);
		if((Player::camPos().x() - _pos.x()) * _anim.scale().x() < 0) _anim.scale().x() *= -1;
		_light.draw(static_cast<int>(_pos.x() + allg_width / 2 - Player::camPos().x()),
					static_cast<int>(_pos.y() + allg_height / 2 - Player::camPos().y()));
	}

	Boss::Boss(double x, double y)
			: Enemy(x, y)
	{
		_use_col = false;
		_walk_speed = boss_speed;
		_use_grav = false;
		_walk_anim = 0;
		_instance = this;
		_life = 10000;
		_damage = 250;
		_anim.load("../files/textures/sprites/boss.png", 0.15, 5, 1);
		_default_interval = 0.15;
		_pos.bounds()(20, 30);
		_current_state = new SpawnEnemies();
		_light.radius(150);
		_light.colorize(20, 20, 20, 20);
	}
	Boss::~Boss()
	{
		_instance = nullptr;
	}

	void Boss::fade()
	{
		if(_drawnable)
			_current_state = new Explode();
	}

	Boss &Boss::instance(double x, double y)
	{
		if(!_instance) _instance = new Boss(x, y);
		return *_instance;
	}
	void Boss::destroy()
	{
		if(_instance) delete _instance;
		_instance = nullptr;
	}

//												Portal

	void Portal::updAI()
	{
		static bool b1;
		static bool b2;
		b1 = false;
		b2 = false;
		
		if(&Player::player1())
		{
			b1 = (_pos.onCol(Player::player1().collision()) != BoxCol::ColType::none)?(Keyboard::instance().find(Player::player1().attack())):false;
			
		}
		if(&Player::player2())
		{
			b2 = (_pos.onCol(Player::player2().collision()) != BoxCol::ColType::none)?(Keyboard::instance().find(Player::player2().attack())):false;
		}
		_activated = b1||b2;
	}

	Portal::Portal()
			: _activated(false)
	{
		_anim.load("../files/textures/background/portal.png", 0.0, 1, 1);
		_pos.bounds() = _anim.size() / 2;
		_use_phys = false;
		_use_ai = false;
	}
	Portal::~Portal()
	{
	
	}

	Portal::operator bool()
	{
		return _activated;
	}

#include "map.h"
#include "display.h"

//										Chunk

	Container<Chunk::Block *> Chunk::_border;
	int Chunk::_hole = 0;
	int Chunk::_tower = 0;
	int Chunk::_hgt = 0;

	Chunk::~Chunk()
	{
		for(auto &i : _all)
			delete i;
		for(auto &i : _towers)
			delete i;
	}

	void Chunk::addBlock(Chunk::Block *b)
	{
		_all.push_back(b);
		_surface.push_back(b);
	}

	Container<Chunk::Block *> & Chunk::blocks()
	{
		return _surface;
	}

const Point &Chunk::pos()
	{
		return _pos;
	}

	const double &Chunk::maxPosition() const
	{
		return _max_position;
	}

	void Chunk::update()
	{
		static  Container<Tower*> removed;
		removed.clear();
		for(auto &i : _all)
		{
			i->draw();
		}
		for(auto &i : _towers)
		{
			i->update();
			if(i->faded())
				removed.push_back(i);
		}
		for(auto &i : removed)
		{
			_towers.erase(i);
			delete i;
		}
	}

	void Chunk::destroy()
	{
		_border.clear();
		_hole = 0;
		_tower = 0;
		_hgt = 0;
	}

//								Block

	Chunk::Block::Block()
	{
	
	}
	Chunk::Block::Block(string path)
	{
		_texture.load(path);
		_pos.bounds() = _texture.scale().prod(_texture.size()) / 2;
	}
	Chunk::Block::~Block()
	{
	
	}

	const BoxCol &Chunk::Block::colBox() const
	{
		return _pos;
	}

	void Chunk::Block::draw()
	{
		_texture.draw(static_cast<int>(_pos.x() + allg_width / 2 - Player::camPos().x()),
					  static_cast<int>(_pos.y() + allg_height / 2 - Player::camPos().y()));
		if(Display::instance().showInfo()&&GameTimer::instance().fpsLimiter())
		{ // TODO
			al_draw_filled_rectangle(static_cast<float>(_pos.x() + allg_width / 2 - Player::camPos().x() - _pos.bounds().x()),
									 static_cast<float>(_pos.y() + allg_height / 2 - Player::camPos().y() - _pos.bounds().y()),
									 static_cast<float>(_pos.x() + allg_width / 2 - Player::camPos().x() + _pos.bounds().x()),
									 static_cast<float>(_pos.y() + allg_height / 2 - Player::camPos().y() + _pos.bounds().y()),
									 collider_color);
		}
	}
	void Chunk::Block::pos(double posx, double posy)
	{
		_pos(posx, posy);
	}
	void Chunk::Block::bounds(double posx, double posy)
	{
		_pos.bounds()(posx, posy);
	}
	const Point &Chunk::Block::size()
	{
		return _texture.size();
	}

//								Barrier

	Chunk::Barrier::Barrier()
	{
	
	}
	Chunk::Barrier::Barrier(double x, double y)
	{
		bounds(block_size / 2, 100 * block_size);
		pos(x, y);
	}
	Chunk::Barrier::~Barrier()
	{
	
	}

//								Dirt

	Chunk::Dirt::Dirt()
	{
	
	}
	Chunk::Dirt::Dirt(double x, double y)
			: Block("../files/textures/blocks/dirt.png")
	{
		pos(x, y);
	}
	Chunk::Dirt::~Dirt()
	{
	
	}

//								Grass

	Chunk::Grass::Grass()
	{
	
	}
	Chunk::Grass::Grass(double x, double y)
			: Block("../files/textures/blocks/grass.png")
	{
		pos(x, y);
	}
	Chunk::Grass::~Grass()
	{
	
	}

//								Stone

	Chunk::Stone::Stone()
	{
	
	}
	Chunk::Stone::Stone(double x, double y)
			: Block("../files/textures/blocks/stone.png")
	{
		pos(x, y);
	}
	Chunk::Stone::~Stone()
	{
	
	}

//								Brick

	Chunk::Brick::Brick()
	{
	
	}
	Chunk::Brick::Brick(double x, double y)
			: Block("../files/textures/blocks/brick.png")
	{
		pos(x, y);
	}
	Chunk::Brick::~Brick()
	{
	
	}

//								HellGrass

	Chunk::HellGrass::HellGrass()
	{
	
	}
	Chunk::HellGrass::HellGrass(double x, double y)
			: Block("../files/textures/blocks/hellgrass.png")
	{
		pos(x, y);
	}
	Chunk::HellGrass::~HellGrass()
	{
	
	}

//								HellStone

	Chunk::HellStone::HellStone()
	{
	
	}
	Chunk::HellStone::HellStone(double x, double y)
			: Block("../files/textures/blocks/hellstone.png")
	{
		pos(x, y);
	}
	Chunk::HellStone::~HellStone()
	{
	
	}

//								HellBrick

	Chunk::HellBrick::HellBrick()
	{
	
	}
	Chunk::HellBrick::HellBrick(double x, double y)
			: Block("../files/textures/blocks/hellbrick.png")
	{
		pos(x, y);
	}
	Chunk::HellBrick::~HellBrick()
	{
	
	}

//											Map

	Map::Map()
			: _spawn(default_spawn_timer),
			  _posx(0),
			  _height(0),
			  _spawn_chance(0.5)
	{
	
	}
	Map::~Map()
	{
	
	}

	void Map::spawnChance(double d)
	{
		_spawn_chance = d;
	}

	void Map::destroy()
	{
		for(auto &i : _chunks)
		{
			delete i;
		}
		for(auto &i : _enemies)
		{
			delete i;
		}
		_chunks.clear();
		_cur_chunks.clear();
		_enemies.clear();
		_posx = -platform_limits_x;
		_height = 0;
		Chunk::destroy();
	}

	const double &Map::maxPosition() const
	{
		return _max_position;
	}

	Container<Chunk *> &Map::chunks()
	{
		return _cur_chunks;
	}

	Container<Enemy *> & Map::enemies()
	{
		return _enemies;
	}

	void Map::update()
	{
		static Container<Chunk*>::iterator last;
		static Container<Enemy*> removed;
		last = it1;
		while((abs(Player::camPos().x() - it0->pos().x()) < abs(Player::camPos().x() - it1->pos().x())) && it0.prev())
		{
			it0--;
			it1--;
			it2--;
		}
		while((abs(Player::camPos().x() - it2->pos().x()) < abs(Player::camPos().x() - it1->pos().x())) && it2.next())
		{
			it0++;
			it1++;
			it2++;
		}
		if(!(last == it1))
		{
			_cur_chunks.clear();
			_cur_chunks.push_back(*it0);
			_cur_chunks.push_back(*it1);
			_cur_chunks.push_back(*it2);
		}
		for(auto &i : _cur_chunks)
		{
			i->update();
		}
		if(_enemies.size() < max_enemies)
		{
			if(_spawn())
			{
				if(rand()%10000 / 10000.0 < _spawn_chance * default_spawn_chance)
				{
					static int pos;
					static int flag;
					static double d;
					pos = (rand()%2) * 2;
					d = Player::camPos().x() + (pos - 1) * allg_width * chunk_scale;
					flag = 8;
					while(flag > 0)
					{
						for(auto &i : _cur_chunks[pos]->blocks())
						{
							if(abs(i->colBox().x() - d) < block_size / 2)
							{
								flag = 0;
								break;
							}
						}
						if(flag) d += block_size;
						flag--;
					}
					if(flag == 0)
					{
						while(flag != 0)
						{
							for(auto &i : _cur_chunks[pos]->blocks())
							{
								if(abs(i->colBox().x() - d) < block_size)
								{
									flag = 0;
									break;
								}
							}
							d -= block_size;
						}
					}
					_enemies.push_back(Enemy::get(d, _cur_chunks[pos]->pos().y() - 2 * block_size));
				}
			}
		}
		for(auto &i : _queue)
		{
			if(_enemies.size() < max_enemies) _enemies.push_back(i);
			else break;
		}
		_queue.clear();
		for(auto &i : _enemies)
		{
			i->update();
			if(i->faded()) removed.push_back(i);
		}
		for(auto &i : removed)
		{
			_enemies.erase(i);
			if(i == &Boss::instance())
				Boss::destroy();
			else
				delete i;
		}
		removed.clear();
	}

	Map &Map::instance()
	{
		static Map inst;
		return inst;
	}

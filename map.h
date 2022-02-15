#ifndef MAP_H
#define MAP_H

#include "container.h"
#include "geometry.h"
#include "graphics.h"
#include "entity.h"

class Chunk final
{
private:
	class Block
	{
	private:
		BoxCol _pos;
		Image _texture;
	public:
		Block();
		Block(string path);
		virtual ~Block();
		
		const BoxCol &colBox() const;
		
		void draw();
		void pos(double posx, double posy);
		void bounds(double posx, double posy);
		const Point &size();
	};
public:
	class Barrier final : public Block
	{
	public:
		Barrier();
		Barrier(double x, double y);
		~Barrier();
	};
	
	class Dirt final : public Block
	{
	public:
		Dirt();
		Dirt(double x, double y);
		~Dirt();
	};
	
	class Grass final : public Block
	{
	public:
		Grass();
		Grass(double x, double y);
		~Grass();
	};
	
	class Stone final : public Block
	{
	public:
		Stone();
		Stone(double x, double y);
		~Stone();
	};
	
	class Brick final : public Block
	{
	public:
		Brick();
		Brick(double x, double y);
		~Brick();
	};
	
	class HellGrass final : public Block
	{
	public:
		HellGrass();
		HellGrass(double x, double y);
		~HellGrass();
	};
	
	class HellStone final : public Block
	{
	public:
		HellStone();
		HellStone(double x, double y);
		~HellStone();
	};
	
	class HellBrick final : public Block
	{
	public:
		HellBrick();
		HellBrick(double x, double y);
		~HellBrick();
	};
private:
	static Container<Block *> _border;
	static int _hole;
	static int _tower;
	static int _hgt;
	
	Container<Block*> _all;
	Container<Block*> _surface;
	Container<Tower*> _towers;
	
	Point _pos;
	
	double _max_position;
public:
	template <typename GRASS, typename DIRT, typename STONE, typename TOWER>
	inline Chunk(double &posx, double &height, const GRASS &, const DIRT &, const STONE &, const TOWER &, double chance)
	{
		_pos(posx + static_cast<int>(allg_width * chunk_scale / (block_size)) * block_size / 2, height);
		_max_position = 0;
		static Point i;
		static Point incr;
		static bool flag = true;
		incr(block_size, block_size);
		
		for(i(posx, 0); i.x() < static_cast<int>(allg_width * chunk_scale / (block_size)) * block_size + posx; i.x() += incr.x())
		{
			if((rand()%10000 / 10000.0 < platform_random_holes * chance)&&(_hole <= 0))
			{
				i.x() += incr.x() * (rand() % 3 + 1);
				_hole = 4;
				_tower = 3;
				_hgt = 0;
				for(auto &bl : _border)
					_surface.push_back(bl);
			}
			else
			{
				flag = (_hole == 4);
				_border.clear();
				_hole--;
				if((rand()%10000 / 10000.0 < platform_random_height * chance)&&(_hgt <= 0))
				{
					height += incr.y() * (rand()%3 - 1);
					if(rand()%10000 / 10000.0 < platform_random_height)
						height += incr.y() * (rand()%3 - 1);
					_hole = 3;
					_tower = 3;
					_hgt = 0;
				}
				else _hgt--;
				i.y() = height;
				
				if((rand()%10000 / 10000.0 < platform_random_towers * chance)&&(_tower <= 0))
				{
					_towers.push_back(new TOWER(i.x(), i.y() - block_size / 2));
					_hole = 3;
					_tower = 3;
					_hgt = 3;
				}
				else _tower--;
				
				addBlock(new GRASS(i.x(), i.y()));
				
				i.y() += incr.y();
				
				for(; i.y() < height + 4 * block_size; i.y() += incr.y())
					addBlock(new DIRT(i.x(), i.y()));
				
				for(; i.y() < height + 10 * block_size; i.y() += incr.y())
					addBlock(new STONE(i.x(), i.y()));
				
				if(i.y() > _max_position) _max_position = i.y();
				if(height < _pos.y()) _pos.y() = height;
			}
			if(flag)
			{
				for(auto &bl : _border)
					_surface.push_back(bl);
			}
		}
		for(auto &bl : _border)
			_surface.push_back(bl);
		_border.clear();
		posx = i.x();
	}
	~Chunk();
	
	void addBlock(Block *b);
	
	Container<Block*> &blocks();
	
	const Point &pos();
	
	const double &maxPosition() const;
	
	void update();
	
	static void destroy();
};

class Map final
{
private:
	Container<Chunk*> _chunks;
	Container<Chunk*> _cur_chunks;
	
	Container<Chunk*>::iterator it0;
	Container<Chunk*>::iterator it1;
	Container<Chunk*>::iterator it2;
	
	Container<Enemy*> _enemies;
	Container<Enemy*> _queue;
	
	Timer _spawn;
	
	double _max_position;
	
	double _posx;
	double _height;
	
	double _spawn_chance;
	
	Map();
	
	Map(const Map &) = delete;
	Map(Map &&) = delete;
	
	void operator=(const Map &) = delete;
	void operator=(Map &&) = delete;
public:
	~Map();
	
	template <typename GRASS, typename DIRT, typename STONE, typename TOWER>
	inline bool build(const GRASS &, const DIRT &, const STONE &, const TOWER &, double limits, double chance)
	{
		static Chunk *c = nullptr;
		if(!_chunks.size()) _posx = -limits;
		if(_posx <= limits)
		{
			c = new Chunk(_posx, _height, GRASS(), DIRT(), STONE(), TOWER(), chance);
			_chunks.push_back(c);
			if(c->maxPosition() > _max_position) _max_position = c->maxPosition();
			return true;
		}
		else
		{
			_chunks.begin()->addBlock(new Chunk::Barrier(-limits + 3 * block_size, _height));
			_chunks.end()->addBlock(new Chunk::Barrier(_posx - 4 * block_size, _height));
			it0 = _chunks.begin();
			it1 = it0;
			it1++;
			it2 = it1;
			it2++;
			_cur_chunks.push_back(*it0);
			_cur_chunks.push_back(*it1);
			_cur_chunks.push_back(*it2);
			return false;
		}
	}
	
	void spawnChance(double d);
	
	void destroy();
	
	void addEnemy(Enemy *e)
	{
		_queue.push_back(e);
	}
	
	const double &maxPosition() const;
	
	Container<Chunk*> &chunks();
	
	Container<Enemy*> &enemies();
	
	void update();
	
	static Map &instance();
};

#endif

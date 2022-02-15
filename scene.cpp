#include "scene.h"
#include "map.h"
#include "input.h"

//											MainMenu

	MainMenu::MainMenu()
	{
		_play.pos(allg_width / 2, 3 * allg_height / 7);
		_quit.pos(allg_width / 2, 4 * allg_height / 7);
		_play.text("Play");
		_quit.text("Quit");
		_quit.colorizeStandard(100, 10, 40, 120);
		_quit.colorizeHover(255, 0, 0, 200);
		_background.load(sky_background);
		_background.distance(-1);
		_title.fontSize(40);
		_title.text("Ninjas Vs Zombies");
	}
	MainMenu::~MainMenu()
	{
	
	}

	void MainMenu::update()
	{
		_background.draw();
		_title.draw(allg_width / 2, allg_height / 6);
		_play.update();
		_quit.update();
		if(_play)
		{
			scene() = new SelectPlayers();
		}
		else if(_quit)
		{
			Error::instance().error() = Error::err::allg_close;
		}
	}

//											SelectPlayers

	SelectPlayers::SelectPlayers()
	{
		_1player.pos(allg_width / 2, 3 * allg_height / 8);
		_2player.pos(allg_width / 2, 4 * allg_height / 8);
		_back.pos(allg_width / 2, 5 * allg_height / 8);
		_1player.text("1 Player");
		_2player.text("2 Players");
		_back.text("Back");
		_back.colorizeStandard(100, 10, 40, 120);
		_back.colorizeHover(200, 10, 40, 160);
		_background.load(sky_background);
		_background.distance(-1);
		_title.fontSize(40);
		_title.text("Select Players");
	}
	SelectPlayers::~SelectPlayers()
	{
	
	}

	void SelectPlayers::update()
	{
		_background.draw();
		_title.draw(allg_width / 2, allg_height / 6);
		_1player.update();
		_2player.update();
		_back.update();
		if(_1player)
		{
			scene() = new LoadMap(1);
		}
		else if(_2player)
		{
			scene() = new LoadMap(2);
		}
		else if(_back)
		{
			scene() = new MainMenu();
		}
	}

//											LoadMap

	LoadMap::LoadMap(int players)
			: _players(players)
	{
		_new.pos(allg_width / 2, 3 * allg_height / 7);
		_back.pos(allg_width / 2, 4 * allg_height / 7);
		_new.text("New Map");
		_back.text("Back");
		_back.colorizeStandard(100, 10, 40, 120);
		_back.colorizeHover(200, 10, 40, 160);
		_background.load(sky_background);
		_background.distance(-1);
		_title.fontSize(40);
		_title.text("     Play     ");
	}
	LoadMap::~LoadMap()
	{
	
	}

	void LoadMap::update()
	{
		_background.draw();
		_title.draw(allg_width / 2, allg_height / 6);
		_new.update();
		_back.update();
		if(_new)
		{
			scene() = new SelectPhase(_players);
		}
		else if(_back)
		{
			scene() = new SelectPlayers();
		}
	}

//											SelectPhase

	SelectPhase::SelectPhase(int players)
			: _players(players)
	{
		_phase1.pos(allg_width / 2, 3 * allg_height / 8);
		_phase2.pos(allg_width / 2, 4 * allg_height / 8);
		_phase3.pos(allg_width / 2, 5 * allg_height / 8);
		_back.pos(allg_width / 2, 6 * allg_height / 8);
		_phase1.text("Phase 1");
		_phase2.text("Phase 2");
		_phase3.text("Phase 3");
		_back.text("Back");
		_phase1.colorizeStandard(20, 140, 20, 160);
		_phase1.colorizeHover(20, 200, 20, 200);
		_phase2.colorizeStandard(140, 140, 40, 160);
		_phase2.colorizeHover(200, 200, 40, 200);
		_phase3.colorizeStandard(180, 10, 40, 200);
		_phase3.colorizeHover(255, 0, 0, 200);
		_back.colorizeStandard(100, 10, 40, 120);
		_back.colorizeHover(200, 10, 40, 160);
		_background.load(sky_background);
		_background.distance(-1);
		_title.fontSize(40);
		_title.text("Select Phase");
	}
	SelectPhase::~SelectPhase()
	{
	
	}

	void SelectPhase::update()
	{
		_background.draw();
		_title.draw(allg_width / 2, allg_height / 6);
		_phase1.update();
		_phase2.update();
		_phase3.update();
		_back.update();
		if(_phase1)
		{
			scene() = new BuildingMap(true, _players == 2, 0, 0, 0);
		}
		else if(_phase2)
		{
			scene() = new BuildingMap(true, _players == 2, 1, 0, 0);
		}
		else if(_phase3)
		{
			scene() = new BuildingMap(true, _players == 2, 2, 0, 0);
		}
		else if(_back)
		{
			scene() = new LoadMap(_players);
		}
	}

//											BuildingMap

	BuildingMap::BuildingMap(bool p1, bool p2, int phase, int sc1, int sc2)
			: _player1(p1),
			  _player2(p2),
			  _timer(0.2),
			  _points(0),
			  _phase(phase),
			  _score1(sc1),
			  _score2(sc2)
	{
		_building.fontSize(40);
		_building.text("Building Map");
		_background.load(sky_background);
		_background.distance(-1);
		switch(phase)
		{
			case 0 :
				Map::instance().spawnChance(0.5);
				break;
			case 1 :
				Map::instance().spawnChance(0.8);
				break;
			case 2 :
				Map::instance().spawnChance(0.0);
				break;
			default:
				break;
		}
	}
	BuildingMap::~BuildingMap()
	{
	
	}

	void BuildingMap::update()
	{
		static bool b;
		switch(_phase)
		{
			case 0 :
				b = Map::instance().build(Chunk::Grass(), Chunk::Dirt(), Chunk::Stone(), Tower(), platform_limits_x, 1.0);
				break;
			case 1 :
				b = Map::instance().build(Chunk::HellGrass(), Chunk::Dirt(), Chunk::Brick(), RedTower(), platform_limits_x, 0.7);
				break;
			case 2 :
				b = Map::instance().build(Chunk::HellGrass(), Chunk::Dirt(), Chunk::HellStone(), RedTower(), allg_width * 3, 0.0);
				break;
			default:
				break;
		}
		if(b)
		{
			_background.draw();
			_building.draw(TextBox::Flag::center);
			if(_timer())
			{
				switch(_points)
				{
					case 0 :
						_building.text("Building Map.");
						_points++;
						break;
					case 1 :
						_building.text("Building Map..");
						_points++;
						break;
					case 2 :
						_building.text("Building Map...");
						_points++;
						break;
					case 3 :
						_building.text("Building Map");
						_points = 0;
						break;
					default :
						break;
				}
			}
		}
		else
		{
			if(_phase == 2) Map::instance().addEnemy(&Boss::instance(0, Map::instance().chunks()[1]->pos().y() - 6 * block_size));
			scene() = new Phase(_player1, _player2, _phase, _score1, _score2);
		}
	}

//											Phase

	Phase::Phase(bool p1, bool p2, int phase, int sc1, int sc2)
			: _show(false),
			  _flag(false),
			  _phase(phase),
			  _timer(10),
			  _active_portal(false)
	{
		if(p1)
		{
			Player::newPlayer1();
			Player::player1().score() = sc1;
		}
		if(p2)
		{
			Player::newPlayer2();
			Player::player2().score() = sc2;
		}
		switch(phase)
		{
			case 0 :
				_back.load(sky_background);
				break;
			case 1 :
				_back.load("../files/textures/background/sky1.png");
				break;
			case 2 :
				_back.load("../files/textures/background/sky2.png");
				_portal.anim().load("../files/textures/background/redportal.png", 0.0, 1, 1);
				break;
			default:
				break;
		}
		_back.distance(-1);
		_trees.load(game_background);
		_trees.distance(20);
		_menu.pos(allg_width / 2, 4 * allg_height / 10);
		_quit.pos(allg_width / 2, 6 * allg_height / 10);
		_menu.text("Menu");
		_quit.text("Quit");
		_quit.colorizeStandard(160, 10, 40, 160);
		_quit.colorizeHover(255, 0, 0, 200);
		_fade.limits(allg_width / 2, allg_height / 2);
		_fade.colorizeBorder(0, 0, 0, 0);
	}
	Phase::~Phase()
	{
		Player::remPlayer();
		Player::remPlayer();
		Map::instance().destroy();
	}

	void Phase::update()
	{
		_back.draw();
		_trees.draw();
		if(!_show)
		{
			if(_active_portal)
				_portal.update();
			Player::updateMounted();
			Map::instance().update();
			Player::update();
		}
		if(Keyboard::instance().find(ALLEGRO_KEY_ESCAPE)&&!_flag)
		{
			_show = !_show;
			_flag = true;
		}
		else if(!Keyboard::instance().find(ALLEGRO_KEY_ESCAPE))
			_flag = false;
		if(_show)
		{
			_fade.draw(allg_width / 2, allg_height / 2);
			_menu.update();
			_quit.update();
			if(_menu)
			{
				scene() = new MainMenu();
			}
			else if(_quit)
			{
				Error::instance().error() = Error::err::allg_close;
			}
		}
		if((!(&Player::player1()||&Player::player2()))&&!_quit)
		{
			Map::instance().destroy();
			scene() = new GameOver();
		}
		else if((_phase == 2)&&(!&Boss::instance()))
		{
			if(!_active_portal)
			{
				if(&Player::player1()) _portal.pos(Player::player1().collision().x() + 100, Player::player1().collision().y());
				else if(&Player::player2()) _portal.pos(Player::player2().collision().x() + 100, Player::player2().collision().y());
			}
			_active_portal = true;
			if(_portal)
			{
				static int score2;
				score2 = 0;
				if(&Player::player2()) score2 = Player::player2().score();
				scene() = new YouWin(Player::player1().score(), score2);
			}
		}
		if(_phase != 2)
			if(_timer.updated())
			{
				if(!_active_portal)
				{
					if(&Player::player1()) _portal.pos(Player::player1().collision().x() + 100, Player::player1().collision().y());
					else if(&Player::player2()) _portal.pos(Player::player2().collision().x() + 100, Player::player2().collision().y());
				}
				_active_portal = true;
				if(_portal)
				{
					scene() = new BuildingMap(static_cast<bool>(&Player::player1()),
											  static_cast<bool>(&Player::player2()),
											  _phase + 1,
											  (&Player::player1())?Player::player1().score():0,
											  (&Player::player2())?Player::player2().score():0);
				}
			}
	}

//											YouWin

YouWin::YouWin(int p1score, int p2score)
{
	_menu.pos(allg_width / 2, 4 * allg_height / 6);
	_menu.text("Back To Menu");
	_background.load(sky_background);
	_background.distance(-1);
	_win.fontSize(40);
	
	string str = "You Win !!!";
	str += "\nPlayer 1 Score : ";
	str += to_string(p1score);
	str += "\nPlayer 2 Score : ";
	str += to_string(p2score);
	_win.text(str);
}
YouWin::~YouWin()
{

}

void YouWin::update()
{
	_background.draw();
	_win.draw(TextBox::Flag::up);
	_menu.update();
	if(_menu)
	{
		scene() = new MainMenu();
	}
}

//											GameOver

	GameOver::GameOver()
	{
		_retry.pos(allg_width / 2, 4 * allg_height / 10);
		_quit.pos(allg_width / 2, 6 * allg_height / 10);
		_retry.text("Try Again");
		_quit.text("Quit");
		_background.load(sky_background);
		_background.distance(-1);
	}
	GameOver::~GameOver()
	{
	
	}

	void GameOver::update()
	{
		_background.draw();
		Font::instance().colorize(255,0,0);
		Font::instance().writeText(40, allg_width / 2, 1 * allg_height / 6, "You Lose");
		Font::instance().colorize(255,255,255);
		_retry.update();
		_quit.update();
		if(_retry)
		{
			scene() = new MainMenu();
		}
		else if(_quit)
		{
			Error::instance().error() = Error::err::allg_close;
		}
	}

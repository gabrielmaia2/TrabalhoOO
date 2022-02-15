#include "graphics.h"
#include "display.h"
#include "input.h"
#include <cmath>

using namespace std;

//									Image

	Image::Image()
			: _img(nullptr),
			  _scale(1, 1),
			  _angle(0)
	{
		colorize(255,255,255);
	}
	Image::~Image()
	{
		unload();
	}

	Image::operator bool() const
	{
		return static_cast<bool>(_img);
	}

	void Image::load(string path)
	{
		if(path.size())
		{
			al_destroy_bitmap(_img);
			_img = al_load_bitmap(path.c_str());
			if(!_img)
				Error::instance().error() = Error::err::allg_load_img;
		}
	}
	void Image::unload()
	{
		al_destroy_bitmap(_img);
		_img = nullptr;
	}

	Point &Image::scale()
	{
		return _scale;
	}
	const Point &Image::scale() const
	{
		return _scale;
	}

	const Point &Image::size() const
	{
		static Point p;
		return p(al_get_bitmap_width(_img), al_get_bitmap_height(_img));
	}

	void Image::angle(double a)
	{
		_angle = a;
	}
	const double &Image::angle() const
	{
		return _angle;
	}

	void Image::colorize(unsigned char red, unsigned char green, unsigned char blue, unsigned char alpha)
	{
		_color.r = red / 255.0f;
		_color.g = green / 255.0f;
		_color.b = blue / 255.0f;
		_color.a = alpha / 255.0f;
	}

	void Image::draw(int posx, int posy, int sposx, int sposy, int slimx, int slimy)
	{
		if(GameTimer::instance().fpsLimiter()&&(_img)&&
		   ((posx + slimx * abs(_scale.x()) > 0 && posx - slimx * abs(_scale.x()) < allg_width)
			&&(posy + slimy * abs(_scale.y()) > 0 && posy - slimy * abs(_scale.y()) < allg_height)))
			al_draw_tinted_scaled_rotated_bitmap_region(
					_img,
					static_cast<float>(sposx),
					static_cast<float>(sposy),
					static_cast<float>(slimx),
					static_cast<float>(slimy),
					_color,
					static_cast<float>(slimx / 2),
					static_cast<float>(slimy / 2),
					static_cast<float>(posx),
					static_cast<float>(posy),
					static_cast<float>(_scale.x()),
					static_cast<float>(_scale.y()),
					static_cast<float>(_angle), 0);
	}
	void Image::draw(int posx, int posy, int sposx, int sposy)
	{
		if(_img) draw(posx, posy, sposx, sposy, static_cast<int>(size().x()), static_cast<int>(size().y()));
	}
	void Image::unsafeDraw(int posx, int posy, int sposx, int sposy, int slimx, int slimy)
	{
		if(_img&&GameTimer::instance().fpsLimiter())
			al_draw_tinted_scaled_rotated_bitmap_region(
					_img,
					static_cast<float>(sposx),
					static_cast<float>(sposy),
					static_cast<float>(slimx),
					static_cast<float>(slimy),
					_color,
					static_cast<float>(slimx / 2),
					static_cast<float>(slimy / 2),
					static_cast<float>(posx),
					static_cast<float>(posy),
					static_cast<float>(_scale.x()),
					static_cast<float>(_scale.y()),
					static_cast<float>(_angle), 0);
	}
	void Image::unsafeDraw(int posx, int posy, int sposx, int sposy)
	{
		if(_img) unsafeDraw(posx, posy, sposx, sposy, static_cast<int>(size().x()), static_cast<int>(size().y()));
	}

//										BoxImg

	BoxImg::BoxImg()
			: _limits(50,20),
			  _border(al_map_rgba(255, 255, 255 , 120)),
			  _center(al_map_rgba(0, 0, 0, 120)),
			  _thick(3),
			  _radius(6)
	{
	
	}
	BoxImg::~BoxImg()
	{
	
	}

	void BoxImg::limits(int x, int y)
	{
		_limits(x,y);
	}
	const Point &BoxImg::limits() const
	{
		return _limits;
	}

	void BoxImg::thickness(int t)
	{
		_thick = t;
	}
	const int &BoxImg::thickness() const
	{
		return _thick;
	}

	void BoxImg::radius(int r)
	{
		_radius = r;
	}
	const int &BoxImg::radius() const
	{
		return _radius;
	}

	void BoxImg::colorizeBorder(unsigned char red, unsigned char green, unsigned char blue, unsigned char alpha)
	{
		_border.r = red / 255.0f;
		_border.g = green / 255.0f;
		_border.b = blue / 255.0f;
		_border.a = alpha / 255.0f;
	}
	void BoxImg::colorizeCenter(unsigned char red, unsigned char green, unsigned char blue, unsigned char alpha)
	{
		_center.r = red / 255.0f;
		_center.g = green / 255.0f;
		_center.b = blue / 255.0f;
		_center.a = alpha / 255.0f;
	}

	void BoxImg::draw(int posx, int posy)
	{
		if(GameTimer::instance().fpsLimiter())
		{
			al_draw_filled_rounded_rectangle(
					static_cast<float>((posx - _limits.x())),
					static_cast<float>((posy - _limits.y())),
					static_cast<float>((posx + _limits.x())),
					static_cast<float>((posy + _limits.y())),
					_radius, _radius,
					_center);
			al_draw_rounded_rectangle(
					static_cast<float>((posx - _limits.x())),
					static_cast<float>((posy - _limits.y())),
					static_cast<float>((posx + _limits.x())),
					static_cast<float>((posy + _limits.y())),
					_radius, _radius,
					_border, _thick);
		}
	}

//										Font

	Font::Font()
	{
		load(default_font, maximum_font_size);
		colorize(255, 255, 255);
	}
	Font::~Font()
	{
		unload();
	}

	void Font::load(string path, int max)
	{
		unload();
		for(int i = 1; i <= max; i++)
		{
			_font.push_back(al_load_font(path.c_str(), i, 0));
			if(!*_font.end()) Error::instance().error() = Error::err::allg_load_font;
		}
	}
	void Font::unload()
	{
		for(auto &i : _font)
		{
			al_destroy_font(i);
		}
		_font.clear();
	}

	const ALLEGRO_FONT *Font::operator()(unsigned char size) const
	{
		return _font[size - 1];
	}

	void Font::colorize(unsigned char red, unsigned char green, unsigned char blue, unsigned char alpha)
	{
		_color.r = red / 255.0f;
		_color.g = green / 255.0f;
		_color.b = blue / 255.0f;
		_color.a = alpha / 255.0f;
	}

	void Font::writeText(unsigned char size, int posx, int posy, string text)
	{
		if(GameTimer::instance().fpsLimiter())
			al_draw_text((*this)(size),
						 _color,
						 posx - text.size() * size / 3.5f,
						 posy - size / 1.5f,
						 0,
						 text.c_str());
	}

	Font &Font::instance()
	{
		static Font inst;
		return inst;
	}

//									Button

	Button::Button()
			: _font_size(default_font_size),
			  _is_active(false)
	{
		colorizeIntensitive(0,5,40,200);
		colorizeHover(0,80,250,200);
		colorizeActive(20,40,80,200);
		colorizeStandard(0,40,130,200);
		colorizeBorder(0,0,40,200);
		limits(160, 40);
	}
	Button::~Button()
	{
	
	}

	Button::operator bool()
	{
		return _is_active;
	}

	void Button::colorizeIntensitive(unsigned char red, unsigned char green, unsigned char blue, unsigned char alpha)
	{
		_intensitive.colorizeCenter(red, green, blue, alpha);
	}
	void Button::colorizeHover(unsigned char red, unsigned char green, unsigned char blue, unsigned char alpha)
	{
		_hover.colorizeCenter(red, green, blue, alpha);
	}
	void Button::colorizeActive(unsigned char red, unsigned char green, unsigned char blue, unsigned char alpha)
	{
		_active.colorizeCenter(red, green, blue, alpha);
	}
	void Button::colorizeStandard(unsigned char red, unsigned char green, unsigned char blue, unsigned char alpha)
	{
		_standard.colorizeCenter(red, green, blue, alpha);
	}
	void Button::colorizeBorder(unsigned char red, unsigned char green, unsigned char blue, unsigned char alpha)
	{
		_intensitive.colorizeBorder(red, green, blue, alpha);
		_hover.colorizeBorder(red, green, blue, alpha);
		_active.colorizeBorder(red, green, blue, alpha);
		_standard.colorizeBorder(red, green, blue, alpha);
	}

	void Button::radius(int r)
	{
		_intensitive.radius(r);
		_hover.radius(r);
		_active.radius(r);
		_standard.radius(r);
	}
	void Button::thickness(int t)
	{
		_intensitive.thickness(t);
		_hover.thickness(t);
		_active.thickness(t);
		_standard.thickness(t);
	}
	void Button::limits(int x, int y)
	{
		_intensitive.limits(x,y);
		_hover.limits(x,y);
		_active.limits(x,y);
		_standard.limits(x,y);
	}

	void Button::pos( int x, int y)
	{
		_pos(x,y);
	}
	const Point &Button::pos() const
	{
		return _pos;
	}

	void Button::text(string s)
	{
		_text = s;
	}
	const string &Button::text() const
	{
		return _text;
	}

	void Button::switchState()
	{
		_is_active = !_is_active;
	}
	const bool &Button::active() const
	{
		return _is_active;
	}

	void Button::fontSize(unsigned char f)
	{
		_font_size = f;
	}
	const unsigned char &Button::fontSize() const
	{
		return _font_size;
	}

	void Button::update()
	{
		static bool _up = true;
			if(Point(Mouse::instance().x(), Mouse::instance().y()).inRange(_pos, _hover.limits()))
			{
				if(Mouse::instance().left()&&_up)
				{
					switchState();
					_up = false;
				}
				else if(!Mouse::instance().left())
				{
					_up = true;
				}
				if(Mouse::instance().left())
				{
					_current = &_intensitive;
				}
				else
				{
					_current = &_hover;
				}
			}
			else if(_is_active)
			{
				_current = &_active;
			}
			else
			{
				_current = &_standard;
			}
			
			if(_current)
			{
				_current->draw(static_cast<int>(_pos.x()), static_cast<int>(_pos.y()));
				Font::instance().writeText(_font_size, static_cast<int>(_pos.x()), static_cast<int>(_pos.y()), _text);
			}
	}

//									TextBox

	TextBox::TextBox()
			: _fsize(default_font_size)
	{
		colorizeCenter(0, 0, 0, 120);
		colorizeBorder(255, 255, 255 , 120);
	}
	TextBox::~TextBox()
	{
	
	}

	void TextBox::fontSize(unsigned char s)
	{
		_fsize = s;
	}
	const unsigned char &TextBox::fontSize() const
	{
		return _fsize;
	}

	void TextBox::thickness(unsigned int t)
	{
		_box.thickness(t);
	}
	void TextBox::radius(unsigned int r)
	{
		_box.radius(r);
	}

	void TextBox::text(string txt)
	{
		if(_fsize)
		{
			_text.clear();
			static unsigned long i, i2;
			i = 0;
			i2 = 0;
			for (auto &c : txt)
			{
				if(c == '\n')
				{
					if(!_text.size())
					{
						_text.push_back(txt.substr(0, i2));
					}
					else if(i2 > 1)
					{
						_text.push_back(txt.substr(i - i2 + 1, i2 - 1));
					}
					i2 = 0;
				}
				i++;
				i2++;
			}
			if(i2 > 1)
			{
				if(i2 == i)
					_text.push_back(txt);
				else
					_text.push_back(txt.substr(i - i2 + 1, i2 - 1));
			}
			i = 0;
			for (auto &str : _text)
				if (str.size() > i)
					i = str.size();
			i = static_cast<unsigned long>(i * _fsize * 3.6);
			_box.limits(static_cast<unsigned int>((i + (_box.thickness() + _fsize / 2)) / (11)),
						static_cast<int>((3 * _text.size() * _fsize / 4 + (_box.thickness() + _fsize / 2))));
		}
	}

	void TextBox::colorizeCenter(unsigned char red, unsigned char green, unsigned char blue, unsigned char alpha)
	{
		_box.colorizeCenter(red, green, blue, alpha);
	}
	void TextBox::colorizeBorder(unsigned char red, unsigned char green, unsigned char blue, unsigned char alpha)
	{
		_box.colorizeBorder(red, green, blue, alpha);
	}

	void TextBox::draw(TextBox::Flag flags)
	{
		if(_text.size())
		{
			static int posx;
			static int posy;
			static unsigned long i;
			i = 0;
			for (auto &str : _text)
				if (str.size() > i)
					i = str.size();
			switch(flags)
			{
				case Flag::up :
					posx = allg_width / 2;
					posy = static_cast<int>((_fsize + _fsize + _text.size() * _fsize * 3 / 4));
					break;
				case Flag::down :
					posx = allg_width / 2;
					posy = allg_height - static_cast<int>((_fsize + _fsize + _text.size() * _fsize * 3 / 4));
					break;
				case Flag::left :
					posx = static_cast<int>((_fsize + i * _fsize / 3.2));
					posy = allg_height / 2;
					break;
				case Flag::right :
					posx = allg_width - static_cast<int>((_fsize + i * _fsize / 3.2));
					posy = allg_height / 2;
					break;
				case Flag::up_left :
					posx = static_cast<int>((_fsize + i * _fsize / 3.2));
					posy = static_cast<int>((_fsize + _fsize + _text.size() * _fsize * 3 / 4));
					break;
				case Flag::down_left :
					posx = static_cast<int>((_fsize + i * _fsize / 3.2));
					posy = allg_height - static_cast<int>((_fsize + _fsize + _text.size() * _fsize * 3 / 4));
					break;
				case Flag::up_right :
					posx = allg_width - static_cast<int>((_fsize + i * _fsize / 3.2));
					posy = static_cast<int>((_fsize + _fsize + _text.size() * _fsize * 3 / 4));
					break;
				case Flag::down_right :
					posx = allg_width - static_cast<int>((_fsize + i * _fsize / 3.2));
					posy = allg_height - static_cast<int>((_fsize + _fsize + _text.size() * _fsize * 3 / 4));
					break;
				case Flag::center :
					posx = allg_width / 2;
					posy = allg_height / 2;
					break;
				case Flag::cursor :
					posx = Mouse::instance().x() + static_cast<int>((Mouse::instance().size().x() / 2 + _fsize + i * _fsize / 3.2));
					posy = Mouse::instance().y() + static_cast<int>((Mouse::instance().size().y() / 2 + _fsize + _fsize + _text.size() * _fsize * 3 / 4));
					break;
				default:
					posx = allg_width / 2;
					posy = allg_height / 2;
					break;
			}
			_box.draw(posx, posy);
			posy -= static_cast<int>(3 * (_text.size() - 1) * _fsize / 4);
			for(auto &str : _text)
			{
				Font::instance().writeText(_fsize, posx, posy, str);
				posy += 3 * _fsize / 2;
			}
		}
	}
	void TextBox::draw(int posx, int posy)
	{
		if(_text.size())
		{
			_box.draw(posx, posy);
			posy -= static_cast<int>(3 * (_text.size() - 1) * _fsize / 4);
			for(auto &str : _text)
			{
				Font::instance().writeText(_fsize, posx, posy, str);
				posy += 3 * _fsize / 2;
			}
		}
	}

//								Animation

	Animation::Animation(double i)
			: _divisions(0,0),
			  _timer(i),
			  _anim(0),
			  _position(0),
			  _last_anim(0),
			  _last_time(i),
			  _end(false)
	{
	
	}
	Animation::~Animation()
	{
	
	}

	void Animation::load(string path, double inter, unsigned int x, unsigned int y)
	{
		_img.load(path);
		_divisions(x,y);
		_timer = inter;
	}
	void Animation::unload()
	{
		_img.unload();
		_divisions(0,0);
		_timer = 0;
	}

	Point Animation::size()
	{
		static Point p;
		p = _img.size();
		p /= _divisions;
		return p;
	}
	const Point &Animation::divisions()
	{
		return _divisions;
	}

	Point &Animation::scale()
	{
		return _img.scale();
	}
	Point Animation::scale() const
	{
		return _img.scale();
	}

	void Animation::anim(unsigned int a, double inter)
	{
		if(a != _anim)
		{
			_last_anim = _anim;
			_last_time = _timer.interval();
			if(inter)
				_timer = inter;
			_anim = a;
			_position = 0;
		}
	}
	unsigned int Animation::anim()
	{
		return _anim;
	}

	void Animation::colorize(unsigned char red, unsigned char green, unsigned char blue, unsigned char alpha)
	{
		_img.colorize(red, green ,blue, alpha);
	}

	void Animation::angle(double a)
	{
		_img.angle(a);
	}
	const double &Animation::angle() const
	{
		return _img.angle();
	}

	Timer &Animation::timer()
	{
		return _timer;
	}
	const Timer &Animation::timer() const
	{
		return _timer;
	}
	void Animation::last()
	{
		_timer = _last_time;
		_anim = _last_anim;
	}

	void Animation::draw(int px, int py)
	{
		if(_position >= _divisions.x())
		{
			_position = 0;
			_end = true;
		}
		else _end = false;
		
		if((_img)&&(_anim < _divisions.y()))
		{
			_img.draw(px, py,
					  static_cast<int>(_position * size().x()),
					  static_cast<int>(_anim * size().y()),
					  static_cast<int>(size().x()),
					  static_cast<int>(size().y()));
		}
		
		if(_timer())
			_position++;
	}

	const bool &Animation::end()
	{
		return _end;
	}

	void Animation::reset()
	{
		_position = 0;
		_last_time = 0.0;
		_last_anim = 0;
	}

//											Background

	Background::Background()
			: _pos(allg_width / 2),
			  _distance(0),
			  _dx(0),
			  _last(0)
	{
	
	}
	Background::~Background()
	{
	
	}

	void Background::load(string path)
	{
		_img.load(path);
		_img.scale()(allg_width / _img.size().x(), allg_height / _img.size().y());
	}
	void Background::unload()
	{
		_img.unload();
	}

	void Background::colorize(unsigned char red, unsigned char green, unsigned char blue, unsigned char alpha)
	{
		_img.colorize(red, green ,blue, alpha);
	}

	void Background::draw()
	{
		if(_distance >= 0)
		{
			_dx = _last - Player::camPos().x();
			_last = Player::camPos().x();
			_pos += _dx / exp(_distance / 10);
		}
		if(_pos < 0) _pos = allg_width;
		if(_pos > allg_width) _pos = 0;
		_img.unsafeDraw(static_cast<int>(_pos + allg_width), allg_height / 2);
		_img.unsafeDraw(static_cast<int>(_pos), allg_height / 2);
		_img.unsafeDraw(static_cast<int>(_pos - allg_width), allg_height / 2);
	}

	void Background::distance(double d)
	{
		_distance = d;
	}

//											ValueBar

	ValueBar::ValueBar(double d)
			: _limits(15, 2),
			  _max(d),
			  _value(d)
	{
		colorizeBorder(0, 0, 0);
		colorizeBackground(40, 40, 40, 180);
		colorizeCenter(200, 20, 20, 200);
	}
	ValueBar::~ValueBar()
	{
	
	}

	const double &ValueBar::value() const
	{
		return _value;
	}
	double &ValueBar::value()
	{
		return _value;
	}
	const double &ValueBar::max() const
	{
		return _max;
	}
	void ValueBar::max(double m)
	{
		_max = m;
	}

	const Point &ValueBar::limits() const
	{
		return _limits;
	}
	void ValueBar::limits(double x, double y)
	{
		_limits(x, y);
	}

	void ValueBar::colorizeBorder(unsigned char red, unsigned char green, unsigned char blue, unsigned char alpha)
	{
		_border.r = red / 255.0f;
		_border.g = green / 255.0f;
		_border.b = blue / 255.0f;
		_border.a = alpha / 255.0f;
	}
	void ValueBar::colorizeCenter(unsigned char red, unsigned char green, unsigned char blue, unsigned char alpha)
	{
		_center.r = red / 255.0f;
		_center.g = green / 255.0f;
		_center.b = blue / 255.0f;
		_center.a = alpha / 255.0f;
	}
	void ValueBar::colorizeBackground(unsigned char red, unsigned char green, unsigned char blue, unsigned char alpha)
	{
		_back.r = red / 255.0f;
		_back.g = green / 255.0f;
		_back.b = blue / 255.0f;
		_back.a = alpha / 255.0f;
	}

	void ValueBar::draw(int posx, int posy)
	{
		if(GameTimer::instance().fpsLimiter()&&(_value < _max)&&(_value > 0)&&
		   ((posx + _limits.x() > 0 && posx - _limits.x() < allg_width)
			&&(posy + _limits.y() > 0 && posy - _limits.y() < allg_height)))
		{
			al_draw_filled_rectangle(
					static_cast<float>(posx - _limits.x()),
					static_cast<float>(posy - _limits.y()),
					static_cast<float>(posx + _limits.x()),
					static_cast<float>(posy + _limits.y()),
					_back);
			al_draw_filled_rectangle(
					static_cast<float>(posx - _limits.x()),
					static_cast<float>(posy - _limits.y()),
					static_cast<float>(posx + (2 * _limits.x() * _value / _max) - _limits.x()),
					static_cast<float>(posy + _limits.y()),
					_center);
			al_draw_rectangle(
					static_cast<float>(posx - _limits.x()),
					static_cast<float>(posy - _limits.y()),
					static_cast<float>(posx + _limits.x()),
					static_cast<float>(posy + _limits.y()),
					_border, 2);
		}
	}

//											LinearLight

	LinearLight::LinearLight()
	{
		_light.load("../files/textures/sprites/linearlight.png");
	}
	LinearLight::~LinearLight()
	{
	
	}

	void LinearLight::range(double r)
	{
		_light.scale().y() = r / _light.size().y();
	}
	const double &LinearLight::range() const
	{
		static double d;
		d = _light.scale().y() * _light.size().y();
		return d;
	}
	void LinearLight::distance(double r)
	{
		_light.scale().x() = r / _light.size().x();
	}
	const double &LinearLight::distance() const
	{
		static double d;
		d = _light.scale().x() * _light.size().x();
		return d;
	}
	void LinearLight::angle(double a)
	{
		_light.angle(a);
	}
	const double &LinearLight::angle() const
	{
		return _light.angle();
	}

	void LinearLight::colorize(unsigned char r, unsigned char g, unsigned char b, unsigned char a)
	{
		_light.colorize(r,g,b,a);
	}

	void LinearLight::draw(int x, int y)
	{
		_light.draw(static_cast<int>(x + distance() * cos(angle()) / 2),
					static_cast<int>(y + distance() * sin(angle()) / 2));
	}

//											RoundLight

	RoundLight::RoundLight()
	{
		_light.load("../files/textures/sprites/roundlight.png");
	}
	RoundLight::~RoundLight()
	{
	
	}

	void RoundLight::radius(double r)
	{
		_light.scale().x() = 2 * r / _light.size().x();
		_light.scale().y() = 2 * r / _light.size().y();
	}
	const double &RoundLight::radius() const
	{
		static double d;
		d = _light.scale().x() * _light.size().x() / 2;
		return d;
	}
	void RoundLight::angle(double a)
	{
		_light.angle(a);
	}
	const double &RoundLight::angle() const
	{
		return _light.angle();
	}

	void RoundLight::colorize(unsigned char r, unsigned char g, unsigned char b, unsigned char a)
	{
		_light.colorize(r,g,b,a);
	}

	void RoundLight::draw(int x, int y)
	{
		_light.draw(x, y);
	}

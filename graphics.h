#ifndef GRAPHICS_H
#define GRAPHICS_H

#include <allegro5/allegro.h>
#include <allegro5/allegro_color.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_memfile.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_native_dialog.h>
#include <string>

#include "container.h"
#include "geometry.h"
#include "time.h"
#include "vars.h"

using namespace std;

/// Is an image
class Image final
{
private:
	ALLEGRO_BITMAP *_img;
	
	ALLEGRO_COLOR _color;
	
	Point _scale;
	double _angle;
public:
	Image();
	~Image();
	
	/// @return if img is loaded (is not nullptr).
	operator bool() const;
	
	/// Loads the image.
	/// @param path is the path of the image file.
	/// @warning Needs to be called before calling draw.
	void load(string path);
	/// Destroy the image.
	void unload();
	
	/// Sets the scale point for the box starting in center.
	/// @default scale is (1,1)
	Point &scale();
	/// Gets the scale point for the box starting in center.
	/// @default scale is (1,1)
	const Point &scale() const;
	
	/// Gets the size of the image.
	const Point & size() const;
	
	/// Sets the rotation angle for the image.
	/// @default angle is 0.
	void angle(double a);
	/// Gets the rotation angle of the image.
	/// @default angle is 0.
	const double &angle() const;
	
	/// Sets the image color.
	/// @default color is (255,255,255,255).
	void colorize(unsigned char red, unsigned char green, unsigned char blue, unsigned char alpha = 255);
	
	/// Draws the image.
	/// @param pos is position in screen (in pixels).
	/// @param spos is start position in image.
	/// @param slim is limit position in image.
	/// @default limit is lower right edge of the image.
	/// @default start is upper left edge of the image.
	void draw(int posx, int posy, int sposx, int sposy, int slimx, int slimy);
	/// Draws the image.
	/// @param pos is position in screen (in pixels).
	/// @param spos is start position in image.
	/// @default limit is lower right edge of the image.
	/// @default start is upper left edge of the image.
	void draw(int posx, int posy, int sposx = 0, int sposy = 0);
	
	void unsafeDraw(int posx, int posy, int sposx, int sposy, int slimx, int slimy);
	void unsafeDraw(int posx, int posy, int sposx = 0, int sposy = 0);
};

/// Create a new rounded box.
class BoxImg final
{
private:
	Point _limits;
	
	ALLEGRO_COLOR _border;
	ALLEGRO_COLOR _center;
	
	int _thick;
	int _radius;
public:
	BoxImg();
	~BoxImg();
	
	/// Sets the limit point for the box starting in center.
	/// @endcode The Box will start in (center - limits) and will end in (center + limits).
	/// @warning Needs to be positive.
	/// @warning Needs to be called before calling draw.
	void limits(int x, int y);
	/// Gets the limit point for the box starting in center.
	/// @endcode The Box will start in (center - limits) and will end in (center + limits).
	const Point &limits() const;
	
	/// Sets the border thickness.
	/// @default thickness is 3.
	void thickness(int t);
	/// Gets the border thickness.
	/// @default thickness is 3.
	const int &thickness() const;
	
	/// Sets the border radius.
	/// @default radius is 6.
	void radius(int r);
	/// Gets the border radius.
	/// @default radius is 6.
	const int &radius() const;
	
	/// Sets the border color.
	/// @default color is (0,0,0,255).
	void colorizeBorder(unsigned char red, unsigned char green, unsigned char blue, unsigned char alpha = 255);
	/// Sets the center color.
	/// @default color is (255,255,255,255).
	void colorizeCenter(unsigned char red, unsigned char green, unsigned char blue, unsigned char alpha = 255);
	
	/// Draws the box.
	/// @param posx is center position in x (in pixels).
	/// @param posy is center position in y (in pixels).
	/// @endcode The Point(0,0) is the upper left edge.
	void draw(int posx, int posy);
};

class Font final
{
private:
	Container<ALLEGRO_FONT *> _font;
	
	ALLEGRO_COLOR _color;
	
	Font();
	
	Font(const Font &) = delete;
	Font(Font &&) = delete;
	
	void operator=(const Font &) = delete;
	void operator=(Font &&) = delete;
public:
	~Font();
	
	/// Loads the font.
	/// @param path is the path of the font file.
	/// @param min is the minimum font size.
	/// @param max is the maximum font size.
	/// @warning Needs to be called before calling write.
	void load(string path = default_font, int max = maximum_font_size);
	/// Destroy the font.
	void unload();
	
	/// Gets an allegro font.
	/// @param size is the font size.
	/// @return the allegro font.
	const ALLEGRO_FONT *operator()(unsigned char size) const;
	
	/// Sets the image color.
	/// @default color is (255,255,255,255).
	void colorize(unsigned char red, unsigned char green, unsigned char blue, unsigned char alpha = 255);
	
	/// Writes a text with this font.
	/// @param size is the font size.
	/// @param posx is center position in x (in pixels).
	/// @param posy is center position in y (in pixels).
	/// @param text is the text to be written.
	/// @endcode The Point(0,0) is the upper left edge.
	void writeText(unsigned char size, int posx, int posy, string text);
	
	/// Gets singleton instance
	static Font &instance();
};

class Mouse;

class Button final
{
private:
	BoxImg _intensitive;
	BoxImg _standard;
	BoxImg _active;
	BoxImg _hover;
	
	Point _pos;
	
	string _text;
	
	BoxImg *_current;
	
	unsigned char _font_size;
	
	bool _is_active;
public:
	Button();
	~Button();
	
	/// @return button state.
	operator bool();
	
	/// Sets the intensitive color.
	/// @default color is (255,255,255,255).
	void colorizeIntensitive(unsigned char red, unsigned char green, unsigned char blue, unsigned char alpha = 255);
	/// Sets the hover color.
	/// @default color is (255,255,255,255).
	void colorizeHover(unsigned char red, unsigned char green, unsigned char blue, unsigned char alpha = 255);
	/// Sets the active color.
	/// @default color is (0,0,0,255).
	void colorizeActive(unsigned char red, unsigned char green, unsigned char blue, unsigned char alpha = 255);
	/// Sets the standard color.
	/// @default color is (127,127,127,255).
	void colorizeStandard(unsigned char red, unsigned char green, unsigned char blue, unsigned char alpha = 255);
	/// Sets the border color.
	/// @default color is (255,255,255,255).
	void colorizeBorder(unsigned char red, unsigned char green, unsigned char blue, unsigned char alpha = 255);
	
	/// Sets the border radius.
	/// @default radius is 6.
	void radius(int r);
	/// Sets the border radius.
	/// @default radius is 3.
	void thickness(int t);
	/// Sets the limit point for the button starting in center.
	/// @endcode The button will start in (center - limits) and will end in (center + limits).
	/// @warning Needs to be positive.
	/// @default limit is (80,30)
	void limits(int x, int y);
	
	/// Sets the center position.
	/// @warning Needs to be called before calling update.
	void pos(int x, int y);
	/// Gets the center position.
	const Point &pos() const;
	
	/// Sets the button text.
	/// @warning Needs to be called before calling update.
	void text(string s);
	/// Gets the button text.
	const string &text() const;
	
	/// Swaps the button state.
	void switchState();
	/// @return button state.
	const bool &active() const;
	
	/// Sets the font size.
	/// @default size is allg_min_font_size.
	void fontSize(unsigned char f);
	const unsigned char &fontSize() const;
	
	/// Updates the button.
	void update();
};

class TextBox
{
public:
	enum class Flag
	{
		none = 0,
		up,
		down,
		left,
		right,
		up_left,
		up_right,
		down_left,
		down_right,
		center,
		cursor,
	};
private:
	BoxImg _box;
	
	Container<string> _text;
	
	unsigned char _fsize;
public:
	TextBox();
	~TextBox();
	
	void fontSize(unsigned char s);
	const unsigned char &fontSize() const;
	
	void text(string txt);
	
	void colorizeCenter(unsigned char red, unsigned char green, unsigned char blue, unsigned char alpha = 255);
	void colorizeBorder(unsigned char red, unsigned char green, unsigned char blue, unsigned char alpha = 255);
	
	void draw(Flag flags);
	void draw(int posx, int posy);
	
	void thickness(unsigned int t);
	
	void radius(unsigned int r);
};

class Animation final
{
private:
	Image _img;
	Point _divisions;
	
	Timer _timer;
	
	unsigned int _anim; // current anim
	unsigned int _position; // the current sprite pos in x (resets every time the anim changes)
	
	unsigned int _last_anim;
	
	double _last_time;
	
	bool _end;
public:
	Animation(double i = 0.1);
	~Animation();
	
	void load(string path, double inter, unsigned int x, unsigned int y);
	void unload();
	
	Point size();
	const Point &divisions();
	
	Point &scale();
	Point scale() const;
	
	void anim(unsigned int a, double inter = 0.0);
	unsigned int anim();
	
	void colorize(unsigned char red, unsigned char green, unsigned char blue, unsigned char alpha = 255);
	
	/// Sets the rotation angle for the image.
	/// @default angle is 0.
	void angle(double a);
	/// Gets the rotation angle of the image.
	/// @default angle is 0.
	const double &angle() const;
	
	Timer &timer();
	const Timer &timer() const;
	void last();
	
	// draws animation (0 is in the top of the sprite) in pixel (px,py)
	void draw(int px, int py);
	
	const bool &end();
	
	void reset();
};

class Background final
{
private:
	Image _img;
	double _pos;
	double _distance;
	
	double _dx;
	double _last;
public:
	Background();
	~Background();
	
	void load(string path);
	void unload();
	
	void colorize(unsigned char red, unsigned char green, unsigned char blue, unsigned char alpha);
	
	void draw();
	
	void distance(double d);
};

class ValueBar
{
private:
	Point _limits;
	
	double _max;
	double _value;
	
	ALLEGRO_COLOR _border;
	ALLEGRO_COLOR _center;
	ALLEGRO_COLOR _back;
public:
	ValueBar(double d = 0);
	~ValueBar();
	
	const double &value() const;
	double &value();
	const double &max() const;
	void max(double m);
	
	const Point &limits() const;
	void limits(double x, double y);
	
	void colorizeBorder(unsigned char red, unsigned char green, unsigned char blue, unsigned char alpha = 255);
	void colorizeCenter(unsigned char red, unsigned char green, unsigned char blue, unsigned char alpha = 255);
	void colorizeBackground(unsigned char red, unsigned char green, unsigned char blue, unsigned char alpha = 255);
	
	void draw(int posx, int posy);
};

class LinearLight final
{
private:
	Image _light;
public:
	LinearLight();
	~LinearLight();
	
	void range(double r);
	const double &range() const;
	void distance(double r);
	const double &distance() const;
	void angle(double a);
	const double &angle() const;
	
	void colorize(unsigned char r, unsigned char g, unsigned char b, unsigned char a = 255);
	
	void draw(int x, int y);
};

class RoundLight final
{
private:
	Image _light;
public:
	RoundLight();
	~RoundLight();
	
	void radius(double r);
	const double &radius() const;
	void angle(double a);
	const double &angle() const;
	
	void colorize(unsigned char r, unsigned char g, unsigned char b, unsigned char a = 255);
	
	void draw(int x, int y);
};

#endif

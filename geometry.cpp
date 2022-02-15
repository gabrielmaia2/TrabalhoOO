#include "geometry.h"
#include "vars.h"
#include <cmath>

using namespace std;

//							Point

	Point::Point(double X, double Y)
			: _x(X),
			  _y(Y)
	{
	
	}
	Point::Point(const Point &p)
			: _x(p._x),
			  _y(p._y)
	{
	
	}
	Point::Point(Point &&p)
			: _x(p._x),
			  _y(p._y)
	{
		p._x = 0;
		p._y = 0;
	}
	Point::~Point()
	{
	
	}

// gets sets

	double &Point::x()
	{
		return _x;
	}
	double &Point::y()
	{
		return _y;
	}
	const double &Point::x() const
	{
		return _x;
	}
	const double &Point::y() const
	{
		return _y;
	}

// calc

	void Point::rotate(double a, Point p)
	{
		double x2;
		x2 = cos(a) * (_x - p._x) - sin(a) * (_y - p._y);
		_y += sin(a) * (_x - p._x) + cos(a) * (_y - p._y);
		_x += x2;
	}
	void Point::scale(Point s, Point p)
	{
		*this += s.prod(*this - p);
	}
	const double &Point::distance(Point p) const
	{
		static double d;
		d = pow((*this - p) * (*this - p), 0.5);
		return d;
	}
	const double &Point::module() const
	{
		static double d;
		d = distance();
		return d;
	}
	const Point &Point::proj(Point p) const
	{
		static Point p2;
		p2 = (p.module())?(p * ((*this * p) / (p * p))):(0.0);
		return p2;
	}
	const double &Point::operator*(Point p) const
	{
		static double d;
		d = _x * p._x + _y * p._y;
		return d;
	}
	const double &Point::angle(Point p) const
	{
		static double d;
		d = (module() && p.module())?(acos((*this * p)/(module() * p.module()))):(0.0);
		return d;
	}
	const bool &Point::inRange(Point c, Point lim)
	{
		static bool b;
		b = (lim > Point(0,0))?(((*this) > c - lim)&&((*this) < c + lim)):false;
		return b;
	}

// operators

	Point &Point::operator()(double X, double Y)
	{
		_x = X;
		_y = Y;
		return *this;
	}
	Point &Point::operator=(const Point &p)
	{
		_x = p._x;
		_y = p._y;
		return *this;
	}
	Point &Point::operator=(Point &&p)
	{
		_x = p._x;
		_y = p._y;
		p._x = 0;
		p._y = 0;
		return *this;
	}

	const Point &Point::operator+(Point pt) const
	{
		static Point p;
		p(_x + pt._x, _y + pt._y);
		return p;
	}
	const Point &Point::operator-(Point pt) const
	{
		static Point p;
		p(_x - pt._x, _y - pt._y);
		return p;
	}
	const Point &Point::prod(Point pt) const
	{
		static Point p;
		p(_x * pt._x, _y * pt._y);
		return p;
	}
	const Point &Point::operator/(Point pt) const
	{
		static Point p;
		if(pt._x && pt._y) p(_x / pt._x, _y / pt._y);
		else p(0.0, 0.0);
		return p;
	}

	const Point &Point::operator+(double d) const
	{
		static Point p;
		p(_x + d, _y + d);
		return p;
	}
	const Point &Point::operator-(double d) const
	{
		static Point p;
		p(_x - d, _y - d);
		return p;
	}
	const Point &Point::operator*(double d) const
	{
		static Point p;
		p(_x * d, _y * d);
		return p;
	}
	const Point &Point::operator/(double d) const
	{
		static Point p;
		if(d) p(_x / d, _y / d);
		else p(0.0, 0.0);
		return p;
	}

	Point &Point::operator+=(Point pt)
	{
		_x += pt._x;
		_y += pt._y;
		return *this;
	}
	Point &Point::operator-=(Point pt)
	{
		_x -= pt._x;
		_y -= pt._y;
		return *this;
	}
	Point &Point::operator*=(Point pt)
	{
		_x *= pt._x;
		_y *= pt._y;
		return *this;
	}
	Point &Point::operator/=(Point pt)
	{
		if(pt._x && pt._y)
		{
			_x /= pt._x;
			_y /= pt._y;
		}
		return *this;
	}

	Point &Point::operator+=(double d)
	{
		_x += d;
		_y += d;
		return *this;
	}
	Point &Point::operator-=(double d)
	{
		_x -= d;
		_y -= d;
		return *this;
	}
	Point &Point::operator*=(double d)
	{
		_x *= d;
		_y *= d;
		return *this;
	}
	Point &Point::operator/=(double d)
	{
		if(d)
		{
			_x /= d;
			_y /= d;
		}
		return *this;
	}

	const bool &Point::operator>(Point pt) const
	{
		static bool b;
		b = ((_x > pt._x)&&(_y > pt._y));
		return b;
	}
	const bool &Point::operator<(Point pt) const
	{
		static bool b;
		b = ((_x < pt._x)&&(_y < pt._y));
		return b;
	}

//								Line

	Line::Line(Point P1, Point P2)
			: Point(P2 - P1),
			  _p1(P1),
			  _p2(P2)
	{
	
	}
	Line::Line(const Line &l)
			: Point(l._p2 - l._p1),
			  _p1(l._p1),
			  _p2(l._p2)
	{
	
	}
	Line::Line(Line &&l)
			: Point(l._p2 - l._p1),
			  _p1(l._p1),
			  _p2(l._p2)
	{
		l._p1(0,0);
		l._p2(0,0);
		l(0,0);
	}
	Line::~Line()
	{
	
	}

// calc


	void Line::rotate(double a, Point p)
	{
		_p1.rotate(a,p);
		_p2.rotate(a,p);
		static_cast<Point &>(*this) = _p2 - _p1;
	}
	Point Line::normVec(Point p) const
	{
		return (_p2 - p) - (_p2 - p).proj(*this);
	}

	Line &Line::set(Point P1, Point N)
	{
		_p1 = P1;
		static_cast<Point &>(*this) = N;
		_p2 = _p1 + static_cast<Point &>(*this);
		return *this;
	}
	Point Line::nvec() const
	{
			return (module())?(static_cast<const Point &>(*this) / module()):(0.0);
	}

// operators

	Line &Line::operator()(Point P1, Point P2)
	{
		_p1 = P1;
		_p2 = P2;
		static_cast<Point &>(*this) = _p2 - _p1;
		return *this;
	}
	Line &Line::operator=(Line l)
	{
		_p1 = l._p1;
		_p2 = l._p2;
		static_cast<Point &>(*this) = static_cast<Point &>(l);
		return *this;
	}

//								BoxCol

	BoxCol::BoxCol(Point c, Point b)
			: Point(c),
			  _bounds(b)
	{
	
	}
	BoxCol::BoxCol(const BoxCol &b)
			: Point(static_cast<Point>(b)),
			  _bounds(b._bounds)
	{
	
	}
	BoxCol::BoxCol(BoxCol &&b)
			: Point(static_cast<Point>(b)),
			  _bounds(b._bounds)
	{
		b._bounds(0,0);
		b(0,0);
	}
	BoxCol::~BoxCol()
	{
	
	}

	Point &BoxCol::center()
	{
		return static_cast<Point &>(*this);
	}
	const Point &BoxCol::center() const
	{
		return static_cast<const Point &>(*this);
	}
	Point &BoxCol::bounds()
	{
		return _bounds;
	}
	const Point &BoxCol::bounds() const
	{
		return _bounds;
	}

	const BoxCol::ColType &BoxCol::onCol(const BoxCol &b) const
	{
		static ColType type;
		static Point d;
		d = (*this) - b;
		if(d.x() < 0) d.x() = -d.x();
		if(d.y() < 0) d.y() = -d.y();
		if((_bounds.x() || b._bounds.x()) && (_bounds.y() || b._bounds.y()))
		{
			if((d.x() < _bounds.x() + b._bounds.x())&&(d.y() < _bounds.y() + b._bounds.y()))
			{
				// on collision
				if(d.x() / (_bounds.x() + b._bounds.x()) < d.y() / (_bounds.y() + b._bounds.y()))
				{
					// collision in y
					if(y() < b.y())
					{
						if(d.y() < collision_border)
							type = ColType::border_up;
						else
							type = ColType::up;
					}
					else
					{
						if(d.y() < collision_border)
							type = ColType::border_down;
						else
							type = ColType::down;
					}
				}
				else
				{
					// collision in x
					if(x() < b.x())
					{
						if(d.x() < collision_border)
							type = ColType::border_left;
						else
							type = ColType::left;
					}
					else
					{
						if(d.x() < collision_border)
							type = ColType::border_right;
						else
							type = ColType::right;
					}
				}
			}
			else
			{
				type = ColType::none;
			}
		}
		else
		{
			type = ColType::none;
		}
		return type;
	}
	
	void BoxCol::rotate(double d)
	{
		bounds().rotate(d);
	}

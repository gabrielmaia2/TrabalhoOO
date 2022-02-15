#ifndef GEOMETRY_H
#define GEOMETRY_H

// TODO

class Point // can be used as point and as vector
{
protected:
	// point coordinates
	double _x;
	double _y;
public:
	Point(double X = 0.0, double Y = 0.0);
	Point(const Point &p);
	Point(Point &&p);
	virtual ~Point();
	
	// get and set for x and y
	double &x();
	double &y();
	const double &x() const;
	const double &y() const;
	
	// calc
	virtual void rotate(double a, Point p = 0.0); // rotates this point
	virtual void scale(Point s, Point p = 0.0); // changes this point scale
	virtual const double &distance(Point p = 0.0) const; // distance between two points
	virtual const double &module() const; // vec module
	virtual const Point &proj(Point p = 1.0) const; // projection of this vec in vec p
	virtual const double &operator*(Point p) const; // inner product
	virtual const double &angle(Point p = 0.0) const; // angle between two vec
	virtual const bool &inRange(Point c, Point lim); // returns true if point is in range [ c - lim, c + lim ] (lim > (0,0))
	
	// operators
	virtual Point &operator()(double X = 0.0, double Y = 0.0);
	virtual Point &operator=(const Point &p);
	virtual Point &operator=(Point &&p);
	
	virtual const Point &operator+(Point pt) const;
	virtual const Point &operator-(Point pt) const;
	virtual const Point &prod(Point pt) const; // operator* is the inner product, prod returns a point
	virtual const Point &operator/(Point pt) const;
	
	virtual const Point &operator+(double d) const;
	virtual const Point &operator-(double d) const;
	virtual const Point &operator*(double d) const;
	virtual const Point &operator/(double d) const;
	
	virtual Point &operator+=(Point pt);
	virtual Point &operator-=(Point pt);
	virtual Point &operator*=(Point pt);
	virtual Point &operator/=(Point pt);
	
	virtual Point &operator+=(double d);
	virtual Point &operator-=(double d);
	virtual Point &operator*=(double d);
	virtual Point &operator/=(double d);
	
	virtual const bool &operator>(Point pt) const; // if the x and y are greater than pt x and y
	virtual const bool &operator<(Point pt) const; // if the x and y are smaller than pt x and y
};

class Line : public Point // can be used as line and as segment
{
protected:
	// here (x,y) will be the line vector
	Point _p1; // point 1
	Point _p2; // point 2
public:
	Line(Point P1 = 0.0, Point P2 = 1.0);
	Line(const Line &l);
	Line(Line &&l);
	virtual ~Line();
	
	// calc
	void rotate(double a, Point p) override; // rotates the line
	Point normVec(Point p = 0.0) const; // normal vector
	// proj returns the projection of this line segment in l
	// module returns line segment module
	// angle returns angle between two lines
	
	Line &set(Point P1 = 0.0, Point N = 1.0); // uses normal vector
	Point nvec() const; // returns a normal versor
	
	// operators
	Line &operator()(Point P1 = 0.0, Point P2 = 0.0); // uses two points
	Line &operator=(Line l);
};

class BoxCol : public Point
{
protected:
	// here (x,y) is the box center
	Point _bounds;
public:
	enum class ColType
	{
		up,
		down,
		left,
		right,
		border_up,
		border_down,
		border_left,
		border_right,
		none
	};
	
	BoxCol(Point c = 0.0, Point b = 0.0);
	BoxCol(const BoxCol &b);
	BoxCol(BoxCol &&b);
	virtual ~BoxCol();
	
	Point &center();
	const Point &center() const;
	Point &bounds();
	const Point &bounds() const;
	
	/// returns the col position of this relative to b
	/// y-axis is oriented downward
	const ColType &onCol(const BoxCol &b) const;
	
	void rotate(double d);
};

#endif
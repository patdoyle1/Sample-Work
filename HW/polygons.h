#ifndef POLYGONS_H_INCLUDED
#define POLYGONS_H_INCLUDED


#include <iostream>
#include <string>
#include <vector>

#include "utilities.h"

class Polygon
{
public:
	//Constructor
	Polygon(const std::string& theName, const std::vector<Point>& thePoints) : name(theName), points(thePoints) {}

	//accessors
	const std::string getName() const { return name; }
	virtual const int getNumPoints() const { return points.size(); }

	const bool HasAllEqualSides() const;
	const bool HasAllEqualAngles() const;
	const bool HasARightAngle() const;

	const int numEqualSides() const;

protected:
	std::string name;
	std::vector<Point> points;
};

class Quadrilateral : public Polygon 
{
public:
	Quadrilateral(const std::string& theName, const std::vector<Point>& thePoints);
};

class Trapezoid : virtual public Quadrilateral 
{
public:
	Trapezoid(const std::string& theName, const std::vector<Point>& thePoints);
	const int numParallelPairs() const;
};

class Kite : virtual public Quadrilateral
{
public:
	Kite(const std::string& theName, const std::vector<Point>& thePoints);
	const bool hasAdjacentSides() const;
};

class IsoscelesTrapezoid : virtual public Trapezoid{
public:
	IsoscelesTrapezoid(const std::string& theName, const std::vector<Point>& thePoints);
	const bool hasAdjacentAngles() const;
};

class Parallelogram : virtual public Trapezoid
{
public:
	Parallelogram(const std::string& theName, const std::vector<Point>& thePoints);
};

class Rectangle : virtual public Parallelogram, public IsoscelesTrapezoid
{
public:
	Rectangle(const std::string& theName, const std::vector<Point>& thePoints);
};

class Rhombus : virtual public Parallelogram, public Kite
{
public:
	Rhombus(const std::string& theName, const std::vector<Point>& thePoints);
};

class Square : public Rhombus, public Rectangle
{
public:
	Square(const std::string& theName, const std::vector<Point>& thePoints) 
	: Quadrilateral(theName, thePoints), Trapezoid(theName,thePoints), Parallelogram(theName, thePoints), Rhombus(theName, thePoints), Rectangle(theName, thePoints) {}
};

class Triangle : public Polygon
{
public:
	Triangle(const std::string& theName, const std::vector<Point>& thePoints) ;
};

class IsoscelesTriangle : virtual public Triangle
{
public:
	IsoscelesTriangle(const std::string& theName, const std::vector<Point>& thePoints);
};

class RightTriangle : virtual public Triangle
{
public:
	RightTriangle(const std::string& theName, const std::vector<Point>& thePoints);
};

class EquilateralTriangle : public IsoscelesTriangle
{
public:
	EquilateralTriangle(const std::string& theName, const std::vector<Point>& thePoints);
};

class IsoscelesRightTriangle : public RightTriangle, public IsoscelesTriangle
{
public:
	IsoscelesRightTriangle(const std::string& theName, const std::vector<Point>& thePoints) : Triangle(theName, thePoints), RightTriangle(theName, thePoints), IsoscelesTriangle(theName, thePoints){}
};

#endif // POLYGONS_H_INCLUDED
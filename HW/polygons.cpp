#include <iostream>
#include <string>
#include <vector>

#include "polygons.h"

const int Polygon::numEqualSides() const{ //the max this can return from a quadrilateral is 5
	int equalSides = 0;
	std::vector<double> lengths;
	for(int h = 0; h < points.size() - 1; h++)
	{
		double thedistance = DistanceBetween(points[h],points[h+1]);
		lengths.push_back(thedistance);
	}
	lengths.push_back(DistanceBetween(points[points.size()-1], points[0]));
	for(int h = 0; h < lengths.size()-1; h++)
	{
		for(int k = h+1; k < lengths.size(); k++)
		{
			if(EqualSides(lengths[h],lengths[k]))
				equalSides++;
		}
	}
	return equalSides;
}
//boolean helper function to show if has all equal sides
const bool Polygon::HasAllEqualSides() const
{
	std::vector<double> lengths;
	for(int h = 0; h < points.size() - 1; h++)
	{
		double thedistance = DistanceBetween(points[h],points[h+1]);
		lengths.push_back(thedistance);
	}
	lengths.push_back(DistanceBetween(points[points.size()-1], points[0]));
	for(int h = 0; h < lengths.size()-1; h++)
	{
		for(int k = h+1; k < lengths.size(); k++)
		{
			if(!EqualSides(lengths[h],lengths[k]))
				return false;
		}
	}
	return true;
}
//boolean helper function to see if shape has all equal sides
const bool Polygon::HasAllEqualAngles() const
{
	std::vector<double> angles;
	for(int h = 1; h < points.size()-1; h++)
	{
		double theAngle = Angle(points[h-1], points[h], points[h+1]);
		angles.push_back(theAngle);
	}
	angles.push_back(Angle(points[points.size()-2], points[points.size()-1], points[0]));
	angles.push_back(Angle(points[points.size()-1], points[0], points[1]));
	for(int h = 1; h < angles.size(); h++)
	{
		if(!EqualAngles(angles[h-1], angles[h]))
			return false;
	}
	return true;
}
//A boolean helper function to show whether the shape has right angles
const bool Polygon::HasARightAngle() const
{
	for(int h = 1; h < points.size()-1; h++)
	{
		double theAngle = Angle(points[h-1], points[h], points[h+1]);
		if(RightAngle(theAngle))
			return true;
	}
	Point p1 = points[0];
	Point p2 = points[points.size()-1];
	Point p3 = points[points.size()-2];
	Point p4 = points[1];
	double angle1 = Angle(p2, p1, p4); //try figuring this out!!! trolololololollololol
	double angle2 = Angle(p3, p2, p1);
	if(RightAngle(angle1) || RightAngle(angle2))
		return true;
	return false;
}

Quadrilateral::Quadrilateral(const std::string& theName, const std::vector<Point>& thePoints) : Polygon(theName,thePoints)
{
	if(getNumPoints() != 4)
	{
		throw 5;
	}
}

const int Trapezoid::numParallelPairs() const
{
	int parallelCnt = 0;
	std::vector<Vector> sides;
	for(int x = 0; x < points.size()-1; x++)
	{
		Vector theVec(points[x],points[x+1]);
		sides.push_back(theVec);
	}
	sides.push_back(Vector(points[points.size()-1], points[0]));
	for(int h = 0; h < sides.size()-1; h++)
	{
		for(int k = h+1; k < points.size(); k++)
		{
			if(Parallel(sides[h], sides[k]))
				parallelCnt++;
		}
	}
	return parallelCnt;
}

Trapezoid::Trapezoid(const std::string& theName, const std::vector<Point>& thePoints) : Quadrilateral(theName,thePoints)
{
	if(numParallelPairs() < 1)
	{
		throw 5;
	}
}

const bool Kite::hasAdjacentSides() const
{
	std::vector<Vector> sides;
	for(int h = 0; h < points.size()-1; h++)
	{
		Vector theVector = Vector(points[h], points[h+1]);
		sides.push_back(theVector);
	}
	sides.push_back(Vector(points[points.size()-1], points[0]));
	for(int h = 1; h < sides.size(); h++)
	{
		if(EqualSides(sides[h-1].Length(), sides[h].Length()))
			return true;
	}
	return false;
}

Kite::Kite(const std::string& theName, const std::vector<Point>& thePoints) : Quadrilateral(theName,thePoints)
{
	if(numEqualSides() < 1 || !hasAdjacentSides())
	{
		throw 5;
	}
}
//boolean helper to show if adjacent angels ==
const bool IsoscelesTrapezoid::hasAdjacentAngles() const
{
	std::vector<double> angles;
	for(int h = 1; h < points.size()-1; h++)
	{
		double theAngle = Angle(points[h-1], points[h], points[h+1]);
		angles.push_back(theAngle);
	}
	angles.push_back(Angle(points[points.size()-2], points[points.size()-1], points[0]));
	angles.push_back(Angle(points[points.size()-1], points[0], points[1]));
	for(int h = 1; h < angles.size(); h++)
	{
		if(EqualAngles(angles[h-1], angles[h]))
			return true;
	}
	return false;
}

IsoscelesTrapezoid::IsoscelesTrapezoid(const std::string& theName, const std::vector<Point>& thePoints) : Quadrilateral(theName,thePoints), Trapezoid(theName,thePoints)
{
	if(numEqualSides() < 1 || !hasAdjacentAngles()){
		throw 5;
	}
}

Parallelogram::Parallelogram(const std::string& theName, const std::vector<Point>& thePoints) : Quadrilateral(theName,thePoints), Trapezoid(theName,thePoints) 
{
	if(numEqualSides() < 2)
	{
		throw 5;	
	}
}

Rectangle::Rectangle(const std::string& theName, const std::vector<Point>& thePoints) : Quadrilateral(theName, thePoints), Trapezoid(theName,thePoints), Parallelogram(theName, thePoints), IsoscelesTrapezoid(theName, thePoints)
{
	if(!HasARightAngle())
	{
		throw 5;
	}
}

Rhombus::Rhombus(const std::string& theName, const std::vector<Point>& thePoints) : Quadrilateral(theName, thePoints), Trapezoid(theName,thePoints), Parallelogram(theName, thePoints), Kite(theName, thePoints)
{
	if(numEqualSides() < 6)
		throw 5;
}



//triangles

Triangle::Triangle(const std::string& theName, const std::vector<Point>& thePoints) : Polygon(theName, thePoints)
{
	if(getNumPoints() != 3)
	{
		throw 5;
	}
}

IsoscelesTriangle::IsoscelesTriangle(const std::string& theName, const std::vector<Point>& thePoints) : Triangle(theName, thePoints)
{
	if(numEqualSides() < 1)
	{
		throw 5;
	}
}

RightTriangle::RightTriangle(const std::string& theName, const std::vector<Point>& thePoints) : Triangle(theName,thePoints)
{
	if(!HasARightAngle())
	{
		throw 5;
	}
}

EquilateralTriangle::EquilateralTriangle(const std::string& theName, const std::vector<Point>& thePoints) : Triangle(theName, thePoints), IsoscelesTriangle(theName, thePoints)
{
	if(numEqualSides() < 2)
	{
		throw 5;
	}
}


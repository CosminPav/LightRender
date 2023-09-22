#pragma once


	class Point
	{
	public:
		int X{ 0 };
		int Y{ 0 };

		Point() : X(0), Y(0){}
		Point(int x, int y) : X(x), Y(y) {}

		//Copy constructor
		Point(const Point& point) : X(point.X) {}

		~Point() {}
	};



#pragma once


namespace Math
{
	class Vector2D
	{
	public:
		float X, Y = 0;

		//Defalt constructor
		Vector2D() : X(0), Y(0)
		{}

		//overloaded constructor
		Vector2D(float x, float y) : X(x), Y(y)
		{}
		//Copy constructor
		Vector2D(const Vector2D& vector) : X(vector.X), Y(vector.Y)
		{}

		Vector2D operator*(float Num) const
		{
			return Vector2D(X * Num, Y * Num);
		}
		Vector2D operator+(const Vector2D& Vec) const
		{
			return Vector2D(X + Vec.X, Y + Vec.Y);
		}
		Vector2D operator-(const Vector2D& Source) const {
			return Vector2D(X - Source.X, Y - Source.Y);
		}

		~Vector2D()
		{}
	};
}


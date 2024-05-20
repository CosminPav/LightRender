9#pragma once

#include <cmath>

namespace Math 
{
	class Vector3D
	{
	public:
		float X, Y, Z = 0;
		
		//Defalt constructor
		Vector3D() : X(0), Y(0), Z(0)
		{}

		//overloaded constructor
		Vector3D(float x, float y, float z) : X(x), Y(y), Z(z)
		{}
		//Copy constructor
		Vector3D(const Vector3D& vector) : X(vector.X), Y(vector.Y), Z(vector.Z)
		{}  //Delete this, move only class


		static Vector3D Lerp(const Vector3D& Start, const Vector3D& End, const float& DeltaTime) 
		{
			Vector3D Vec;
			Vec.X = Start.X * (1.0f - DeltaTime) + End.X * (DeltaTime);
			Vec.Y = Start.Y* (1.0f - DeltaTime) + End.Y * (DeltaTime);
			Vec.Z = Start.Z* (1.0f - DeltaTime) + End.Z * (DeltaTime);
			return Vec;
		}

		static Vector3D Normalize(const Vector3D& Vector) 
		{
			Vector3D Res;
			float Length = sqrt((Vector.X * Vector.X) + (Vector.Y * Vector.Y) + (Vector.Z * Vector.Z));
			if (!Length)
				return Vector3D();
			Res.X = Vector.X / Length;
			Res.Y = Vector.Y / Length;
			Res.Z = Vector.Z / Length;

			return Res;
		}

		static Vector3D CrossProduct(const Vector3D& Vec_1, const Vector3D& Vec_2) {
			Vector3D Res;

			Res.X = (Vec_1.Y * Vec_2.Z) - (Vec_1.Z * Vec_2.Y);
			Res.Y = (Vec_1.Z * Vec_2.X) - (Vec_1.X * Vec_2.Z);
			Res.Z = (Vec_1.X * Vec_2.Y) - (Vec_1.Y * Vec_2.X);

			return Res;
		}

		Vector3D operator*(float Num) const
		{
			return Vector3D(X * Num, Y * Num, Z * Num);
		}
		Vector3D operator+(const Vector3D& Vec) const
		{
			return Vector3D(X + Vec.X, Y +Vec.Y, Z + Vec.Z);
		}
		Vector3D operator-(const Vector3D& Vec) const {
			return Vector3D(X - Vec.X, Y - Vec.Y, Z - Vec.Z);
		}

		//Copy assignment opertator
		//Vector3D& operator=(const Vector3D& vec) noexcept
		//{
		//	X = vec.X; Y = vec.Y; Z = vec.Z;
		//	return *this;
		//}

		////Move assignment operator
		//Vector3D& operator=(Vector3D&& vec) noexcept
		//{
		//	X = std::move(vec.X); Y =std::move( vec.Y); Z = std::move(vec.Z);
		//	return *this;
		//}

		~Vector3D()
		{}
	};
}


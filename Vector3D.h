#pragma once


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
		{}

		//Move Constructor
		Vector3D(Vector3D&& vector) noexcept : X(vector.X), Y(vector.Y), Z(vector.Z) 
		{}

		static Vector3D Lerp(const Vector3D& Start, const Vector3D& End, const float& DeltaTime) 
		{
			Vector3D Vec;
			Vec.X = Start.X * (1.0f - DeltaTime) + End.X * (DeltaTime);
			Vec.Y = Start.Y* (1.0f - DeltaTime) + End.Y * (DeltaTime);
			Vec.Z = Start.Z* (1.0f - DeltaTime) + End.Z * (DeltaTime);
			return Vec;
		}

		Vector3D operator*(float Num) 
		{
			return Vector3D(X * Num, Y * Num, Z * Num);
		}
		Vector3D operator+(Vector3D Vec)
		{
			return Vector3D(X + Vec.X, Y +Vec.Y, Z + Vec.Z);
		}

		//Copy assignment opertator
		Vector3D& operator=(const Vector3D& vec)
		{
			X = std::move(vec.X); Y = std::move(vec.Y); Z = std::move(vec.Z);
			return *this;
		}

		//Move assignment operator
		Vector3D& operator=(Vector3D&& vec) noexcept
		{
			X = vec.X; Y = vec.Y; Z = vec.Z;
			return *this;
		}

		~Vector3D()
		{}
	};
}


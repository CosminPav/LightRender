#pragma once
namespace Math
{
	class Vector4D
	{

	public:
		float X{ 0.0f };
		float Y{ 0.0f };
		float Z{ 0.0f };
		float A{ 0.0f };

		Vector4D() {}
		Vector4D(float x, float y, float z, float a) : X(x), Y(y), Z(z), A(a)
		{}

		Vector4D(const Vector4D& Vec4) : X(Vec4.X), Y(Vec4.Y), Z(Vec4.Z), A(Vec4.A)
		{}

		void Cross(Vector4D& v1, Vector4D& v2, Vector4D &v3) 
		{
			X = v1.Y * (v2.Z * v3.A - v3.Z * v2.A) - v1.Z * (v2.Y * v3.A - v3.Y * v2.A) + v1.A * (v2.Y * v3.Z - v2.Z * v3.Y);
			Y = -(v1.X * (v2.Z * v3.A - v3.Z * v2.A) - v1.Z * (v2.X * v3.A - v3.X * v2.A) + v1.A * (v2.A * v3.Z - v3.X * v2.Z));
			Z = v1.X * (v2.Y * v3.A - v3.Y * v2.A) - v1.Y * (v2.X * v3.A - v3.X * v2.A) + v1.A * (v2.A * v3.Y - v3.X * v2.Y);
			A = -(v1.X * (v2.Y * v3.Z - v3.Y * v2.Z) - v1.Y * (v2.X * v3.Z - v3.X * v2.Z) + v1.Z * (v2.X * v3.Y - v3.X * v2.Y));
		}

		~Vector4D() = default;
	};
}


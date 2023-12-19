#pragma once
#include <memory>
#include "Vector3D.h"
#include "Vector4D.h"

namespace Math
{
	class Matrix4X4 
	{
		float mMat[4][4] = {};

	public:
		//default constructor
		Matrix4X4()
		{}

		void SetIdentity() {
			memset(mMat, 0, sizeof(float)*16);
			mMat[0][0] = 1;
			mMat[1][1] = 1;
			mMat[2][2] = 1;
			mMat[3][3] = 1;
		}

		//TRANSLATION MATRIX

		void SetTranslation(const Vector3D& TranslationVector)
		{
			mMat[3][0] = TranslationVector.X;
			mMat[3][1] = TranslationVector.Y;
			mMat[3][2] = TranslationVector.Z;
		}
		//Scale matrix
		void SetScale(const Vector3D& ScaleVector) 
		{
			mMat[0][0] = ScaleVector.X;
			mMat[1][1] = ScaleVector.Y;
			mMat[2][2] = ScaleVector.Z;
		}

		//Rotation matrix
		void SetRotationX(float x)
		{
			mMat[1][1] = cos(x);
			mMat[1][2] = sin(x);
			mMat[2][1] = -sin(x);
			mMat[2][2] = cos(x);
		}

		void SetRotationY(float y)
		{
			mMat[0][0] = cos(y);
			mMat[0][2] = -sin(y);
			mMat[2][0] = sin(y);
			mMat[2][2] = cos(y);
		}

		void SetRotationZ(float z)
		{
			mMat[0][0] = cos(z);
			mMat[0][1] = sin(z);
			mMat[1][0] = -sin(z);
			mMat[1][1] = cos(z);
		}


		void SetOrthiH(float Width, float Height, float NearPlane, float FarPlane)
		{
			SetIdentity();
			mMat[0][0] = 2.0f / Width;
			mMat[1][1] = 2.0f / Height;
			mMat[2][2] = 1.0f / (FarPlane - NearPlane);
			mMat[3][2] = -(NearPlane / (FarPlane - NearPlane));
		}

		void SetMatrix(const Matrix4X4& Mat)
		{
			memcpy(mMat, Mat.mMat, sizeof(float) * 16);
		}

		float GetDeterminant()
		{
			Vector4D minor, v1, v2, v3;
			float Det;

			v1 = Vector4D(mMat[0][0], mMat[1][0], mMat[2][0], mMat[3][0]);
			v2 = Vector4D(mMat[0][1], mMat[1][1], mMat[2][1], mMat[3][1]);
			v3 = Vector4D(mMat[0][2], mMat[1][2], mMat[2][2], mMat[3][2]);


			minor.Cross(v1, v2, v3);
			Det = -(mMat[0][3] * minor.X + mMat[1][3] * minor.Y + mMat[2][3] * minor.Z +
				mMat[3][3] * minor.A);
			return Det;
		}

		void Inverse()
		{
			int a, i, j;
			Matrix4X4 Out;
			Vector4D V, Vec[3];
			float Det = 0.0f;

			Det = GetDeterminant();
			if (!Det) return;
			for (i = 0; i < 4; i++)
			{
				for (j = 0; j < 4; j++)
				{
					if (j != i)
					{
						a = j;
						if (j > i) a = a - 1;
						Vec[a].X = (mMat[j][0]);
						Vec[a].Y = (mMat[j][1]);
						Vec[a].Z = (mMat[j][2]);
						Vec[a].A = (mMat[j][3]);
					}
				}
				V.Cross(Vec[0], Vec[1], Vec[2]);

				Out.mMat[0][i] = static_cast<float>(pow(-1.0f, i) * V.X / Det);
				Out.mMat[1][i] = static_cast<float>(pow(-1.0f, i) * V.Y / Det);
				Out.mMat[2][i] = static_cast<float>(pow(-1.0f, i) * V.Z / Det);
				Out.mMat[3][i] = static_cast<float>(pow(-1.0f, i) * V.A / Det);
			}										   

			SetMatrix(Out);
		}

		void SetPerspectiveFOVLh(float Fov, float Asspect, float zNear, float zFar) 
		{
			float yScale = static_cast<float>(1.0f / tan(Fov / 2.0f));
			float xScale = yScale / Asspect;

			mMat[0][0] = xScale;
			mMat[1][1] = yScale;
			mMat[2][2] = zFar / (zFar - zNear);
			mMat[2][3] = 1.0f;
			mMat[3][2] = (-zNear * zFar) / (zFar - zNear);
		}

		void operator*=(const Matrix4X4& matrix)
		{
			Matrix4X4 Out;
			for (size_t i = 0; i < 4; ++i) {
				for (size_t j = 0; j < 4; ++j) {
					Out.mMat[i][j] = mMat[i][0] * matrix.mMat[0][j] +
									 mMat[i][1] * matrix.mMat[1][j] +
									 mMat[i][2] * matrix.mMat[2][j] +
									 mMat[i][3] * matrix.mMat[3][j];
				}
			}
			memcpy(mMat, Out.mMat, sizeof(float) * 16);
		}


		Vector3D GetZDirection()
		{
			return Vector3D(mMat[2][0], mMat[2][1], mMat[2][2]);
		}

		Vector3D GetXDirection()
		{
			return Vector3D(mMat[0][0], mMat[0][1], mMat[0][2]);
		}
		Vector3D GetYDirection()
		{
			return Vector3D(mMat[1][0], mMat[1][1], mMat[1][2]);
		}
		Vector3D GetTranslation()
		{
			return Vector3D(mMat[3][0], mMat[3][1], mMat[3][2]);
		}
		//default destructor
		~Matrix4X4()
		{}
	};
}

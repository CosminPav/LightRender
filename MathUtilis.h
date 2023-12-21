#pragma once

class Vector3D;

namespace Math 
{
	static float Lerp(float& Start, float& End, const float& DeltaTime)
	{
		return Start * (1.0f - DeltaTime) + End * (DeltaTime);
	}
}
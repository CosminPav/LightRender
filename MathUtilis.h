#pragma once

class Vector3D;

namespace Math 
{
	float Lerp(float& Start, float& End, const float& DeltaTime)
	{
		return Start * (1.0f - DeltaTime) + End * (DeltaTime);
	}
}
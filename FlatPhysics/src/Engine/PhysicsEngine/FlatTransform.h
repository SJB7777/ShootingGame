#pragma once
#include <cmath>
#include "FlatVector.h"

namespace FlatPhysics
{	
	class FlatVector;

	struct FlatTransform
	{
		float positionX;
		float positionY;
		float Sin;
		float Cos;

		FlatTransform(FlatVector position, float angle);

		FlatTransform(float x, float y, float angle);
		

		static FlatTransform Zero() { return FlatTransform(0, 0, 0); }
	};
}
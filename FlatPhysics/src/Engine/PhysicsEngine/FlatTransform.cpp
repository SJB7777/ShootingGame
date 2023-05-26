#include "FlatTransform.h"

namespace FlatPhysics
{
	FlatTransform::FlatTransform(FlatVector position, float angle) :
		positionX(position.x), positionY(position.y), Sin(sin(angle)), Cos(cos(angle)) { }

	FlatTransform::FlatTransform(float x, float y, float angle) :
		positionX(x), positionY(y), Sin(sin(angle)), Cos(cos(angle)) { }
}

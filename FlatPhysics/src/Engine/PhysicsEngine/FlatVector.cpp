#include "FlatVector.h"

namespace FlatPhysics {

	FlatVector FlatVector::operator+ (const FlatVector &other) const
	{
		return FlatVector(x + other.x, y + other.y);
	}

	FlatVector FlatVector::operator- (const FlatVector& other) const
	{
		return FlatVector(x - other.x, y - other.y);
	}

	FlatVector FlatVector::operator* (const float s) const
	{
		return FlatVector(x * s, y * s);
	}

	FlatVector operator* (const float s, const FlatVector& v)
	{
		return FlatVector(v.x * s, v.y * s);
	}

	FlatVector FlatVector::operator/ (const float s) const
	{
		return FlatVector(x / s, y / s);
	}

	bool FlatVector::operator== (const FlatVector& other) const
	{
		return (x == other.x && y == other.y);
	}

	FlatVector FlatVector::Transform(FlatVector v, FlatTransform transform)
	{
		return FlatVector(
			transform.Cos * v.x - transform.Sin * v.y + transform.positionX,
			transform.Sin * v.x + transform.Cos * v.y + transform.positionY);
	}

}
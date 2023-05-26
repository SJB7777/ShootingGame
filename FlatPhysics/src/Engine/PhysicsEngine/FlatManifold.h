#pragma once
#include "FlatBody.h"

namespace FlatPhysics
{
	class FlatBody;

	struct FlatManifold
	{
		MultiBody* BodyA;
		MultiBody* BodyB;
		FlatVector Normal;
		float Depth;
		FlatVector Contact1;
		FlatVector Contact2;
		int ContactCount;

		FlatManifold() {}

		FlatManifold(
			MultiBody* bodyA, MultiBody* bodyB,
			const FlatVector& normal, float depth,
			const FlatVector& contact1, const FlatVector& contact2, int contactCount);
		
	};
}

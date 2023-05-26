#include "FlatManifold.h"

namespace FlatPhysics
{
	FlatManifold::FlatManifold(
		MultiBody* bodyA, MultiBody* bodyB,
		const FlatVector& normal, float depth,
		const FlatVector& contact1, const FlatVector& contact2, int contactCount) :
		BodyA(bodyA), BodyB(bodyB), Normal(normal), Depth(depth),
		Contact1(contact1), Contact2(contact2), ContactCount(contactCount)
	{}
}
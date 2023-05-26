#pragma once
#include "FlatVector.h"
#include "FlatMath.h"
#include "FlatBody.h"
#include "FlatAABB.h"
#include <vector>
#include <limits>
#include <cmath>

namespace FlatPhysics
{
	class FlatBody;

	class Collisions
	{

	public: 
		static bool IntersectAABBs(const FlatAABB& a, const FlatAABB& b);
		
		static bool Collide(FlatBody* bodyA, FlatBody* bodyB, FlatVector& normal, float& depth);

		static void FindContactPoints(
			FlatBody* bodyA, FlatBody* bodyB,
			FlatVector& contact1, FlatVector& contact2,
			int& contactCount);

	private:

		static void PointSegmentDistance(
			const FlatVector& p, const FlatVector& a, const FlatVector& b,
			float& distanceSquared, FlatVector& contact);

		static void FindPolygonsContactPoints(
			const std::vector<FlatVector>& verticesA, const std::vector<FlatVector>& verticesB,
			FlatVector& contact1, FlatVector& contact2, int& contactCount);

		static void FindCirclePolygonContactPoint(
			const FlatVector& circleCenter, float circleRadius,
			const FlatVector& polygonCenter, const std::vector<FlatVector>& polygonVertices,
			FlatVector& cp);

		static void FindCirclesContactPoint(const FlatVector& centerA, float radiusA, const FlatVector& centerB, FlatVector& cp);

		bool static IntersectCirclePolygon(const FlatVector& circleCenter, const float circleRadius,
			const FlatVector& polygonCenter, const std::vector<FlatVector>& vertices,
			FlatVector& normal, float& depth);

		bool static IntersectPolygons(const FlatVector& centerA, const std::vector<FlatVector>& verticesA,
			const FlatVector& centerB, const std::vector<FlatVector>& verticesB,
			FlatVector& normal, float& depth);

		bool static IntersectCircles(
			const FlatVector& centerA, const float radiusA, const FlatVector& centerB, const float radiusB,
			FlatVector& normal, float& depth);
		
		
	private:
		
		int static FindClosestPointOnPolygon(const FlatVector& circleCenter, const std::vector<FlatVector>& vertices);

		void static ProjectCircle(const FlatVector& center, const float radius, const FlatVector& axis, float& min, float& max);

		void static ProjectVertices(const std::vector<FlatVector>& vertices, const FlatVector& axis, float& min, float& max);
	};
}

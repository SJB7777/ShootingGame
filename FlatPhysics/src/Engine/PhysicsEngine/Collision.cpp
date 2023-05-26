#include "Collisions.h"

namespace FlatPhysics
{
	bool Collisions::IntersectAABBs(const FlatAABB& a, const FlatAABB& b)
	{
		return !(a.Max.x <= b.Min.x || b.Max.x <= a.Min.x ||
			a.Max.y <= b.Min.y || b.Max.y <= a.Min.y);
	}

	
	void Collisions::PointSegmentDistance(
		const FlatVector& p, const FlatVector& a, const FlatVector& b,
		float& distanceSquared, FlatVector& cp)
	{
		FlatVector ab = b - a;
		FlatVector ap = p - a;

		float proj = FlatMath::Dot(ap, ab);
		float abLenSq = FlatMath::LengthSquared(ab);
		float d = proj / abLenSq;

		if (d <= 0.0f)
		{
			cp = a;
		}
		else if (d >= 1.0f)
		{
			cp = b;
		}
		else
		{
			cp = a + ab * d;
		}

		distanceSquared = FlatMath::DistanceSquared(p, cp);
	}

	void Collisions::FindPolygonsContactPoints(const std::vector<FlatVector>& verticesA, const std::vector<FlatVector>& verticesB, FlatVector& contact1, FlatVector& contact2, int& contactCount)
	{
		contact1 = FlatVector::Zero();
		contact2 = FlatVector::Zero();
		contactCount = 0;

		float minDistSq = std::numeric_limits<float>::max();

		for (int i = 0; i < verticesA.size(); i++)
		{
			FlatVector p = verticesA[i];

			for (int j = 0; j < verticesB.size(); j++)
			{
				FlatVector va = verticesB[j];
				FlatVector vb = verticesB[(j + 1) % verticesB.size()];

				float distSq;
				FlatVector cp;
				PointSegmentDistance(p, va, vb, distSq, cp);
				if (FlatMath::NearlyEqual(distSq, minDistSq))
				{
					if (!FlatMath::NearlyEqual(cp, contact1))
					{
						contact2 = cp;
						contactCount = 2;
					}
					
				}
				else if (distSq < minDistSq)
				{
					minDistSq = distSq;
					contactCount = 1;
					contact1 = cp;
				}
			}

			for (int i = 0; i < verticesB.size(); i++)
			{
				FlatVector p = verticesB[i];

				for (int j = 0; j < verticesA.size(); j++)
				{
					FlatVector va = verticesA[j];
					FlatVector vb = verticesA[(j + 1) % verticesA.size()];

					float distSq;
					FlatVector cp;
					PointSegmentDistance(p, va, vb, distSq, cp);
					if (FlatMath::NearlyEqual(distSq, minDistSq))
					{
						if (!FlatMath::NearlyEqual(cp, contact1))
						{
							contact2 = cp;
							contactCount = 2;
						}

					}
					else if (distSq < minDistSq)
					{
						minDistSq = distSq;
						contactCount = 1;
						contact1 = cp;
					}
				}
			}
		}
	}

	void Collisions::FindCirclePolygonContactPoint(
		const FlatVector& circleCenter, float circleRadius,
		const FlatVector& polygonCenter, const std::vector<FlatVector>& polygonVertices,
		FlatVector& cp)
	{
		cp = FlatVector::Zero();

		float minDistSq = std::numeric_limits<float>::max();

		for (int i = 0; i < polygonVertices.size(); i++)
		{
			FlatVector va = polygonVertices[i];
			FlatVector vb = polygonVertices[(i + 1) % polygonVertices.size()];

			float distSq;
			FlatVector contact;
			PointSegmentDistance(circleCenter, va, vb, distSq, contact);

			if (distSq < minDistSq)
			{
				minDistSq = distSq;
				cp = contact;
			}
		}
	}

	void Collisions::FindCirclesContactPoint(const FlatVector& centerA, float radiusA, const FlatVector& centerB, FlatVector& cp)
	{
		FlatVector ab = centerB - centerA;
		FlatVector dir = FlatMath::Normalize(ab);
		cp = centerA + dir * radiusA;
	}


	void Collisions::FindContactPoints(
		FlatBody* bodyA, FlatBody* bodyB,
		FlatVector& contact1, FlatVector& contact2,
		int& contactCount)
	{
		contact1 = FlatVector::Zero();
		contact2 = FlatVector::Zero();
		contactCount = 0;

		ShapeType shapeTypeA = bodyA->shapeType;
		ShapeType shapeTypeB = bodyB->shapeType;
		
		if (shapeTypeA == ShapeType::Box)
		{
			if (shapeTypeB == ShapeType::Box)
			{
				FindPolygonsContactPoints(bodyA->GetTransformedVertices(), bodyB->GetTransformedVertices(),
					contact1, contact2, contactCount);
			}

			else if (shapeTypeB == ShapeType::Circle)
			{
				FindCirclePolygonContactPoint(bodyB->GetPosition(), bodyB->Radius, bodyA->GetPosition(), bodyA->GetTransformedVertices(), contact1);
				contactCount = 1;
			}
		}

		else if (shapeTypeA == ShapeType::Circle)
		{
			if (shapeTypeB == ShapeType::Box)
			{
				FindCirclePolygonContactPoint(bodyA->GetPosition(), bodyA->Radius, bodyB->GetPosition(), bodyB->GetTransformedVertices(), contact1);
				contactCount = 1;
			}

			else if (shapeTypeB == ShapeType::Circle)
			{
				FindCirclesContactPoint(bodyA->GetPosition(), bodyA->Radius, bodyB->GetPosition(), contact1);
				contactCount = 1;
			}
		}
	}



	bool Collisions::Collide(FlatBody* bodyA, FlatBody* bodyB, FlatVector& normal, float& depth)
	{
		normal = FlatVector::Zero();
		depth = 0.0f;
		
		ShapeType shapeTypeA = bodyA->shapeType;
		ShapeType shapeTypeB = bodyB->shapeType;
		
		if (shapeTypeA == ShapeType::Box)
		{
			
			if (shapeTypeB == ShapeType::Box)
			{
				
				return Collisions::IntersectPolygons(
					bodyA->GetPosition(), bodyA->GetTransformedVertices(),
					bodyB->GetPosition(), bodyB->GetTransformedVertices(),
					normal, depth);
			}

			else if (shapeTypeB == ShapeType::Circle)
			{
				
				
				bool result = Collisions::IntersectCirclePolygon(
					bodyB->GetPosition(), bodyB->Radius,
					bodyA->GetPosition(), bodyA->GetTransformedVertices(),
					normal, depth);
				normal = -normal;
				return result;
			}

			
		}

		else if (shapeTypeA == ShapeType::Circle)
		{
			
			if (shapeTypeB == ShapeType::Box)
			{
				
				return Collisions::IntersectCirclePolygon(
					bodyA->GetPosition(), bodyA->Radius,
					bodyB->GetPosition(), bodyB->GetTransformedVertices(),
					normal, depth);
			}

			else if (shapeTypeB == ShapeType::Circle)
			{
				
				return Collisions::IntersectCircles(
					bodyA->GetPosition(), bodyA->Radius,
					bodyB->GetPosition(), bodyB->Radius,
					normal, depth);
			}
		}

		return false;
	}

	bool Collisions::IntersectCirclePolygon(const FlatVector& circleCenter, const float circleRadius,
		const FlatVector& polygonCenter, const std::vector<FlatVector>& vertices,
		FlatVector& normal, float& depth)
	{
		normal = FlatVector::Zero();
		depth = std::numeric_limits<float>::max();

		
		FlatVector axis;
		float axisDepth = 0.0f;
		float minA, maxA, minB, maxB;

		for (int i = 0; i < vertices.size(); i++)
		{
			FlatVector va = vertices[i];
			FlatVector vb = vertices[(i + 1) % vertices.size()];

			FlatVector edge = vb - va;
			axis = FlatVector(-edge.y, edge.x);
			axis = FlatMath::Normalize(axis);


			ProjectVertices(vertices, axis, minA, maxA);
			ProjectCircle(circleCenter, circleRadius, axis, minB, maxB);

			if (minA >= maxB || minB >= maxA)
			{
				return false;
			}

			axisDepth = std::min(maxB - minA, maxA - minB);

			if (axisDepth < depth)
			{
				depth = axisDepth;
				normal = axis;
			}
		}

		int cpIndex = FindClosestPointOnPolygon(circleCenter, vertices);
		
		FlatVector cp = vertices[cpIndex];

		axis = cp - circleCenter;
		axis = FlatMath::Normalize(axis);

		ProjectVertices(vertices, axis, minA, maxA);
		ProjectCircle(circleCenter, circleRadius, axis, minB, maxB);

		if (minA >= maxB || minB >= maxA)
		{
			return false;
		}

		axisDepth = std::min(maxB - minA, maxA - minB);

		if (axisDepth < depth)
		{
			depth = axisDepth;
			normal = axis;
		}




		FlatVector direction = polygonCenter - circleCenter;

		if (FlatMath::Dot(direction, normal) < 0)
		{
			normal = -normal;
		}


		return true;
	}

	bool Collisions::IntersectPolygons(const FlatVector& centerA, const std::vector<FlatVector>& verticesA,
		const FlatVector& centerB, const std::vector<FlatVector>& verticesB,
		FlatVector& normal, float& depth)
	{
		normal = FlatVector::Zero();
		depth = std::numeric_limits<float>::max();

		for (int i = 0; i < verticesA.size(); i++)
		{
			FlatVector va = verticesA[i];
			FlatVector vb = verticesA[(i + 1) % verticesA.size()];

			FlatVector edge = vb - va;
			FlatVector axis(-edge.y, edge.x);
			axis = FlatMath::Normalize(axis);

			float minA, maxA, minB, maxB;
			ProjectVertices(verticesA, axis, minA, maxA);
			ProjectVertices(verticesB, axis, minB, maxB);

			if (minA >= maxB || minB >= maxA)
			{
				return false;
			}


			float axisDepth = std::min(maxB - minA, maxA - minB);

			if (axisDepth < depth)
			{
				depth = axisDepth;
				normal = axis;
			}

		}


		for (int i = 0; i < verticesB.size(); i++)
		{
			FlatVector va = verticesB[i];
			FlatVector vb = verticesB[(i + 1) % verticesB.size()];

			FlatVector edge = vb - va;
			FlatVector axis(-edge.y, edge.x);
			axis = FlatMath::Normalize(axis);

			float minA, maxA, minB, maxB;
			ProjectVertices(verticesA, axis, minA, maxA);
			ProjectVertices(verticesB, axis, minB, maxB);

			if (minA >= maxB || minB >= maxA)
			{
				return false;
			}

			float axisDepth = std::min(maxB - minA, maxA - minB);

			if (axisDepth < depth)
			{
				depth = axisDepth;
				normal = axis;
			}
		}


		FlatVector direction = centerB - centerA;

		if (FlatMath::Dot(direction, normal) < 0)
		{
			normal = -normal;
		}
		
		return true;
	}



	bool Collisions::IntersectCircles(
		const FlatVector& centerA, const float radiusA, const FlatVector& centerB, const float radiusB,
		FlatVector& normal, float& depth)
	{
		normal = FlatVector::Zero();
		depth = 0.0f;
		float distance = FlatMath::Distance(centerA, centerB);
		float radii = radiusA + radiusB;

		if (distance >= radii) {
			return false;
		}

		normal = FlatMath::Normalize(centerB - centerA);
		depth = radii - distance;

		return true;
	}

	

	

	int Collisions::FindClosestPointOnPolygon(const FlatVector& circleCenter, const std::vector<FlatVector>& vertices)
	{
		
		
		int result = -1;
		float minDistanceSq = std::numeric_limits<float>::max();

		for (int i = 0; i < vertices.size(); i++)
		{
			FlatVector v = vertices[i];
			float distanceSq = FlatMath::DistanceSquared(v, circleCenter);

			if (distanceSq < minDistanceSq)
			{
				minDistanceSq = distanceSq;
				result = i;
			}
		}

		return result;
	}

	void Collisions::ProjectCircle(const FlatVector& center, const float radius, const FlatVector& axis, float& min, float& max)
	{
		FlatVector direction = FlatMath::Normalize(axis);
		FlatVector directionAndRadius = direction * radius;

		FlatVector p1 = center + directionAndRadius;
		FlatVector p2 = center - directionAndRadius;

		min = FlatMath::Dot(p1, axis);
		max = FlatMath::Dot(p2, axis);

		if (min > max)
		{
			// sawp the min and max values
			float t = min;
			min = max;
			max = t;
		}
	}

	void Collisions::ProjectVertices(const std::vector<FlatVector>& vertices, const FlatVector& axis, float& min, float& max)
	{


		min = std::numeric_limits<float>::max();
		max = std::numeric_limits<float>::lowest();

		for (int i = 0; i < vertices.size(); i++)
		{
			FlatVector v = vertices[i];
			float proj = FlatMath::Dot(v, axis);

			if (proj < min) { min = proj; }
			if (proj > max) { max = proj; }
		}
	}


}
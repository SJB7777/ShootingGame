#pragma once
#include <algorithm>
#include "FlatVector.h"

namespace FlatPhysics
{

	struct FlatAABB
	{
	private:
		float calculateArea() const { return getWidth() * getHeight(); }
	public:

		FlatVector Min;
		FlatVector Max;
		
		float Area;

		FlatAABB() {};
		FlatAABB(const FlatVector& min, const FlatVector& max);
		FlatAABB(float minX, float minY, float maxX, float maxY);

		// Merge this AABB with another AABB
		FlatAABB Merge(const FlatAABB& other) const;

		// Calculate the center of the AABB
		FlatVector center() const;
		// Calculate the extents (half the width and height) of the AABB
		FlatVector extents() const;

		// Check if this AABB overlaps with another AABB
		bool overlaps(const FlatAABB& other) const;

		bool contains(const FlatAABB& other) const
		{
			return other.Min.x >= Min.x &&
				other.Max.x <= Max.x &&
				other.Min.y >= Min.y &&
				other.Max.y <= Max.y;
				
		}

		FlatVector GetSize() const;

		FlatAABB& operator=(const FlatAABB& other);

		float getWidth() const { return Max.x - Min.x; }
		float getHeight() const { return Max.y - Min.y; }
		
	};
}